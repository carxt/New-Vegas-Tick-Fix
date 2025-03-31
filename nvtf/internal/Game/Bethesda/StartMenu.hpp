#pragma once

class StartMenu {
public:
	char		padding[0x1A8];
	uint32_t	uiFlags;

	static StartMenu* GetSingleton();

	bool GetSettingsChanged() const;

	static void SaveSettings();
};

ASSERT_OFFSET(StartMenu, uiFlags, 0x1A8); // 0x1A8