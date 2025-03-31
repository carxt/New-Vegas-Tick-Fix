#include "FastExit.hpp"
#include "Game\Bethesda\StartMenu.hpp"

namespace FastExit {
	void FastExitHook() {
		StartMenu* pStartMenu = StartMenu::GetSingleton();
		if (pStartMenu && pStartMenu->GetSettingsChanged())
			pStartMenu->SaveSettings();

		TerminateProcess(GetCurrentProcess(), 0);
	}

	void ReadINI(const char* iniPath) {
	}

	void InitHooks() {
		WriteRelJump(0x86B66E, FastExitHook);
	}
}