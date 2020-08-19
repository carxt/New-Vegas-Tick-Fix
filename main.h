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
int g_bRemove0x80SafeGuard = 0;
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


#include "hooks.h"
#include "FPSTimer.h"
#include "GameUI.h"
#include "direct3dhooks.h"
#include "CriticalSections.h"


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



void TimeGlobalHook() {

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







void __declspec(naked) FPSHookHandler()
{

	__asm {
		push ecx
		call TimeGlobalHook
		pop ecx
		jmp OriginalFunction
	}

}
void HookFPSStuff()
{
	SafeWriteBuf(0x86E65A, "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 0x11);
	WriteRelCall((UINT32)0x86E65A, (UInt32)& FPSHookHandler);
	OriginalFunction = 0x86E66C;
}


void DoPatches()
{
	//int g_bAllowDebugging = 1;
	//if (g_bAllowDebugging)	SafeWriteBuf(0x4DAD61, "\x90\x90\x90\x90\x90\x90\x90", 7);
	if (g_bSpinCriticalSections) {
		_MESSAGE("CS ENABLED");
		WriteRelJump(0x0A62B08, (UInt32)NiObjectCriticalSections);
		WriteRelJump(0x0AA8D5B, (UInt32)MemHeapCSHook);
		DoHeapCriticalSectionSpin();
		if ((signed int)g_iSpinCount > -1) {
			SafeWrite32(0x0FDF054, (UInt32)InitCriticalSectionHook);
		}



	}
	if (g_bEnableExperimentalHooks) {
		if (g_bRemoveRCSafeGuard)	RemoveRefCountSafeGuard();
		if (g_bRemove0x80SafeGuard) Remove0x80SafeGuard();
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
		if (!g_bAlternateGTCFix) SafeWrite32(0xFDF060, (UInt32)ReturnCounter);
		else {
			timeBeginPeriod(1);  SafeWrite32(0xFDF060, (UInt32)timeGetTime);
		}
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
		D3DHooks::UseD3D9xPatchMemory(g_bForceD3D9Ex, g_bUseDynamicResources);
}



