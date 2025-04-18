#include "ThreadingTweaks.hpp"

namespace ThreadingTweaks {

	namespace Setting {
		uint8_t ucReplaceTextureCreationLocks		= 0;
		uint8_t ucReplaceGeometryPrecacheLocks		= 0;
		bool	bTweakMiscCriticalSections			= false;
		bool	bAddPauseToSpinLocks				= false;
		bool	bReplaceDeadlockCSWithWaitAndSleep	= false;
	}

	struct BSSpinLock {
		uint32_t uiOwningThread = 0;
		uint32_t uiLockCount	= 0;
		uint32_t unk08[6]		= {};
	};

	void WINAPI EnterCriticalSectionRendererHook(LPCRITICAL_SECTION lpCriticalSection) {
		constexpr uint32_t uiMinSpinSleep	= 0x900;
		constexpr uint32_t uiMinSpinYield	= 0x800;
		constexpr uint32_t uiSpinAbort		= 0x200;
		uint32_t uiSpinCount = lpCriticalSection->SpinCount & 0xFFFFFF;
		if (uiSpinCount > uiSpinAbort) [[unlikely]]
			return EnterCriticalSection(lpCriticalSection);

		uiSpinCount = 0xE00;
		uint32_t i = 0;
		while (i <= uiSpinCount) {
			if (TryEnterCriticalSection(lpCriticalSection)) [[likely]]
				return;

			_mm_pause();
			if (i > uiMinSpinYield) [[unlikely]] {
				if (i > uiMinSpinSleep) [[unlikely]]
					Sleep(0); 
				else
					SwitchToThread(); 
			}
			i++;
		}
		return EnterCriticalSection(lpCriticalSection);
	}

	void WINAPI EnterCriticalSectionHook(LPCRITICAL_SECTION lpCriticalSection) {
		constexpr uint32_t uiMinSpinThreshold	= 80;
		constexpr uint32_t uiMinSpinBusy		= 750;
		uint32_t uiSpinCount = lpCriticalSection->SpinCount & 0xFFFFFF;
		if (uiSpinCount > uiMinSpinThreshold) [[unlikely]]
			return EnterCriticalSection(lpCriticalSection);

		uiSpinCount = 1500;
		uint32_t i = 0;
		while (i <= uiSpinCount) {
			if (TryEnterCriticalSection(lpCriticalSection)) [[likely]]
				return;
			
			_mm_pause();
			if (i > uiMinSpinBusy) [[unlikely]]
				Sleep(0);
			i++;
		}
		return EnterCriticalSection(lpCriticalSection);
	}

	BOOL WINAPI InitializeCriticalSectionHook(LPCRITICAL_SECTION lpCriticalSection) {
		return InitializeCriticalSectionEx(lpCriticalSection, 2400, RTL_CRITICAL_SECTION_FLAG_NO_DEBUG_INFO);
		lpCriticalSection->SpinCount &= ~(RTL_CRITICAL_SECTION_ALL_FLAG_BITS) | RTL_CRITICAL_SECTION_FLAG_NO_DEBUG_INFO;
	}

	void IntrinsicSleepHook(DWORD dwMilliseconds) {
		_mm_pause();
		Sleep(dwMilliseconds);
	}

