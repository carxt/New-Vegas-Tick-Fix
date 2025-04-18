#pragma once

class BSTimer {
public:
	uint8_t		ucDisableCounter;
	float		fClamp;
	float		fClampRemainder;
	float		fDelta;
	float		fRealTimeDelta;
	uint32_t	uiLastTime;
	uint32_t	uiFirstTime;
	bool		bIsChangeTimeMultSlowly;
	bool		bSameFrameDelta;

	void Update(uint32_t auiTime);
};

ASSERT_SIZE(BSTimer, 0x20);