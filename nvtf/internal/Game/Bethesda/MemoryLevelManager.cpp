#include "MemoryLevelManager.hpp"

MemoryLevelManager* MemoryLevelManager::GetSingleton() {
	return reinterpret_cast<MemoryLevelManager*>(0x11DEFFC);
}

// GAME - 0x878080
void MemoryLevelManager::CheckMemoryLevel(bool abInIdle) {
	ThisCall(0x878080, this, abInIdle);
}

// GAME - 0x878160
void MemoryLevelManager::BeginMemoryFree(MEM_FREE_DATA& arData, bool abLockRenderer, bool a3, bool abLockFaceGen) {
	CdeclCall(0x878160, &arData, abLockRenderer, a3, abLockFaceGen);
}

// GAME - 0x878250
void MemoryLevelManager::FreeReleasedObjects(bool abLockedFaceGen) {
	CdeclCall(0x878250, abLockedFaceGen);
}

// GAME - 0x878200
void MemoryLevelManager::EndMemoryFree(MEM_FREE_DATA& arData) {
	CdeclCall(0x878200, &arData);
}
