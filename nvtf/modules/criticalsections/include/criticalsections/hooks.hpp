#pragma once
#include <common/common.hpp>
#include <common/structs.hpp>
#include <nvse/prefix.h>
#include <Windows.h>

namespace tickfix {
	namespace criticalsections {
		namespace  hooks {
			//extern void PrintLog(const char* fmt, ...);
			void TweakRefCountSafeGuard(int mode);
			void TweakRendererLockSafeGuard(int mode);
			void TweakMiscCriticalSections();
			void TurnProblematicCSIntoBusyLocks();

		};

	};
};
