#pragma once


float* g_FPSGlobal = (float*)(0x11F6398);
UINT32 LastTime;
UINT32 OriginalFunction;
float* fMaxTime = (float*)0x1267B38;
bool initTimeHook = false;
float fLowerMaxTimeBoundary = 0;
DWORD* InterfSingleton = (DWORD*)0x11D8A80;


int g_bGTCFix = 0;
int g_bAllowDirectXDebugging = 0;
int g_bAllowBrightnessChangeWindowed = 0;
int g_bFastExit = 0;
int g_bFPSFix = 0;
int g_iMaxFPS = 1;
int g_iMinFPS = 1;
int g_bfMaxTime;
int g_bEnableThreadingTweaks = 0;
int g_bRemoveRCSafeGuard = 0;
int g_bTweakMiscCriticalSections = 0;
int g_bReplaceDeadlockCSWithWaitAndSleep = 0;
int g_bSpiderHandsFix = 0;
double	DesiredMax = 1;
double	DesiredMin = 1000;
double	HavokMax = 1;
double	HavokMin = 1;
int g_bModifyDirectXBehavior = 1;
int g_bRedoHashtables = 0;
int g_bResizeHashtables = 1;
int g_bAlternateGTCFix = 0;
int g_bRemoveGTCLimits = 0;
int g_bAutomaticFPSFix = 0;
int g_bUseExperimentalCacheForBuffers = 0;







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
void ResetBrightness()
{
	if (foreWindow && D3DHooks::SetGammaRampInit)
	{
		HDC Devic = ::GetDC(foreWindow);
		SetDeviceGammaRamp(Devic, &D3DHooks::StartingGammaRamp);
		::ReleaseDC(foreWindow, Devic);
	}
}
void FastExit()
{
	ResetBrightness();
	/*for (auto it = MapLogger.begin(); it != MapLogger.end(); ++it)
	{
		_MESSAGE("Address 0x%X called %i times", it->first, it->second);
	}*/
	if (UInt32 start = (UInt32)GetStartMenuSingleton())
	{
		if (*(UInt32*)(start + 0x1A8) & StartMenuFlags::kHasChangedSettings)
		{
			((void (*)(void))(0x7D6D70))(); // SaveIniSettings
		}
	}
	TerminateProcess(GetCurrentProcess(), 0);
}

int __fastcall hk_OSGlobalsExit(void* thisObj)
{
	ResetBrightness();
	return ThisStdCall<int>(0x5B6CB0, thisObj);
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
bool* g_IsMenuMode = (bool*)0x11DEA2B;
bool* g_DialogMenu2 = (bool*)0x11DEA2B;


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


constexpr float fTimerOffsetMult = 0.9825; // I have no idea why this works but it does.
void __stdcall TimeGlobalHook(void* unused) {
	//FPSLimt(nullptr);
	double Delta = GetFPSCounterMiliSeconds_WRAP();
	//FPSLimitClock = std::chrono::steady_clock::now();
	if (g_bfMaxTime)*fMaxTime = (Delta > FLT_EPSILON) ? ((Delta < DesiredMin) ? (Delta > DesiredMax ? Delta / 1000 : DesiredMax / 1000) : DesiredMin / 1000) : fLowerMaxTimeBoundary / 1000 ;

	if (!*g_IsMenuMode || !(!*g_DialogMenu2 && !*g_DialogMenu))
	{
		*g_FPSGlobal = (Delta > 0) ? ((Delta < DesiredMin) ? ((Delta > DesiredMax) ? Delta : DesiredMax) : DesiredMin) : 0;
	}
	else
	{
		*g_FPSGlobal = 0;
	}

	if (g_bSpiderHandsFix > 0 && *g_FPSGlobal > FLT_EPSILON)
	{
		*g_FPSGlobal = 1000 / ((1000 / *g_FPSGlobal) * fTimerOffsetMult);
		if (g_bfMaxTime) *fMaxTime = 1000 / ((1000 / *fMaxTime) * fTimerOffsetMult);
		if (*fMaxTime < FLT_EPSILON) *fMaxTime = FLT_EPSILON;
	//	if (*fMaxTime > ((DesiredMin / 1000))) *fMaxTime = ((DesiredMin / 1000));
	}

}




void __stdcall TimeGlobalHook_NoSafeGuards(void* unused) {

	double Delta = GetFPSCounterMiliSeconds_WRAP();
	if (g_bfMaxTime)*fMaxTime = ((Delta > 0 && Delta < fLowerMaxTimeBoundary) ? (Delta > DesiredMax ? Delta / 1000 : DesiredMax / 1000) : fLowerMaxTimeBoundary / 1000);

	*g_FPSGlobal = (Delta > 0) ? ((Delta < DesiredMin) ? ((Delta > DesiredMax) ? Delta : DesiredMax) : DesiredMin) : 0;

	if (g_bSpiderHandsFix > 0 && *g_FPSGlobal > FLT_EPSILON)
	{
		*g_FPSGlobal = 1000 / ((1000 / *g_FPSGlobal) * fTimerOffsetMult);

		//if (g_bfMaxTime) *fMaxTime = 1000 / ((1000 / *fMaxTime) * fTimerOffsetMult);
		*g_FPSGlobal = 1000 / ((1000 / *g_FPSGlobal) * fTimerOffsetMult);
		if (g_bfMaxTime) *fMaxTime = 1000 / ((1000 / *fMaxTime) * fTimerOffsetMult);
		if (*fMaxTime < FLT_EPSILON) *fMaxTime = FLT_EPSILON;
	}

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
	if (g_bAllowDirectXDebugging)
	{//SafeWriteBuf(0x4DAD61, "\x90\x90\x90\x90\x90\x90\x90", 7);
		SafeWriteBuf(0x09F9968, "\xC2\x04\x00\xCC\xCC\xCC", 6);
	}
	if (g_bEnableThreadingTweaks) {
		if (g_bRemoveRCSafeGuard)	RemoveRefCountSafeGuard();
		//if (g_bRemoveRendererLockSafeGuard) RemoveRendererLockSafeGuard();
		if (g_bTweakMiscCriticalSections) TweakMiscCriticalSections();
		//	SafeWriteBuf(0x8728D7, "\x8B\xE5\x5D\xC3\x90\x90", 6);
		if (g_bReplaceDeadlockCSWithWaitAndSleep) TurnProblematicCSIntoBusyLocks();
	}

	//Fast Exit Hook
	if (g_bFastExit) {
		WriteRelJump(0x86B66E, (UInt32)FastExit);
	}
	if (g_bRedoHashtables) {
		DoHashTableStuff();

	}

	if (g_bAllowBrightnessChangeWindowed)
	{
		WriteRelCall(0x4DD119, (uintptr_t)(D3DHooks::hk_SetGammaRamp));
		for (UInt32 patchAddr : {0x5B6CA6, 0x7D0C3E, 0x86A38B})
		{
			WriteRelCall(patchAddr, (UInt32)hk_OSGlobalsExit);

		}
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

			DesiredMax = 1000 / double(g_iMaxFPS);
			DesiredMin = 1000 / double(g_iMinFPS);
			fLowerMaxTimeBoundary = (*fMaxTime) * 1000;
			HookFPSStuff();
		}
	}
	if (g_bModifyDirectXBehavior)
		D3DHooks::UseD3D9xPatchMemory(g_bUseDefaultPoolForTextures);


}



