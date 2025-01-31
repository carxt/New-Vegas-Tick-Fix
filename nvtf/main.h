#pragma once


float* g_FPSGlobal = (float*)(0x11F6398);
UINT32 LastTime;
UINT32 OriginalFunction;
float* fMaxTime = (float*)0x1267B38;
bool initTimeHook = false;
float fLowerMaxTimeBoundary = 0;
float fMaxTimeDefault = 0;
DWORD** InterfSingleton = (DWORD**)0x11D8A80;
DWORD** BGSLoadGameSingleton = (DWORD**)0x11DDF38;


int g_bGTCFix = 0;
int g_bFastExit = 0;
int g_bFPSFix = 0;
int g_iMaxFPS = 1;
int g_iMinFPS = 1;
int g_bfMaxTime;
int g_bEnableThreadingTweaks = 0;
int g_iTweakRCSafeGuard = 0;
int g_iTweakMiscRendererSafeGuards = 0;
int g_bTweakMiscCriticalSections = 0;
int g_bReplaceDeadlockCSWithWaitAndSleep = 0;
int g_bSpiderHandsFix = 0;
double	fDesiredMax = 1;
double	fDesiredMin = 1000;
double	fHavokMax = 1;
double	fHavokMin = 1;
int g_bModifyDirectXBehavior = 1;
int g_bRedoHashtables = 0;
int g_bResizeHashtables = 1;
int g_bAlternateGTCFix = 0;
int g_bRemoveGTCLimits = 0;
int g_bAutomaticFPSFix = 0;
int g_bUseExperimentalCacheForBuffers = 0;
int g_bWaterLODPatch = 0;

double g_fMaxTimeLowerBoundary = 0;




#include "hooks.h"
#include "FPSTimer.h"
#include "direct3dhooks.h"
#include "CriticalSections.h"
#include <intrin.h>
#include <thread>

std::map<uintptr_t, int> MapLogger;


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










inline UInt32 RetrieveAddrFromDisp32Opcode(UInt32 address)
{
	return *(UInt32*)(address + 1) + address + 5;
}
inline UInt32 Calculaterel32(UInt32 Destination, UInt32 source)
{
	return 	Destination - source - 5;
}






//volatile double Delta;
bool* g_DialogMenu = (bool*)0X11D9514;
bool* g_DialogMenu2 = (bool*)0x11DEA2C;
bool* g_bIsMenuMode = (bool*)0x11DEA2B;
bool* g_bIsInPauseFade = (bool*)0x11DEA2D;
bool* g_bIsLoadingNewGame = (bool*)0x11D8907;


//uintptr_t OriginalDisplayCall;
std::chrono::steady_clock::time_point FPSTargetClock;
uintptr_t __fastcall FPSLimt() {
	//bool resResult = ThisStdCall_B(OriginalDisplayCall, thisObj);
	//int FPSTarget = 50;
	//int CurrentFPS = 1000 / GetFPSCounterMiliSeconds_WRAP(false);
	double FPSTarget = 50;
	auto CurrentClock = std::chrono::steady_clock::now();
	if (FPSTargetClock > CurrentClock)
	{
		std::this_thread::sleep_for(FPSTargetClock - CurrentClock);
		//std::this_thread::sleep_until(FPSTargetClock);

	}
	FPSTargetClock = std::chrono::steady_clock::now() + std::chrono::milliseconds(unsigned int(1000 / FPSTarget));
	//auto targetSleep = ((std::chrono::microseconds(std::chrono::seconds(1)) / FPSTarget) - std::chrono::microseconds(std::chrono::seconds(long long((GetFPSCounterMiliSeconds_WRAP(false) + 1) / 1000))));
	//auto target_tp = std::chrono::steady_clock::now() + targetSleep;
	//std::this_thread::sleep_until(target_tp);
	return 0;
}

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
	//FPSLimt(nullptr);
	double Delta = GetFPSCounterMiliSeconds_WRAP();
	//FPSLimitClock = std::chrono::steady_clock::now();
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


