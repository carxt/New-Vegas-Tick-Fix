#pragma once

class BGSSaveLoadGame {
public:
	char		padding[0x244];
	uint32_t	uiGlobalFlags;
	uint8_t		ucCurrentMinorVersion;

	static BGSSaveLoadGame* GetSingleton();

	bool IsLoading() const;
};

ASSERT_SIZE(BGSSaveLoadGame, 0x24C);