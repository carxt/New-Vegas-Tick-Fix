#include "SafeWrite.hpp"
#include <memoryapi.h>

void SafeWrite8(SIZE_T addr, SIZE_T data)
{
	SIZE_T	oldProtect;

	VirtualProtect((void *)addr, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
	*((uint8_t *)addr) = data;
	VirtualProtect((void *)addr, 4, oldProtect, &oldProtect);
}

void SafeWrite16(SIZE_T addr, SIZE_T data)
{
	SIZE_T	oldProtect;

	VirtualProtect((void *)addr, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
	*((uint16_t *)addr) = data;
	VirtualProtect((void *)addr, 4, oldProtect, &oldProtect);
}

void SafeWrite32(SIZE_T addr, SIZE_T data)
{
	SIZE_T	oldProtect;

	VirtualProtect((void *)addr, 4, PAGE_EXECUTE_READWRITE, &oldProtect);
	*((SIZE_T *)addr) = data;
	VirtualProtect((void *)addr, 4, oldProtect, &oldProtect);
}

void SafeWriteBuf(SIZE_T addr, const void * data, SIZE_T len)
{
	SIZE_T	oldProtect;

	VirtualProtect((void *)addr, len, PAGE_EXECUTE_READWRITE, &oldProtect);
	memcpy((void *)addr, data, len);
	VirtualProtect((void *)addr, len, oldProtect, &oldProtect);
}

void WriteRelJump(SIZE_T jumpSrc, SIZE_T jumpTgt)
{
	// jmp rel32
	SafeWrite8(jumpSrc, 0xE9);
	SafeWrite32(jumpSrc + 1, jumpTgt - jumpSrc - 1 - 4);
}

void WriteRelCall(SIZE_T jumpSrc, SIZE_T jumpTgt)
{
	// call rel32
	SafeWrite8(jumpSrc, 0xE8);
	SafeWrite32(jumpSrc + 1, jumpTgt - jumpSrc - 1 - 4);
}

void ReplaceCall(SIZE_T jumpSrc, SIZE_T jumpTgt)
{
	SafeWrite32(jumpSrc + 1, jumpTgt - jumpSrc - 1 - 4);
}

void ReplaceVirtualFunc(SIZE_T jumpSrc, void* jumpTgt) {
	SafeWrite32(jumpSrc, (SIZE_T)jumpTgt);
}

void WriteRelJnz(SIZE_T jumpSrc, SIZE_T jumpTgt)
{
	// jnz rel32
	SafeWrite16(jumpSrc, 0x850F);
	SafeWrite32(jumpSrc + 2, jumpTgt - jumpSrc - 2 - 4);
}

void WriteRelJle(SIZE_T jumpSrc, SIZE_T jumpTgt)
{
	// jle rel32
	SafeWrite16(jumpSrc, 0x8E0F);
	SafeWrite32(jumpSrc + 2, jumpTgt - jumpSrc - 2 - 4);
}