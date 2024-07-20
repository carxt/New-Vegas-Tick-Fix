#pragma once
#include <criticalsections/hooks.hpp>
#include <criticalsections/settings.hpp>
#include <nvse/SafeWrite.h>
#include <criticalsections/structs.hpp>



namespace cs = tickfix::criticalsections;
static void __fastcall EnterLightCriticalSectionWrapper(cs::structs::BGSLightCriticalSection* ecx);
static void __fastcall LeaveLightCriticalSectionWrapper(cs::structs::BGSLightCriticalSection* ecx);
static void asm__MMPauseBusySpinHook();
static void WINAPI hk_EnterCriticalSection(LPCRITICAL_SECTION cs);
static BOOL WINAPI hk_InitializeCriticalSectionhook(LPCRITICAL_SECTION cs);
static void WINAPI hk_EnterCriticalSectionRender(LPCRITICAL_SECTION cs);




void cs::hooks::TweakRefCountSafeGuard(int mode)
{
	//takes out Renderer+0x180 CS calls
	switch (mode) {
	case 1:
		SafeWrite16(0xE6DC4C, 0x90);
		WriteRelCall(0xE6DC4D, (uintptr_t)hk_EnterCriticalSectionRender);
		SafeWrite16(0xE90B46, 0x90);
		WriteRelCall(0xE90B47, (uintptr_t)hk_EnterCriticalSectionRender);
		SafeWrite16(0xE90C91, 0x90);
		WriteRelCall(0xE90C92, (uintptr_t)hk_EnterCriticalSectionRender);
		break;
	case 2:
		SafeWriteBuf(0xE6DC4B, "\x90\x90\x90\x90\x90\x90\x90", 7);
		SafeWriteBuf(0xE6DC69, "\x90\x90\x90\x90\x90\x90\x90", 7);

		SafeWriteBuf(0xE90B45, "\x90\x90\x90\x90\x90\x90\x90", 7);
		SafeWriteBuf(0xE90B79, "\x90\x90\x90\x90\x90\x90\x90", 7);
		SafeWriteBuf(0xE90BAA, "\x90\x90\x90\x90\x90\x90\x90", 7);

		SafeWriteBuf(0xE90C90, "\x90\x90\x90\x90\x90\x90\x90", 7);
		SafeWriteBuf(0xE90CBD, "\x90\x90\x90\x90\x90\x90\x90", 7);
		SafeWriteBuf(0xE90CFC, "\x90\x90\x90\x90\x90\x90\x90", 7);
		break;
	default:
		break;

	}

}



void cs::hooks::TurnProblematicCSIntoBusyLocks() {
	using namespace cs::structs;
	static BGSLightCriticalSection LipFileLCS = {};
	SafeWrite32(0x8A2252 + 1, (uintptr_t)&LipFileLCS);
	WriteRelCall(0x8A2257, (uintptr_t)EnterLightCriticalSectionWrapper);
	SafeWrite32(0x8A245F + 1, (uintptr_t)&LipFileLCS);
	WriteRelCall(0x8A2464, (uintptr_t)LeaveLightCriticalSectionWrapper);
	SafeWrite32(0x8A2CC9 + 1, (uintptr_t)&LipFileLCS);
	WriteRelCall(0x8A2CCE, (uintptr_t)LeaveLightCriticalSectionWrapper);


}

void cs::hooks::TweakRendererLockSafeGuard(int mode)
{
	switch (mode)
	{
	case 1:
		//Hopefully improves renderer perf
		//aaaaaaa
		SafeWrite16(0xE74126, 0xBE90);
		SafeWrite32(0xE74128, (uintptr_t)hk_EnterCriticalSectionRender);
		break;
	case 2:
		//takes out lock called by both Renderer+0x80 AND Renderer+0x100
		SafeWrite16(0xE7413E, 0x905F);
		SafeWrite16(0xE7414B, 0x9058);
		//exit
		SafeWrite16(0xE744A0, 0x905A);
		SafeWrite16(0xE744A3, 0x905F);
		break;

	default:
		break;

	}
}


void cs::hooks::TweakMiscCriticalSections()
{
	SafeWrite8(0x04538EB, 0x90);
	WriteRelCall(0x04538EC, (uintptr_t)hk_EnterCriticalSection);
	SafeWrite8(0xA5B571, 0x90);
	WriteRelCall(0xA5B572, (uintptr_t)hk_InitializeCriticalSectionhook);
	//SafeWrite16(0x040FC4C, 0x9090);
	WriteRelCall(0x040FC63, (uintptr_t)asm__MMPauseBusySpinHook);
	WriteRelCall(0x040FC57, (uintptr_t)asm__MMPauseBusySpinHook);


}






static void WINAPI hk_EnterCriticalSectionRender(LPCRITICAL_SECTION cs)
{
	constexpr unsigned int minSpinSleep = 0x900;
	constexpr unsigned int minSpinYield = 0x800;
	constexpr unsigned int spinAbort = 0x200;
	unsigned int spinCount = cs->SpinCount & 0xFFFFFF;
	if (spinCount > spinAbort) [[unlikely]] {

		return EnterCriticalSection(cs);
	}
	spinCount = 0xE00;
	unsigned int i = 0;
	while (i <= spinCount)
	{
		if (TryEnterCriticalSection(cs)) return;
		_mm_pause();
		if (i > minSpinYield) { if (i > minSpinSleep) { Sleep(0); } else { SwitchToThread(); } }
		i++;
	}
	return EnterCriticalSection(cs);
}



static __declspec (naked) void asm__MMPauseBusySpinHook()
{
	__asm {
		pause
		push dword ptr ss : [esp + 0x4]
		call Sleep
		ret
	}
}


static void __fastcall EnterLightCriticalSectionWrapper(cs::structs::BGSLightCriticalSection* ecx)
{
	((void(__thiscall*)(cs::structs::BGSLightCriticalSection*, uintptr_t))(0x40FBF0))(ecx, 0);
}

static void __fastcall LeaveLightCriticalSectionWrapper(cs::structs::BGSLightCriticalSection* ecx)
{
	((void(__thiscall*)(cs::structs::BGSLightCriticalSection*))(0x40FBA0))(ecx);
}


static void WINAPI hk_EnterCriticalSection(LPCRITICAL_SECTION cs)
{
	constexpr unsigned int minSpinThreshold = 80;
	constexpr unsigned int minSpinBusy = 750;
	unsigned int spinCount = cs->SpinCount & 0xFFFFFF;
	if (spinCount > minSpinThreshold) return EnterCriticalSection(cs);
	spinCount = 1500;
	unsigned int i = 0;
	while (i <= spinCount)
	{
		if (TryEnterCriticalSection(cs)) return;
		_mm_pause();
		if (i > minSpinBusy) { Sleep(0); }
		i++;
	}
	return EnterCriticalSection(cs);
}


static BOOL WINAPI hk_InitializeCriticalSectionhook(LPCRITICAL_SECTION cs)
{
	return InitializeCriticalSectionEx(cs, 2400, RTL_CRITICAL_SECTION_FLAG_NO_DEBUG_INFO);
	cs->SpinCount &= ~(RTL_CRITICAL_SECTION_ALL_FLAG_BITS) | RTL_CRITICAL_SECTION_FLAG_NO_DEBUG_INFO;

}
