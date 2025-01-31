#pragma once
extern void __stdcall SafeWrite8(UInt32 addr, UInt32 data);
extern void PrintLog(const char* fmt, ...);

struct BGSLightCriticalSection {
	uintptr_t OwningThreadId = 0;
	uintptr_t LockCount = 0;

};

void TweakRefCountSafeGuard(int mode);
void TweakRendererLockSafeGuard(int mode);
void TweakMiscCriticalSections();
void TurnProblematicCSIntoBusyLocks();