#include "CriticalSections.h"
#include "nvse/SafeWrite.h"

void WINAPI hk_EnterCriticalSectionRender(LPCRITICAL_SECTION cs)
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

void ReplaceTextureCreationLocks(int mode)
{
	switch (mode){
		case 1:
			// NiDX9Renderer::CreateSourceTextureRendererData
			SafeWrite16(0xE6DC4C, 0x90);
			WriteRelCall(0xE6DC4D, (uintptr_t)hk_EnterCriticalSectionRender);

			// NiDX9TextureManager::PrepareTextureForRendering
			SafeWrite16(0xE90B46, 0x90);
			WriteRelCall(0xE90B47, (uintptr_t)hk_EnterCriticalSectionRender);

			// NiDX9TextureManager::PrecacheTexture
			SafeWrite16(0xE90C91, 0x90);
			WriteRelCall(0xE90C92, (uintptr_t)hk_EnterCriticalSectionRender);
			break;
		case 2:
			// NiDX9Renderer::CreateSourceTextureRendererData
			SafeWriteBuf(0xE6DC4B, "\x90\x90\x90\x90\x90\x90\x90", 7);
			SafeWriteBuf(0xE6DC69, "\x90\x90\x90\x90\x90\x90\x90", 7);

			// NiDX9TextureManager::PrepareTextureForRendering
			SafeWriteBuf(0xE90B45, "\x90\x90\x90\x90\x90\x90\x90", 7);
			SafeWriteBuf(0xE90B79, "\x90\x90\x90\x90\x90\x90\x90", 7);
			SafeWriteBuf(0xE90BAA, "\x90\x90\x90\x90\x90\x90\x90", 7);

			// NiDX9TextureManager::PrecacheTexture
			SafeWriteBuf(0xE90C90, "\x90\x90\x90\x90\x90\x90\x90", 7);
			SafeWriteBuf(0xE90CBD, "\x90\x90\x90\x90\x90\x90\x90", 7);
			SafeWriteBuf(0xE90CFC, "\x90\x90\x90\x90\x90\x90\x90", 7);
			break;
		default:
			break;
	
	}
}

void ReplaceGeometryPrecacheLocks(int mode)
{
	// NiDX9Renderer::PerformPrecache
	switch (mode)
	{
		case 1: 

			SafeWrite16(0xE74126, 0xBE90);
			SafeWrite32(0xE74128, (uintptr_t)hk_EnterCriticalSectionRender);
			break;
		case 2: 
			// Removes renderer and precache critical sections
			// Enter
			SafeWrite16(0xE7413E, 0x905F);
			SafeWrite16(0xE7414B, 0x9058);
			// Leave
			SafeWrite16(0xE744A0, 0x905A);
			SafeWrite16(0xE744A3, 0x905F);
			break;
		default: 
			break;
	}
}

void WINAPI hk_EnterCriticalSection(LPCRITICAL_SECTION cs)
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

void IntrinsicSleepHook(DWORD dwMilliseconds) {
	_mm_pause();
	Sleep(dwMilliseconds);
}

void TweakMiscCriticalSections()
{
	// Replaces NiGlobalStringTable's critical section creation
	// Works only if JIP is not installed, as JIP replaces NiGlobalStringTable with its own implementation
	SafeWrite8(0xA5B571, 0x90);
	WriteRelCall(0xA5B572, (uintptr_t)hk_InitializeCriticalSectionhook);

	// Replaces NiCriticalSection::Enter
	SafeWrite8(0x04538EB, 0x90);
	WriteRelCall(0x04538EC, (uintptr_t)hk_EnterCriticalSection);

	// BSSpinlock::Lock
	WriteRelCall(0x040FC63, (uintptr_t)IntrinsicSleepHook);
	WriteRelCall(0x040FC57, (uintptr_t)IntrinsicSleepHook);

	// Model::ModUseCount
	WriteRelCall(0x040FC57, (uintptr_t)IntrinsicSleepHook);

	// IOManager::WaitForTask
	WriteRelCall(0x5289CB, (uintptr_t)IntrinsicSleepHook);

	// IOManager::TryCancelTask
	WriteRelCall(0x528936, (uintptr_t)IntrinsicSleepHook);

	// BSTaskManager::CancelTask
	WriteRelCall(0x44AD0C, (uintptr_t)IntrinsicSleepHook);
	WriteRelCall(0x44AD23, (uintptr_t)IntrinsicSleepHook);
	WriteRelCall(0x44AD3A, (uintptr_t)IntrinsicSleepHook);

	// NavMeshObstacleManager::ForceUpdate
	WriteRelCall(0x6C39F6, (uintptr_t)IntrinsicSleepHook);
	WriteRelCall(0x6C3A83, (uintptr_t)IntrinsicSleepHook);
	WriteRelCall(0x6C3B13, (uintptr_t)IntrinsicSleepHook);

	// NavMeshObstacleManager::PushTask
	WriteRelCall(0x6C5DFD, (uintptr_t)IntrinsicSleepHook);
}

void TurnProblematicCSIntoBusyLocks() {
	static BSSpinLock LipFileLCS = {};

	// Hooks in Actor::SpeakSoundFunction, replaces critical section with a spin lock
	SafeWrite32(0x8A2252 + 1, (uintptr_t)&LipFileLCS);
	WriteRelCall(0x8A2257, 0x40FBF0);
	SafeWrite32(0x8A245F + 1, (uintptr_t)&LipFileLCS);
	WriteRelCall(0x8A2464, 0x40FBA0);
	SafeWrite32(0x8A2CC9 + 1, (uintptr_t)&LipFileLCS);
	WriteRelCall(0x8A2CCE, 0x40FBA0);
}