	void ReplaceTextureCreationLocks() {
		switch (Setting::ucReplaceTextureCreationLocks) {
		case 1: [[likely]]
			// NiDX9Renderer::CreateSourceTextureRendererData
			SafeWrite16(0xE6DC4C, 0x90);
			WriteRelCall(0xE6DC4D, (uintptr_t)EnterCriticalSectionRendererHook);

			// NiDX9TextureManager::PrepareTextureForRendering
			SafeWrite16(0xE90B46, 0x90);
			WriteRelCall(0xE90B47, (uintptr_t)EnterCriticalSectionRendererHook);

			// NiDX9TextureManager::PrecacheTexture
			SafeWrite16(0xE90C91, 0x90);
			WriteRelCall(0xE90C92, (uintptr_t)EnterCriticalSectionRendererHook);
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

	void ReplaceGeometryPrecacheLocks() {
		// NiDX9Renderer::PerformPrecache
		if (Setting::ucReplaceGeometryPrecacheLocks) {
			SafeWrite16(0xE74126, 0xBE90);
			SafeWrite32(0xE74128, (uintptr_t)EnterCriticalSectionRendererHook);
		}
	}

	void TweakMiscCriticalSections() {
		// Replaces NiGlobalStringTable's critical section creation
		// Works only if JIP is not installed, as JIP replaces NiGlobalStringTable with its own implementation
		SafeWrite8(0xA5B571, 0x90);
		WriteRelCall(0xA5B572, InitializeCriticalSectionHook);

		// Replaces NiCriticalSection::Enter
		SafeWrite8(0x04538EB, 0x90);
		WriteRelCall(0x04538EC, EnterCriticalSectionHook);
	}

	void AddPauseToSpinLocks() {
		// BSSpinlock::Lock
		WriteRelCall(0x040FC63, IntrinsicSleepHook);
		WriteRelCall(0x040FC57, IntrinsicSleepHook);

		// Model::ModUseCount
		WriteRelCall(0x040FC57, IntrinsicSleepHook);

		// IOManager::WaitForTask
		WriteRelCall(0x5289CB, IntrinsicSleepHook);

		// IOManager::TryCancelTask
		WriteRelCall(0x528936, IntrinsicSleepHook);

		// BSTaskManager::CancelTask
		WriteRelCall(0x44AD0C, IntrinsicSleepHook);
		WriteRelCall(0x44AD23, IntrinsicSleepHook);
		WriteRelCall(0x44AD3A, IntrinsicSleepHook);

		// NavMeshObstacleManager::ForceUpdate
		WriteRelCall(0x6C39F6, IntrinsicSleepHook);
		WriteRelCall(0x6C3A83, IntrinsicSleepHook);
		WriteRelCall(0x6C3B13, IntrinsicSleepHook);

		// NavMeshObstacleManager::PushTask
		WriteRelCall(0x6C5DFD, IntrinsicSleepHook);
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

	void ReadINI(const char* iniPath) {
		Setting::ucReplaceTextureCreationLocks  = GetPrivateProfileInt("ThreadingTweaks", "iReplaceTextureCreationLocks", 1, iniPath);
		Setting::ucReplaceGeometryPrecacheLocks = GetPrivateProfileInt("ThreadingTweaks", "iReplaceGeometryPrecacheLocks", 1, iniPath);

		// Legacy support
		{
			uint32_t uiTweakRCSafeGuard			  = GetPrivateProfileInt("ThreadingTweaks", "iTweakRCSafeGuard", UINT32_MAX, iniPath);
			uint32_t uiTweakMiscRendererSafeGuard = GetPrivateProfileInt("ThreadingTweaks", "iTweakMiscRendererSafeGuard", UINT32_MAX, iniPath);
			if (uiTweakRCSafeGuard != UINT32_MAX)
				Setting::ucReplaceTextureCreationLocks = uiTweakRCSafeGuard;

			if (uiTweakMiscRendererSafeGuard != UINT32_MAX)
				Setting::ucReplaceGeometryPrecacheLocks = uiTweakMiscRendererSafeGuard;
		}

		Setting::bTweakMiscCriticalSections			= GetPrivateProfileInt("ThreadingTweaks", "bTweakMiscCriticalSections", 0, iniPath);
		Setting::bAddPauseToSpinLocks				= GetPrivateProfileInt("ThreadingTweaks", "bAddPauseToSpinLocks", 0, iniPath);
		Setting::bReplaceDeadlockCSWithWaitAndSleep = GetPrivateProfileInt("ThreadingTweaks", "bReplaceDeadlockCSWithWaitAndSleep", 0, iniPath);
	}

	void InitHooks() {
		if (Setting::bTweakMiscCriticalSections) [[likely]]
			TweakMiscCriticalSections();

		if (Setting::bReplaceDeadlockCSWithWaitAndSleep) [[likely]]
			TurnProblematicCSIntoBusyLocks();

		if (Setting::bAddPauseToSpinLocks) [[likely]]
			AddPauseToSpinLocks();

		ReplaceTextureCreationLocks();
		ReplaceGeometryPrecacheLocks();
	}
}