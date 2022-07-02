#pragma once
#pragma once
#pragma comment(lib, "winmm.lib")
#include "SafeWrite.h"


void NopFunctionCall(UInt32 addr, UInt32 numArgs = 0)
{
	if (numArgs == 0)
	{
		// write nops instead of add esp, 0
		SafeWriteBuf(addr, "\x90\x90\x90\x90\x90", 5);
	}
	else
	{
		UInt32 oldProtect;
		VirtualProtect((void*)addr, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
		*(UInt16*)addr = 0xC483; // add esp, X
		*(UInt8*)(addr + 2) = numArgs * 4;
		*(UInt16*)(addr + 3) = 0x9090;
		VirtualProtect((void*)addr, 4, oldProtect, &oldProtect);
	}
}

void NopIndirectCall(UInt32 addr, UInt32 numArgs = 0)
{
	NopFunctionCall(addr, numArgs);
	SafeWrite8(addr + 5, 0x90);
}




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
		//6o
		SafeWrite32(0x00A2EFDF, 607);
		SafeWrite32(0x00A2EFED, 151);
		SafeWrite32(0x00A660B7, 641);
		SafeWrite32(0x00A660C4, 163);
		SafeWrite32(0x00B61841, 1213);
		SafeWrite32(0x00B61854, 307);
		SafeWrite32(0x00B7FF73, 991);
		SafeWrite32(0x00B7FF85, 251);
		SafeWrite32(0x00B9A5EB, 631);
		SafeWrite32(0x00B9A5FD, 157);
		
	}


}
