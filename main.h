#pragma once
#include "hooks.h"

float* g_TimeGlobal = (float*)(0x11F6398);
static double LastTime;
static UINT32 OriginalFunction;



int g_bGTCFix = 0;
int g_bFastExit = 0;
int g_bInlineStuff = 0;
int g_bFPSFix = 0;

void TimeGlobalHook() {
	float* fMaxTime = (float*)0x1267B38;
	static bool init = false;
	if (!init) {
		init = true;
		LastTime = timeGetTime();
	}

	double getTime = timeGetTime();
	double Delta = (getTime - LastTime);
	*g_TimeGlobal = Delta;
	LastTime = getTime;
	*fMaxTime = Delta;
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
	OriginalFunction = ((*(UINT32*)(0x86B3E3 + 1)) + 5);
	WriteRelCall((UINT32)0x86B3E3, (UInt32)FPSHookHandler);
}

void DoPatches()
{
	if (g_bInlineStuff) HookInlines();
	//Fast Exit Hook
	if (g_bFastExit) WriteRelJump(0x86B66E, (UInt32)FastExit);
	if (g_bGTCFix) {
		timeBeginPeriod(1);
		ULONG Address = (DWORD)timeGetTime;
		SafeWrite32(0xFDF060, (UInt32)Address);
		if (g_bFPSFix) HookFPSStuff();
	}

}