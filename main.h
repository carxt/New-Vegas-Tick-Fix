#pragma once


float* g_FPSGlobal = (float*)(0x11F6398);
UINT32 LastTime;
UINT32 OriginalFunction;
float* fMaxTime = (float*)0x1267B38;
bool initTimeHook = false;
float DefaultMaxTime = 0;
DWORD* InterfSingleton = (DWORD*)0x11D8A80;


int g_bGTCFix = 0;
int g_bAllowBrightnessChangeWindowed = 0;
int g_bFastExit = 0;
int g_bInlineStuff = 0;
int g_bFPSFix = 0;
int g_iMaxFPS = 1;
int g_iMinFPS = 1;
int g_bSpinCriticalSections = 1;
int g_bfMaxTime;
int g_bEnableExperimentalHooks = 0;
int g_bRemoveRCSafeGuard = 0;
int g_bRemoveRendererLockSafeGuard = 0;
int g_bTweakMiscCriticalSections = 0;
int g_bSpiderHandsFix = 0;
float g_iDialogFixMult = 1;
double	DesiredMax = 1;
double	DesiredMin = 1;
double	HavokMax = 1;
double	HavokMin = 1;
int g_bModifyDirectXBehavior = 1;
int g_bRedoHashtables = 0;
int g_bResizeHashtables;
int g_bReplaceHashingAlgorithm;
int g_bAlternateGTCFix = 0;
int g_bLightInlines = 0;
int g_bHeavyInlines = 0;
int g_bAutomaticFPSFix = 0;
int g_bUseDefaultPoolForBuffers = 0;
#include "hooks.h"
#include "FPSTimer.h"
#include "GameUI.h"
#include "direct3dhooks.h"
#include "CriticalSections.h"
#include <intrin.h>

InterfaceManager* InterfaceManager::GetSingleton()
{
	return *(InterfaceManager * *)0x11D8A80;
}

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

static Menu* GetStartMenuSingleton() { return *(Menu * *)0x11DAAC0; };
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
	if (UInt32 start = (UInt32) GetStartMenuSingleton())
	{
		if (*(UInt32*)(start  + 0x1A8) & StartMenuFlags::kHasChangedSettings)
		{
			((void (*)(void))(0x7D6D70))(); // SaveIniSettings
		}
	}
	TerminateProcess(GetCurrentProcess(), 0);
}

