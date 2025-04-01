#pragma once

class BSTimer {
public:
	bool		bDisableCounter;
	float		fClamp;
	float		fClampRemainder;
	float		fDelta;
	float		fRealTimeDelta;
	uint32_t	uiMsPassed;
	uint32_t	uiUnk18;
	bool		bIsChangeTimeMultSlowly;
};

ASSERT_SIZE(BSTimer, 0x20);