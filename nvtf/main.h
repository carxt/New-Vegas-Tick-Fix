#pragma once
#include "float.h"

float* g_FPSGlobal = (float*)(0x11F6398);
float* fMaxTime = (float*)0x1267B38;
float fLowerMaxTimeBoundary = 0;
float fMaxTimeDefault = 0;
DWORD** BGSLoadGameSingleton = (DWORD**)0x11DDF38;


int g_bGTCFix = 0;
int g_bFastExit = 0;
int g_bFPSFix = 0;
int g_iMaxFPS = 1;
int g_iMinFPS = 1;
int g_bfMaxTime;
int g_bEnableThreadingTweaks = 0;
int g_iReplaceTextureCreationLocks = 0;
int g_iReplaceGeometryPrecacheLocks = 0;
int g_bTweakMiscCriticalSections = 0;
int g_bReplaceDeadlockCSWithWaitAndSleep = 0;
int g_bSpiderHandsFix = 0;
double	fDesiredMax = 1;
double	fDesiredMin = 1000;
int g_bModifyDirectXBehavior = 1;
int g_bRedoHashtables = 0;
int g_bResizeHashtables = 1;
int g_bAlternateGTCFix = 0;
int g_bRemoveGTCLimits = 0;

double g_fMaxTimeLowerBoundary = 0;

#include "hooks.h"
#include "FPSTimer.h"
#include "direct3dhooks.h"
#include "CriticalSections.h"
#include <intrin.h>

enum StartMenuFlags
{
	kHasChangedSettings = 0x2,
	kLoad = 0x4,
	kIsSaveMenuNotLoad = 0x8,
	kIsMainMenuShown = 0x10,
	kAreOptionsInitialised = 0x20,
	kShowDLCPopup = 0x400,
	kIsActionMapping = 0x1000,
	kShowCredits = 0x10000,
	kControllerDisconnected = 0x20000,
	kSomethingSave = 0x40000000,
	kShowMustRestartToSaveSettings = 0x400000,
	ksomething_credits = 0x2000000,
	kDeleteSave = 0x2000,
	kControllerInputDebounce = 0x4000000,
};

static uintptr_t* GetStartMenuSingleton() { return *(uintptr_t**)0x11DAAC0; };

void FastExit()
{
	if (UInt32 start = (UInt32)GetStartMenuSingleton())
	{
		if (*(UInt32*)(start + 0x1A8) & StartMenuFlags::kHasChangedSettings)
		{
			((void (*)(void))(0x7D6D70))(); // SaveIniSettings
		}
	}
	TerminateProcess(GetCurrentProcess(), 0);
}


//volatile double Delta;
bool* g_bIsInPauseFade = (bool*)0x11DEA2D;
bool* g_bIsLoadingNewGame = (bool*)0x11D8907;

constexpr float fTimerOffsetMult = 0.9875; // Formerly 0.9825
void ClampGameCounters() {

	if (g_bSpiderHandsFix > 0 && *g_FPSGlobal > FLT_EPSILON)
	{
		*g_FPSGlobal = 1000 / ((1000 / *g_FPSGlobal) * fTimerOffsetMult);
		if (g_bfMaxTime) {
			*fMaxTime = 1000 / ((1000 / *fMaxTime) * fTimerOffsetMult);
			if (*fMaxTime < FLT_EPSILON) *fMaxTime = FLT_EPSILON;
			//	if (*fMaxTime > ((DesiredMin / 1000))) *fMaxTime = ((DesiredMin / 1000));
		}
	}
	if (g_bfMaxTime) {
		if (*g_bIsInPauseFade || *g_FPSGlobal < FLT_EPSILON) {
			*fMaxTime = fMaxTimeDefault;
		}
		else if ((*fMaxTime > fLowerMaxTimeBoundary)) *fMaxTime = fLowerMaxTimeBoundary;
	}

}

void __stdcall TimeGlobalHook(void* unused) {
	double Delta = GetFPSCounterMiliSeconds_WRAP();

	if (g_bfMaxTime)*fMaxTime = (Delta > FLT_EPSILON) ? ((Delta < fDesiredMin) ? (Delta > fDesiredMax ? Delta / 1000 : fDesiredMax / 1000) : fDesiredMin / 1000) : fLowerMaxTimeBoundary ;

	if ((*BGSLoadGameSingleton && ThisStdCall<bool>((uintptr_t)0x042CE10, *BGSLoadGameSingleton)) || *g_bIsLoadingNewGame)
	{
		*g_FPSGlobal = 0;
	}
	else
	{
		*g_FPSGlobal = (Delta > 0) ? ((Delta < fDesiredMin) ? ((Delta > fDesiredMax) ? Delta : fDesiredMax) : fDesiredMin) : 0;
	}
	ClampGameCounters();
}

void __stdcall TimeGlobalHook_NoSafeGuards(void* unused) {

	double Delta = GetFPSCounterMiliSeconds_WRAP();
	if (g_bfMaxTime)*fMaxTime = ((Delta > 0 && Delta < fLowerMaxTimeBoundary * 1000) ? (Delta > fDesiredMax ? Delta / 1000 : fDesiredMax / 1000) : fLowerMaxTimeBoundary);
	*g_FPSGlobal = (Delta > 0) ? ((Delta < fDesiredMin) ? ((Delta > fDesiredMax) ? Delta : fDesiredMax) : fDesiredMin) : 0;
	ClampGameCounters();

}

static uintptr_t FPSFix_TimeHookCall = NULL;
void HookFPSStuff() 
{
	if (!g_bRemoveGTCLimits) { FPSFix_TimeHookCall = (uintptr_t)(TimeGlobalHook); }
	else { FPSFix_TimeHookCall = (uintptr_t)(TimeGlobalHook_NoSafeGuards); }
	if (FPSFix_TimeHookCall) WriteRelCall((uintptr_t)0x086E667, FPSFix_TimeHookCall);
}

void DoPatches()
{
	if (g_bEnableThreadingTweaks) {
		ReplaceTextureCreationLocks(g_iReplaceTextureCreationLocks);
		if (g_bTweakMiscCriticalSections) 
			TweakMiscCriticalSections();

		if (g_bReplaceDeadlockCSWithWaitAndSleep)
			TurnProblematicCSIntoBusyLocks();

		ReplaceGeometryPrecacheLocks(g_iReplaceGeometryPrecacheLocks);		
	}

	if (g_bFastExit)
		WriteRelJump(0x86B66E, (UInt32)FastExit);

	if (g_bRedoHashtables)
		ResizeHashTables();

	if (g_bGTCFix) {
		FPSStartCounter();
		uintptr_t TargetGTC = (uintptr_t)ReturnCounter_WRAP;
		if (g_bAlternateGTCFix)
			timeBeginPeriod(1);

		SafeWrite32(0xFDF060, TargetGTC);

		if (g_bFPSFix)
		{
			fDesiredMax = 1000.0 / double(g_iMaxFPS);
			fDesiredMin = 1000.0 / double(g_iMinFPS);
			fMaxTimeDefault = *fMaxTime;
			fLowerMaxTimeBoundary = g_fMaxTimeLowerBoundary;
			HookFPSStuff();
		}
	}

	if (g_bModifyDirectXBehavior)
		D3DHooks::Init();
}