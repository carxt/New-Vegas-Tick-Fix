#pragma once
#include "hooks.h"
#include "FPSTimer.h"
#include "CriticalSections.h"
#include "mathVegas.h"

#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x6000
#endif

float* g_FPSGlobal = (float*)(0x11F6398);
UINT32 LastTime;
UINT32 OriginalFunction;
float* fMaxTime = (float*)0x1267B38;
bool initTimeHook = false;
float DefaultMaxTime = 0;
DWORD* InterfSingleton = (DWORD *)0x11D8A80;


int g_bGTCFix = 0;
int g_bFastExit = 0;
int g_bInlineStuff = 0;
int g_bFPSFix = 0;
int g_iMaxFPS = 1;
int g_iMinFPS = 1;
int g_bSpinCriticalSections = 1;
int g_bfMaxTime;


double	DesiredMax = 1;
double	DesiredMin = 1;
double	HavokMax = 1;
double	HavokMin = 1;









bool IsMenuMode()
{
	if (!(*InterfSingleton) ||   !(*(BYTE*)(*InterfSingleton))) return false;
	return ((*(DWORD*)((*InterfSingleton) + 12)) != 0);

}

void TimeGlobalHook() {
	//Console_Print("%lu", timeGetTime());
	//Console_Print("%lu", (UInt32)ReturnCounter());
	double Delta = GetFPSCounterMiliSeconds();
	*g_FPSGlobal = (Delta > 0) ? ((Delta < DesiredMin) ? ((Delta > DesiredMax) ? Delta : DesiredMax) : DesiredMin) : 0;
	if (g_bfMaxTime)*fMaxTime = ((Delta > 0 && Delta < DefaultMaxTime && Delta > DesiredMax) ? Delta / 1000 : DefaultMaxTime / 1000);
}

void FastExit()
{
	TerminateProcess(GetCurrentProcess(), 0);
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
	OriginalFunction = (((*(UINT32*)(0x86B3E3 + 1)) + 5)) + 0x86B3E3;
//	_MESSAGE("%x", OriginalFunction);
	WriteRelCall((UINT32)0x86B3E3, (UInt32)&FPSHookHandler);
}

void DoPatches()
{

	if (g_bSpinCriticalSections) {
		_MESSAGE("CS ENABLED");
		WriteRelJump(0x0A62B08, (UInt32)NiObjectCriticalSections);
	//	WriteRelJump(0x0FB3343, (UInt32)someOddCSCall);
	//	WriteRelJump(0x0A5B570, (UInt32)someOddCSCall_2);
		WriteRelJump(0x0AA8D5B, (UInt32)MemHeapCSHook);
		DoHeapCriticalSectionSpin();
		if ((signed int)g_iSpinCount > -1) {
			SafeWrite32(0x0FDF054, (UInt32)InitCriticalSectionHook);
		}



	}
	
	if (g_bInlineStuff) {
		HookInlines(); //	MathHooks();
	}
	//Fast Exit Hook
	if (g_bFastExit) WriteRelJump(0x86B66E, (UInt32)FastExit);
	if (g_bGTCFix) {
		_MESSAGE("TGT ENABLED");
		//timeBeginPeriod(1);
		//SafeWrite32(0xFDF060, (UInt32)timeGetTime);
		SafeWrite32(0xFDF060, (UInt32)ReturnCounter);
		if (g_bFPSFix)
		{
			_MESSAGE("FPSFIX ENABLED");

			if (!initTimeHook) {
				DesiredMax = 1000 / double(g_iMaxFPS);
				DesiredMin = 1000 / double(g_iMinFPS);
				initTimeHook = true;
				FPSStartCounter();
				DefaultMaxTime = (*fMaxTime) * 1000;
			}
			HookFPSStuff();
		}
	}



	SafeWrite32(0xFDF05C, (UInt32)ReturnCSHook);
}