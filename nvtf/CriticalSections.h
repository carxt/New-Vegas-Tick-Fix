#pragma once

struct BSSpinLock {
	UInt32 uiOwningThread = 0;
	UInt32 uiLockCount = 0;
	UInt32 unk08[6] = {};
};

void ReplaceTextureCreationLocks(int mode);
void ReplaceGeometryPrecacheLocks(int mode);
void TweakMiscCriticalSections();
void TurnProblematicCSIntoBusyLocks();