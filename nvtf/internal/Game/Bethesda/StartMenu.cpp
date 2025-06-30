#include "StartMenu.hpp"

StartMenu* StartMenu::GetSingleton() {
	return *reinterpret_cast<StartMenu**>(0x11DAAC0);
}

bool StartMenu::GetSettingsChanged() const {
	return uiFlags & 2;
}

void StartMenu::SaveSettings() {
	CdeclCall(0x7D6D70);
}
