#pragma once
#include "CriticalSections.h"
#include "nvse/SafeWrite.h"
ULONG g_iSpinCount;


__declspec (naked) void ReturnCSHook()
{
	static const char* const LABEL = "0x%x";
__asm 
{
	
	jmp Go
	push [esp]
	push LABEL
	call PrintLog
	add esp, 8
	Go:
	jmp EnterCriticalSection
}
}








void __stdcall func_InitCSHandler(LPCRITICAL_SECTION cs, DWORD Spin)

{
#ifdef _WIN32_WINNT < 0x0601
	InitializeCriticalSectionAndSpinCount(cs, Spin);
#else
	InitializeCriticalSectionEx(cs, Spin, CRITICAL_SECTION_NO_DEBUG_INFO);
#endif
	//cs->SpinCount |= RTL_CRITICAL_SECTION_FLAG_DYNAMIC_SPIN;
	//cs->SpinCount &= ~RTL_CRITICAL_SECTION_FLAG_DYNAMIC_SPIN;
}


void __stdcall NiObjectCSHandler(LPCRITICAL_SECTION cs1, LPCRITICAL_SECTION cs2, LPCRITICAL_SECTION cs3)
{

	InitializeCriticalSection(cs2);
#ifdef _WIN32_WINNT < 0x0601
	InitializeCriticalSection(cs1);
	InitializeCriticalSection(cs3);

#else
	InitializeCriticalSectionEx(cs1, 1200, CRITICAL_SECTION_NO_DEBUG_INFO)
	InitializeCriticalSectionEx(cs3, 1200, CRITICAL_SECTION_NO_DEBUG_INFO);
	cs1->SpinCount &= ~RTL_CRITICAL_SECTION_FLAG_DYNAMIC_SPIN;
	cs3->SpinCount &= ~RTL_CRITICAL_SECTION_FLAG_DYNAMIC_SPIN;
#endif


}


__declspec (naked)  DWORD NiObjectCriticalSections(DWORD* ecx)
{

	static const DWORD returnAddress = 0x0A62B29;
	__asm
	{
		mov[esi + 8], ebx
		mov[esi + 0x14], ebx
		lea eax, [esi + 0x80]
		push eax
		lea ecx, [esi + 0x100]
		push ecx
		lea edx, [esi + 0x180]
		push edx
		push returnAddress
		//call NiObjectCSHandler
		jmp NiObjectCSHandler
	}
}


__declspec (naked)  DWORD someOddCSCall(DWORD* ecx) //CALLED BEFORE
{

	static const DWORD returnAddress = 0x0FB334E;
	__asm
	{
		push 4000
		push 0x11F3330
		call func_InitCSHandler
		jmp returnAddress
	}
}


__declspec (naked)  DWORD someOddCSCall_2(DWORD* ecx)
{

	static const DWORD returnAddress = 0xA5B577;
	__asm
	{
		push 2000
		push eax
		call func_InitCSHandler
		jmp returnAddress
	}
}



void __stdcall InitCriticalSectionHook(LPCRITICAL_SECTION section)
{

	func_InitCSHandler(section, g_iSpinCount);
}


void __declspec (naked) MemHeapCSHook(DWORD* MemHeap)//CALLED BEFORE
{
	static const UInt32 RetAddr = 0x0AA8D62;
	__asm {
		push 4000
		push edx
		call func_InitCSHandler
		jmp RetAddr
	}


}


























void DoHeapCriticalSectionSpin()

{


	
	SetCriticalSectionSpinCount(&(HeapManager::GetSingleton()->memHeaps->defaultHeap->criticalSection), 4000);
	SetCriticalSectionSpinCount(&(HeapManager::GetSingleton()->memHeaps->fileHeap->criticalSection), 4000);
	//SetCriticalSectionSpinCount((LPCRITICAL_SECTION)0x11F3330, 6000);
	(HeapManager::GetSingleton()->memHeaps->defaultHeap->criticalSection).SpinCount &= ~RTL_CRITICAL_SECTION_FLAG_DYNAMIC_SPIN;
	(HeapManager::GetSingleton()->memHeaps->fileHeap->criticalSection).SpinCount &= ~RTL_CRITICAL_SECTION_FLAG_DYNAMIC_SPIN;
	((LPCRITICAL_SECTION)0x11F3330)->SpinCount &= ~RTL_CRITICAL_SECTION_FLAG_DYNAMIC_SPIN;

}


void WINAPI hk_EnterCriticalSectionRender(LPCRITICAL_SECTION cs)
{
	constexpr unsigned int minSpinSwitch = 0x80;
	constexpr unsigned int minSpinYield = 0x100;
	constexpr unsigned int spinAbort = 0x200;
	unsigned int spinCount = cs->SpinCount & 0xFFFFFF;
	if (spinCount > spinAbort) return EnterCriticalSection(cs);
	spinCount = 1200;
	unsigned int i = 0;
	while (i <= spinCount)
	{
		if (TryEnterCriticalSection(cs)) return;
		_mm_pause();
		if (i > minSpinYield) { Sleep(0); }
		else if (i > minSpinSwitch) { SwitchToThread(); }
		i++;
	}
	return EnterCriticalSection(cs);
}



