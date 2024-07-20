#include <utility>
#include <common/settings.hpp>


namespace tickfix {
	namespace criticalsections {
		namespace settings {
			static const char* sectionName = "ThreadingTweaks";
			static auto iTweakRefCountCS = common::settings::SettingContainer(0, sectionName, "iTweakRCSafeGuard");
			static auto iTweakPrecacheCS = common::settings::SettingContainer(0, sectionName, "iTweakMiscRendererSafeGuard");
			static auto bTweakMiscCS = common::settings::SettingContainer(false, sectionName, "bTweakMiscCriticalSections");
			static auto bReplaceDeadlockCSForUserMutex = common::settings::SettingContainer(false, sectionName, "bReplaceDeadlockCSWithWaitAndSleep");
			void InitSettings();
		};
	};
};