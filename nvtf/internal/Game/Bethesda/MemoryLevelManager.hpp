#pragma once

#include "BSCriticalSection.hpp"

class MemoryLevelManager {
public:
	struct MEM_FREE_DATA {
		bool		bOldInstantResourceRelease;
		bool		bOldInstantVertexDeclRelease;
		bool		bOldInstantTextureRelease;
		bool		bLockedRenderer;
		bool		bRunHeapClean;
		bool		bLockedFaceGen;
		int32_t		iTexturePurgeCount;
	};
	
	uint32_t			uiLowMemoryLevelLoading;
	uint32_t			uiLowMemoryLevelIdle;
	uint32_t			uiLowMemoryLevelIdleCritical;
	uint32_t			uiLowMemoryBufferSize;
	uint32_t			uiLastCheckedMemLevel;
	BSCriticalSection	kCheckLock;

	static MemoryLevelManager* GetSingleton();

	void CheckMemoryLevel(bool abInIdle);

	static void BeginMemoryFree(MEM_FREE_DATA& arData, bool abLockRenderer, bool a3, bool abLockFaceGen);
	static void FreeReleasedObjects(bool abLockedFaceGen);
	static void EndMemoryFree(MEM_FREE_DATA& arData);
};

ASSERT_SIZE(MemoryLevelManager, 0x2C);