//test function, can be left out
/*
DWORD hk_GetTickCount()
{

	if (InterfaceManager::GetSingleton()->currentMode != 2)
	{
		auto retAddr = (uintptr_t)_ReturnAddress();
		auto it = MapLogger.find(retAddr);
		if (it != MapLogger.end())
		{
			it->second += 1;
		}
		else
		{
			MapLogger.insert(std::pair<uintptr_t, int>(retAddr, 1));
		}
	}
	return GetTickCount();
}
*/

__declspec (naked) void asm_FPSTrailHook()
{
	__asm
	{
		pop esi
		mov esp, ebp
		pop ebp
		jmp FPSLimt
	}
}

static uintptr_t FPSFix_TimeHookCall = NULL;
void HookFPSStuff()
{

	//WriteRelJump(0x086EF2B, (uintptr_t)asm_FPSTrailHook);
	//OriginalDisplayCall = (*(uintptr_t*)0x10EE640);
	//SafeWrite32(0x10EE640, (uintptr_t)FPSLimt);
	//SafeWriteBuf(0x86E65A, "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 0x11);
	if (!g_bRemoveGTCLimits) { FPSFix_TimeHookCall = (uintptr_t)(TimeGlobalHook); }
	else { FPSFix_TimeHookCall = (uintptr_t)(TimeGlobalHook_NoSafeGuards); }
	if (FPSFix_TimeHookCall) WriteRelCall((uintptr_t)0x086E667, FPSFix_TimeHookCall);
}



void __stdcall SleepHook(DWORD dwMiliseconds) {
	if (dwMiliseconds <= 0) {
		SwitchToThread();
	}
	Sleep(dwMiliseconds);
}

void DoPatches()
{
	if (g_bEnableThreadingTweaks) {
		TweakRefCountSafeGuard(g_iTweakRCSafeGuard);
		//if (g_bRemoveRendererLockSafeGuard) RemoveRendererLockSafeGuard();
		if (g_bTweakMiscCriticalSections) TweakMiscCriticalSections();
		//	SafeWriteBuf(0x8728D7, "\x8B\xE5\x5D\xC3\x90\x90", 6);
		if (g_bReplaceDeadlockCSWithWaitAndSleep) TurnProblematicCSIntoBusyLocks();
		TweakRendererLockSafeGuard(g_iTweakMiscRendererSafeGuards);		
	}

	//Fast Exit Hook
	if (g_bFastExit) {
		WriteRelJump(0x86B66E, (UInt32)FastExit);
	}

	if (g_bRedoHashtables) {
		DoHashTableStuff();
	}

	if (g_bGTCFix) {
		PrintLog("TGT ENABLED");
		//timeBeginPeriod(1);
		//SafeWrite32(0xFDF060, (UInt32)timeGetTime);
		FPSStartCounter();
		uintptr_t TargetGTC = (uintptr_t)ReturnCounter_WRAP;
		if (g_bAlternateGTCFix)
		{
			timeBeginPeriod(1);
		}
		SafeWrite32(0xFDF060, TargetGTC);
		//SafeWrite32(0xFDF060, (UInt32)hk_GetTickCount);
		if (g_bFPSFix)
		{
			PrintLog("FPSFIX ENABLED");

			fDesiredMax = 1000.0 / double(g_iMaxFPS);
			fDesiredMin = 1000.0 / double(g_iMinFPS);
			fMaxTimeDefault = *fMaxTime;
			fLowerMaxTimeBoundary = g_fMaxTimeLowerBoundary;
			HookFPSStuff();
		}
	}

	if (g_bModifyDirectXBehavior)
		D3DHooks::Init();

	if (g_bWaterLODPatch) {
		WriteRelCall((uintptr_t)0x6FD0D4, (uintptr_t)FakeFrustumHook);
	}
	//for (uintptr_t hookPtr : {0x045B1D4, 0x045B37F, 0x045B41B, 0x0045B4ED, 0x0045B69B, 0x045B7E0, 0x06FA772, 0x06FA7B5, 0x06FA7F8}) 
/*	for (uintptr_t hookPtr : {0x06FA772, 0x06FA7B5, 0x06FA7F8}) {
		//WriteRelCall((uintptr_t)hookPtr, (uintptr_t)hook_WaterCullCheck);

	}*/
}



