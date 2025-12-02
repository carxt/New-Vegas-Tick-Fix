#include "internal/Game/Bethesda/BSCriticalSection.hpp"
#include "internal/Game/Bethesda/MemoryLevelManager.hpp"
#include "internal/Common/CommonUtils.hpp"
template <typename T>
extern void WriteRelJump(SIZE_T jumpSrxc, T jumpTgt);

namespace MiscHooks
{
	static bool __cdecl PurgeCellBuffers();
	namespace Setting {
		unsigned int iPCBMode = 0;
	};
	bool bDoPurgeCells = 0;
	static bool __cdecl hk_Script_PCB() 
	{
		if (Setting::iPCBMode < 2)
		{
			bDoPurgeCells = 1;
		}
		return true;

	}

	bool __cdecl PurgeCellBuffers()
	{
		if (!bDoPurgeCells) return true;
		MemoryLevelManager::MEM_FREE_DATA lFreeData;
		MemoryLevelManager::BeginMemoryFree(lFreeData, true, true, true);
		ThisCall<void>(0x04539A0, *(void**)0x11DEA10, 1, 0);
		MemoryLevelManager::FreeReleasedObjects(lFreeData.bLockedFaceGen);
		MemoryLevelManager::EndMemoryFree(lFreeData);
		bDoPurgeCells = 0;
		return true;
	}
		
	void ReadINI(const char* iniPath) {
		Setting::iPCBMode = GetPrivateProfileInt("Misc","iPCBMode", 0, iniPath);
	}

	void InitHooks() {
		if (Setting::iPCBMode) 
		{
			WriteRelJump(0x05B6CD0, hk_Script_PCB);
			CommonUtils::GetUtilsManager()->RegisterMainLoopFunction(PurgeCellBuffers);
		}
			
	}

};