#pragma once
#include "SafeWrite.h"

void DoHashTableStuff()
{
	if (g_bResizeHashtables) {
		SafeWrite32(0x00473F69, 5009);
		SafeWrite32(0x00583FF6, 1709);
		SafeWrite32(0x006B5C76, 10037);
		SafeWrite32(0x006B7A30, 2819);
		SafeWrite32(0x00845558, 7057);
		SafeWrite32(0x00846FFB, 12049);
		//SafeWrite32(0x00846FFB, 5039);

		SafeWrite32(0x00848072, 12049);

		SafeWrite8(0x00544FA7, 41);
		SafeWrite8(0x00544FC9, 29);
		SafeWrite8(0x00582CA2, 127);
		SafeWrite8(0x00582CEF, 53);
		SafeWrite8(0x00582D64, 31);
		SafeWrite8(0x00587AC9, 43);
		SafeWrite8(0x006C02F8, 127);
		SafeWrite8(0x006C035F, 97);
		SafeWrite8(0x006C0397, 97);
		SafeWrite8(0x006C03AB, 89);
		SafeWrite8(0x006E13AF, 53);
		SafeWrite8(0x0084703E, 59);
		SafeWrite8(0x008470FA, 59);
		SafeWrite8(0x0084AB60, 127);
		SafeWrite8(0x00AD9169, 113);
		SafeWrite8(0x00AD9189, 113);
		SafeWrite8(0x00AD91A9, 113);
		SafeWrite8(0x00AD91CC, 41);
	}
}