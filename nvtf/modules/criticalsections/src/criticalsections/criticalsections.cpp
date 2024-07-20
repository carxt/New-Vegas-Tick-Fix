#include <criticalsections/criticalsections.hpp>

#include <criticalsections/settings.hpp>
#include <criticalsections/hooks.hpp>

namespace cs = tickfix::criticalsections;
bool cs::InitHooks() {
	settings::InitSettings();
	hooks::TweakRefCountSafeGuard(0);
	hooks::TweakMiscCriticalSections();
	hooks::TurnProblematicCSIntoBusyLocks();
	hooks::TweakRendererLockSafeGuard(0);
	return true;

	return false;
}