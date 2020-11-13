#pragma once
extern ULONG g_iSpinCount;
extern void __stdcall SafeWrite8(UInt32 addr, UInt32 data);

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
void __stdcall InitCriticalSectionHook(LPCRITICAL_SECTION section);
void __stdcall func_InitCSHandler(LPCRITICAL_SECTION cs, DWORD Spin);
void __stdcall NiObjectCSHandler(LPCRITICAL_SECTION cs1, LPCRITICAL_SECTION cs2, LPCRITICAL_SECTION cs3);
DWORD NiObjectCriticalSections(DWORD* ecx);
DWORD someOddCSCall(DWORD* ecx);
DWORD someOddCSCall_2(DWORD* ecx);
void MemHeapCSHook(DWORD* MemHeap);
void ReturnCSHook();






















void DoHeapCriticalSectionSpin();
void RemoveRefCountSafeGuard();
void RemoveRendererLockSafeGuard();
void TweakMiscCriticalSections();