int __fastcall hk_OSGlobalsExit(void* thisObj)
{
	ResetBrightness();
	return ThisStdCall(0x5B6CB0, thisObj);
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


 /*
void __stdcall TimeGlobalHook(void* unused) {
	double Delta = GetFPSCounterMiliSeconds(); 
	if (*g_IsMenuMode)
	{
		
		if (*g_DialogMenu2 || *g_DialogMenu)
		{
			*g_FPSGlobal = (Delta > 0) ? ((Delta < DesiredMin) ? ((Delta > DesiredMax / g_iDialogFixMult) ? Delta : DesiredMax / g_iDialogFixMult) : DesiredMin) : 0;
			if (g_bfMaxTime)* fMaxTime = ((Delta > 0 && Delta < DefaultMaxTime && Delta > DesiredMax) ? Delta / 1000 : DefaultMaxTime / 1000);
		}
		else
		{
			*g_FPSGlobal = 0;
			*fMaxTime = DefaultMaxTime;
		}
	}
	else
	{
		*g_FPSGlobal = (Delta > 0) ? ((Delta < DesiredMin) ? ((Delta > DesiredMax) ? Delta : DesiredMax) : DesiredMin) : 0;
		if (g_bfMaxTime)* fMaxTime = ((Delta > 0 && Delta < DefaultMaxTime && Delta > DesiredMax) ? Delta / 1000 : DefaultMaxTime / 1000);
	}
	if (g_bSpiderHandsFix > 0 && *g_FPSGlobal > FLT_EPSILON)
	{
		//__asm {int 3}
		*g_FPSGlobal = 1000 / ((1000 / *g_FPSGlobal) * 0.987);
		if (g_bfMaxTime)* fMaxTime = *g_FPSGlobal / 1000;
	}

}
*/

void __stdcall TimeGlobalHook(void* unused) {

	double Delta = GetFPSCounterMiliSeconds();
	if (g_bfMaxTime)* fMaxTime = ((Delta > 0 && Delta < DefaultMaxTime && Delta > DesiredMax) ? Delta / 1000 : DefaultMaxTime / 1000);

	if (*g_IsMenuMode)
	{

		if (*g_DialogMenu2 || *g_DialogMenu)
		{
			*g_FPSGlobal = (Delta > 0) ? ((Delta < DesiredMin) ? ((Delta > DesiredMax / g_iDialogFixMult) ? Delta : DesiredMax / g_iDialogFixMult) : DesiredMin) : 0;

		}
		else
		{
			*g_FPSGlobal = 0;
		}
	}
	else
	{
		*g_FPSGlobal = (Delta > 0) ? ((Delta < DesiredMin) ? ((Delta > DesiredMax) ? Delta : DesiredMax) : DesiredMin) : 0;
	}

	if (g_bSpiderHandsFix > 0 && *g_FPSGlobal > FLT_EPSILON)
	{
		//__asm {int 3}
		*g_FPSGlobal = 1000 / ((1000 / *g_FPSGlobal) * 0.987);
		if (g_bfMaxTime)* fMaxTime = *g_FPSGlobal / 1000;
	}

}








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



static uintptr_t FPSFix_TimeHookCall = NULL;
void HookFPSStuff()
{
	//SafeWriteBuf(0x86E65A, "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 0x11);
	FPSFix_TimeHookCall = (uintptr_t)(&TimeGlobalHook);
		if (FPSFix_TimeHookCall) WriteRelCall((uintptr_t)0x086E667, FPSFix_TimeHookCall);
}


void DoPatches()
{
	//int g_bAllowDebugging = 1;
	//if (g_bAllowDebugging)	SafeWriteBuf(0x4DAD61, "\x90\x90\x90\x90\x90\x90\x90", 7);

	if (g_bEnableExperimentalHooks) {
		if (g_bRemoveRCSafeGuard)	RemoveRefCountSafeGuard();
		if (g_bRemoveRendererLockSafeGuard) RemoveRendererLockSafeGuard();
		if (g_bTweakMiscCriticalSections) TweakMiscCriticalSections();
		SafeWriteBuf(0x8728D7, "\x8B\xE5\x5D\xC3\x90\x90", 6);

	}
	if (g_bInlineStuff) {
		HookInlines(); //	MathHooks();
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
		_MESSAGE("TGT ENABLED");
		//timeBeginPeriod(1);
		//SafeWrite32(0xFDF060, (UInt32)timeGetTime);
		FPSStartCounter();
		uintptr_t TargetGTC = (uintptr_t)ReturnCounter;
		if (g_bAlternateGTCFix) {
			timeBeginPeriod(1);  
			SafeWrite32(0xFDF060, (UInt32)timeGetTime);
		}
		else {
			SafeWrite32(0xFDF060, TargetGTC);

			//first two are render, rest are audio
		/*	for (uintptr_t Destination : {0xC45995, 0xC45902, 0xADF0F9, 0xAE7619, 0xADDE34, 0xADEA26, 0xAE7777, 0x0AEA109})
			{
				SafeWrite8(Destination, 0x90);
				WriteRelCall(Destination + 1, TargetGTC);
			}*/
		}
		//SafeWrite32(0xFDF060, (UInt32)hk_GetTickCount);
		if (g_bFPSFix)
		{
			_MESSAGE("FPSFIX ENABLED");

				DesiredMax = 1000 / double(g_iMaxFPS);
				DesiredMin = 1000 / double(g_iMinFPS);
				DefaultMaxTime = (*fMaxTime) * 1000;
			HookFPSStuff();
		}
	}
	if (g_bModifyDirectXBehavior)
		D3DHooks::UseD3D9xPatchMemory(g_bForceD3D9Ex || g_bUseDefaultPoolForBuffers, g_bUseDynamicResources);
}



