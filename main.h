#pragma once
#include "hooks.h"

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


bool IsMenuMode()
{
	if (!(*InterfSingleton) ||   !(*(BYTE*)(*InterfSingleton))) return false;
	return ((*(DWORD*)((*InterfSingleton) + 12)) != 0);
}


void TimeGlobalHook() {
	if (!initTimeHook) {
	//	timeBeginPeriod(1); //whatever
		initTimeHook = true;
		LastTime = timeGetTime();
		DefaultMaxTime = *fMaxTime;
	}
	UINT32 getTime = timeGetTime();
	double Delta = ((double)(getTime - LastTime));
	LastTime = getTime;
	if (!IsMenuMode || Delta <= 0)
	{
	
		*fMaxTime = Delta / 1000;
		*g_FPSGlobal = Delta;
	}
	else
	{
		*fMaxTime = DefaultMaxTime;
		*g_FPSGlobal = 0;
	}
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
	_MESSAGE("%x", OriginalFunction);
	WriteRelCall((UINT32)0x86B3E3, (UInt32)&FPSHookHandler);
}

void DoPatches()
{
	if (g_bInlineStuff) HookInlines();
	//Fast Exit Hook
	if (g_bFastExit) WriteRelJump(0x86B66E, (UInt32)FastExit);
	if (g_bGTCFix) {
		timeBeginPeriod(1);
		SafeWrite32(0xFDF060, (UInt32)timeGetTime);
		if (g_bFPSFix) HookFPSStuff();
	}

}