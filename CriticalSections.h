#pragma once
ULONG g_iSpinCount = 0;
#ifdef _WIN32_WINNT
#undef _WIN32_WINNT
#define _WIN32_WINNT 0x601
#endif


//this shit is for XP, but that's old af already
/*
__declspec (naked)  DWORD NiObjectCriticalSections(DWORD* ecx)
{

	static const DWORD returnAddress = 0x0A62B29;
	__asm
	{
		mov [esi+8], ebx
		mov [esi+0x14], ebx
		lea eax, [esi+0x80]
		push 1000
		push eax
		call InitializeCriticalSectionAndSpinCount
		lea ecx, [esi + 0x100]
		push 1000
		push ecx
		call InitializeCriticalSectionAndSpinCount
		lea edx, [esi + 0x180]
		push 1000
		push edx
		call InitializeCriticalSectionAndSpinCount
		jmp returnAddress
	}
}


__declspec (naked)  DWORD someOddCSCall(DWORD* ecx)
{

	static const DWORD returnAddress = 0x0FB334E;
	__asm
	{
		push 6000
		push 0x11F3330
		call InitializeCriticalSectionAndSpinCount
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
		call InitializeCriticalSectionAndSpinCount
		jmp returnAddress
	}
}



void __stdcall InitCriticalSectionHook(LPCRITICAL_SECTION section)
{

	InitializeCriticalSectionAndSpinCount(section, g_iSpinCount);
}


void __declspec (naked) MemHeapCSHook(DWORD* MemHeap)
{
	static const UInt32 RetAddr = 0x0AA8D62;
	__asm{
		push 4000
		push edx
		call InitializeCriticalSectionAndSpinCount
		jmp RetAddr
	}


}




*/

void __stdcall func_InitCSHandler(LPCRITICAL_SECTION cs, DWORD Spin)

{
	InitializeCriticalSectionEx(cs, Spin, CRITICAL_SECTION_NO_DEBUG_INFO);

}





__declspec (naked)  DWORD NiObjectCriticalSections(DWORD* ecx)
{

	static const DWORD returnAddress = 0x0A62B29;
	__asm
	{
		mov[esi + 8], ebx
		mov[esi + 0x14], ebx
		lea eax, [esi + 0x80]
		push 1000
		push eax
		call func_InitCSHandler
		lea ecx, [esi + 0x100]
		push 1000
		push ecx
		call func_InitCSHandler
		lea edx, [esi + 0x180]
		push 1000
		push edx
		call func_InitCSHandler
		jmp returnAddress
	}
}


__declspec (naked)  DWORD someOddCSCall(DWORD* ecx)
{

	static const DWORD returnAddress = 0x0FB334E;
	__asm
	{
		push 6000
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


void __declspec (naked) MemHeapCSHook(DWORD* MemHeap)
{
	static const UInt32 RetAddr = 0x0AA8D62;
	__asm {
		push 4000
		push edx
		call func_InitCSHandler
		jmp RetAddr
	}


}



