#pragma once
ULONG g_iSpinCount = 0;



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


__declspec (naked) void ReturnCSHook()
{
	static const char* const LABEL = "0x%x";
__asm 
{
	
	jmp Go
	push [esp]
	push LABEL
	call _MESSAGE
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























struct MemoryHeap
{
	void* vtable; // AbstractHeap
	UInt32 unk04[0x835];
	CRITICAL_SECTION criticalSection;
};

struct Heaps
{
	MemoryHeap* defaultHeap;
	MemoryHeap* staticHeap;
	MemoryHeap* fileHeap;
};

struct HeapManager
{
	UInt16 pad00;
	UInt16 unk02;
	UInt32 unk04; // 04
	Heaps* memHeaps; // 08
	static HeapManager* GetSingleton() { return (HeapManager*)0x11F6238; }
};



void DoHeapCriticalSectionSpin()

{


	
	SetCriticalSectionSpinCount(&(HeapManager::GetSingleton()->memHeaps->defaultHeap->criticalSection), 4000);
	SetCriticalSectionSpinCount(&(HeapManager::GetSingleton()->memHeaps->fileHeap->criticalSection), 4000);
	//SetCriticalSectionSpinCount((LPCRITICAL_SECTION)0x11F3330, 6000);
	(HeapManager::GetSingleton()->memHeaps->defaultHeap->criticalSection).SpinCount &= ~RTL_CRITICAL_SECTION_FLAG_DYNAMIC_SPIN;
	(HeapManager::GetSingleton()->memHeaps->fileHeap->criticalSection).SpinCount &= ~RTL_CRITICAL_SECTION_FLAG_DYNAMIC_SPIN;
	((LPCRITICAL_SECTION)0x11F3330)->SpinCount &= ~RTL_CRITICAL_SECTION_FLAG_DYNAMIC_SPIN;

}


void RemoveRefCountSafeGuard()
{

	//takes out Renderer+0x180 CS calls

	SafeWriteBuf(0xE6DC4B, "\x90\x90\x90\x90\x90\x90\x90", 7);
	SafeWriteBuf(0xE6DC69, "\x90\x90\x90\x90\x90\x90\x90", 7);

	SafeWriteBuf(0xE90B45, "\x90\x90\x90\x90\x90\x90\x90", 7);
	SafeWriteBuf(0xE90B79, "\x90\x90\x90\x90\x90\x90\x90", 7);
	SafeWriteBuf(0xE90BAA, "\x90\x90\x90\x90\x90\x90\x90", 7);


	SafeWriteBuf(0xE90C90, "\x90\x90\x90\x90\x90\x90\x90", 7);
	SafeWriteBuf(0xE90CBD, "\x90\x90\x90\x90\x90\x90\x90", 7);
	SafeWriteBuf(0xE90CFC, "\x90\x90\x90\x90\x90\x90\x90", 7);


}

void Remove0x80SafeGuard()
{
	//takes out lock called by both Renderer+0x80 AND Renderer+0x100
	SafeWrite16(0xE73FF3, 0x9058);
	SafeWrite16(0xE74000, 0x9058);
	SafeWriteBuf(0xE74106, "\x90\x90\x90\x90\x90\x90\x90\x90\x90\x90", 10);


}