void TweakRefCountSafeGuard(int mode)
{
	//takes out Renderer+0x180 CS calls
	switch (mode){
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

/*
__declspec(naked) void hk_call_E74247()
{
	__asm
	{
		push ecx
		mov eax, dword ptr ss : [esp + 8]
		push ebx
		mov ebx, dword ptr ss : [esp + 18]
		push esi
		push edi
		mov edi, dword ptr ss : [esp + 1C]
		push ebx
		lea edx, dword ptr ss : [esp + 10]
		push edx
		mov edx, dword ptr ss : [esp + 20]
		push edi
		mov esi, ecx
		mov ecx, dword ptr ds : [eax]
		push edx
		push eax
		mov eax, dword ptr ds : [ecx + 2C]
		mov dword ptr ss : [esp + 20] , 0
		call eax
		test eax, eax
		jge 0xE8C038
		pop edi
		pop esi
		xor eax, eax
		pop ebx
		pop ecx
		ret 10
	}

}
__declspec(naked) void hk_call_E74247()
{
	__asm
	{
		push ecx
		mov eax, dword ptr ss : [esp + 8]
		push ebx
		mov ebx, dword ptr ss : [esp + 18]
		push esi
		push edi
		mov edi, dword ptr ss : [esp + 1C]
		push ebx
		lea edx, dword ptr ss : [esp + 10]
		push edx
		mov edx, dword ptr ss : [esp + 20]
		push edi
		mov esi, ecx
		mov ecx, dword ptr ds : [eax]
		push edx
		push eax
		mov eax, dword ptr ds : [ecx + 2C]
		mov dword ptr ss : [esp + 20] , 0
		call eax
		test eax, eax
		jge 0xE8C038
		pop edi
		pop esi
		xor eax, eax
		pop ebx
		pop ecx
		ret 10
	}

}*/

void TweakRendererLockSafeGuard()
{
	//Hopefully improves renderer perf
	//aaaaaaa
	SafeWrite16(0xE74126, 0xBE90);
	SafeWrite32(0xE74128, (uintptr_t)hk_EnterCriticalSectionRender);


}

void WINAPI hk_EnterCriticalSection_OLD(LPCRITICAL_SECTION cs)
{
	unsigned int spinCount = cs->SpinCount & 0xFFFFFF;
	if (spinCount > 100) return EnterCriticalSection(cs);
	spinCount = 800;
	unsigned int i = 0;
	while (i <= 100)
	{
		if (TryEnterCriticalSection(cs)) return;
		i++;
	}
	while (i <= spinCount)
	{
		if (TryEnterCriticalSection(cs)) return;
		Sleep(0);
		i++;
	}
	i = 0;
	while (i <= spinCount)
	{
		if (TryEnterCriticalSection(cs)) return;
		Sleep(1);
		i++;
	}
	return EnterCriticalSection(cs);
}


void WINAPI hk_EnterCriticalSection(LPCRITICAL_SECTION cs)
{
	constexpr unsigned int minSpinBusy = 0x80;
	unsigned int spinCount = cs->SpinCount & 0xFFFFFF;
	if (spinCount > minSpinBusy) return EnterCriticalSection(cs);
	spinCount = 1500;
	unsigned int i = 0;
	while (i <= spinCount)
	{
		if (TryEnterCriticalSection(cs)) return;
		_mm_pause();
		if (i > minSpinBusy) { Sleep(0);}
		i++;
	}
	return EnterCriticalSection(cs);
}


BOOL WINAPI hk_InitializeCriticalSectionhook(LPCRITICAL_SECTION cs)
{
	return InitializeCriticalSectionEx(cs, 2400, RTL_CRITICAL_SECTION_FLAG_NO_DEBUG_INFO);
	cs->SpinCount &= ~(RTL_CRITICAL_SECTION_ALL_FLAG_BITS) | RTL_CRITICAL_SECTION_FLAG_NO_DEBUG_INFO;

}





__declspec (naked) void asm_EnterLCSHook() {
	__asm {
		cmp ecx, 0x80
		pause
		jbe doneLbl
		push 0
		call Sleep
		doneLbl:
		ret
	}
}
void TweakMiscCriticalSections()
{
	SafeWrite8(0x04538EB, 0x90);
	WriteRelCall(0x04538EC, (uintptr_t)hk_EnterCriticalSection);
	SafeWrite8(0xA5B571, 0x90);
	WriteRelCall(0xA5B572, (uintptr_t)hk_InitializeCriticalSectionhook);
	//SafeWrite16(0x040FC4C, 0x9090);
	WriteRelCall(0x040FC63, (uintptr_t)asm_EnterLCSHook);

}





void __fastcall EnterLightCriticalSectionWrapper(BGSLightCriticalSection* ecx)
{
	((void(__thiscall*)(BGSLightCriticalSection*, uintptr_t))(0x40FBF0))(ecx, 0);
}

void __fastcall LeaveLightCriticalSectionWrapper(BGSLightCriticalSection* ecx)
{
	((void(__thiscall*)(BGSLightCriticalSection*))(0x40FBA0))(ecx);
}


void TurnProblematicCSIntoBusyLocks() {
	static BGSLightCriticalSection LipFileLCS = {};
	SafeWrite32(0x8A2252 + 1, (uintptr_t)&LipFileLCS);
	WriteRelCall(0x8A2257, (uintptr_t)EnterLightCriticalSectionWrapper);
	SafeWrite32(0x8A245F + 1, (uintptr_t)&LipFileLCS);
	WriteRelCall(0x8A2464, (uintptr_t)LeaveLightCriticalSectionWrapper);
	SafeWrite32(0x8A2CC9 + 1, (uintptr_t)&LipFileLCS);
	WriteRelCall(0x8A2CCE, (uintptr_t)LeaveLightCriticalSectionWrapper);

	
}


