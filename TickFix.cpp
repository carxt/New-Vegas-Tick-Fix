#include <future>
#include "stdafx.h"
#include "dinput8.h"

float* g_TimeGlobal = (float*)(0x11F6398);
static double LastTime;
static UINT32 OriginalFunction;

void TimeGlobalHook() {
	float* fMaxTime = (float*) 0x1267B38;
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

void __declspec(naked) FPSHookHandler()
{

	__asm {
		push ecx
		call TimeGlobalHook
		pop ecx
		jmp OriginalFunction
	}

}

void MainHook() {
	BYTE MAINCALL = 0xE8;
	OriginalFunction = ((*(UINT32*)(0x86B3E3 + 1)) + 5);
	DetourJump((UINT32)0x86B3E3, &FPSHookHandler);
	PatchMemory(0x86B3E3, (PBYTE)&MAINCALL, 1);
}

void DoPatches()
{

	timeBeginPeriod(1);
	ULONG Address = (DWORD)timeGetTime;
	PatchMemory(0xFDF060, (PBYTE)&Address, 4);

}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD fdwReason, LPVOID lpReserved)
{
	if (fdwReason == DLL_PROCESS_ATTACH)
	{
		DisableThreadLibraryCalls(hModule);

		DoPatches();
	}

	return TRUE;
}