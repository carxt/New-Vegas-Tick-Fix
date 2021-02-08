#pragma once
#pragma once
#pragma comment(lib, "winmm.lib")
#include "SafeWrite.h"

UInt32 ChangeTileLock1();
void __stdcall ChangeTileLock2(DWORD* toExchange);
UInt32 __fastcall FNV1aNumberHasher(NiTPointerMap<void*>* thisMap, void* edx, DWORD key);
//UInt32 __fastcall FNV1aStringHasher(NiTPointerMap<void*>* thisMap, void* edx, char* key);
UInt32 __fastcall FNV1aStringHasherCaseSensitive(NiTPointerMap<void*>* thisMap, void* edx, char* str);
UInt32 __fastcall FNV1aStringHasherCaseInsensitive(NiTPointerMap<void*>* thisMap, void* edx, char* str);

UInt32 __fastcall MurmurHash2(NiTPointerMap<void*>* thisMap, void* edx, DWORD k);
UInt32 __fastcall OriginalHash(NiTPointerMap<void*>* thisMap, void* edx, DWORD key);

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



NiTPointerMap<int>* g_EarlyMap1 = (NiTPointerMap<int>*) 0x011F3308;
NiTPointerMap<void*>* g_TileMap = (NiTPointerMap<void*>*) 0x11F3358;

void DoHashTableStuff()
{
	if (g_bResizeHashtables) {
		SafeWrite32(0x00473F69, 5005);
		SafeWrite32(0x00583FF6, 1703);
		SafeWrite32(0x006B5C76, 10009);
		SafeWrite32(0x006B7A30, 2809);
		SafeWrite32(0x00845558, 7049);
		SafeWrite32(0x00846FFB, 12041);
		//SafeWrite32(0x00846FFB, 5031);

		SafeWrite32(0x00848072, 12041);

		SafeWrite8(0x00544FA7, 39);
		SafeWrite8(0x00544FC9, 29);
		SafeWrite8(0x00582CA2, 119);
		SafeWrite8(0x00582CEF, 49);
		SafeWrite8(0x00582D64, 31);
		SafeWrite8(0x00587AC9, 43);
		SafeWrite8(0x006C02F8, 121);
		SafeWrite8(0x006C035F, 95);
		SafeWrite8(0x006C0397, 97);
		SafeWrite8(0x006C03AB, 89);
		SafeWrite8(0x006E13AF, 53);
		SafeWrite8(0x0084703E, 57);
		SafeWrite8(0x008470FA, 55);
		SafeWrite8(0x0084AB60, 117);
		SafeWrite8(0x00AD9169, 111);
		SafeWrite8(0x00AD9189, 111);
		SafeWrite8(0x00AD91A9, 111);
		SafeWrite8(0x00AD91CC, 39);
		//6o
		SafeWrite32(0x00A2EFDF, 604);
		SafeWrite32(0x00A2EFED, 151);
		SafeWrite32(0x00A660B7, 636);
		SafeWrite32(0x00A660C4, 159);
		SafeWrite32(0x00B61841, 1204);
		SafeWrite32(0x00B61854, 301);
		SafeWrite32(0x00B7FF73, 988);
		SafeWrite32(0x00B7FF85, 247);
		SafeWrite32(0x00B9A5EB, 628);
		SafeWrite32(0x00B9A5FD, 157);
		if (false)
		{
			if (g_TileMap->m_numItems == 0)
			{
				delete g_TileMap;
				ThisStdCall(0xA0D8E0, g_TileMap, 8701);
				*(UInt32*)g_TileMap = 0x1094E7C;
			}

			if (g_EarlyMap1->m_numItems == 0)
			{
				delete g_EarlyMap1;
				ThisStdCall(0xA0D710, g_EarlyMap1, (4 * 371));
				*(UInt32*)g_EarlyMap1 = 0x1094E3C;
			}
		}
	}


}
std::hash <DWORD> IntegerHasher;
std::hash <std::string> StringHasher;


UInt32 __fastcall stdNumberHasher(NiTPointerMap<void*>* thisMap, void* edx, DWORD key)
{
	return IntegerHasher(key) % thisMap->m_numBuckets;
}

UInt32 __fastcall FNV1aNumberHasher(NiTPointerMap<void*>* thisMap, void* edx, DWORD key)
{
	const unsigned int fnv_prime = 16777619;
	unsigned int hash = 0x811c9dc5;
	BYTE* keyPTR = (BYTE*)&key;
	hash ^= (keyPTR[0]);
	hash *= fnv_prime;

	hash ^= (keyPTR[1]);
	hash *= fnv_prime;

	hash ^= (keyPTR[2]);
	hash *= fnv_prime;

	hash ^= (keyPTR[3]);
	hash *= fnv_prime;


	return hash % thisMap->m_numBuckets;
}




int (__cdecl* GameToLower)(int) = (int(__cdecl *)(int)) 0x0EC67AA;
UInt32 __fastcall FNV1aStringHasherCaseInsensitive(NiTPointerMap<void*>* thisMap, void* edx, char* str)
{
	const unsigned int fnv_prime = 16777619;
	unsigned int hash = 0x811c9dc5;
	while (*str)
	{
		hash ^= (char) GameToLower(*str);
		hash *= fnv_prime;
		str++;
	}

	return hash % thisMap->m_numBuckets;
}


UInt32 __fastcall FNV1aStringHasherCaseSensitive(NiTPointerMap<void*>* thisMap, void* edx, char* str)
{
	const unsigned int fnv_prime = 16777619;
	unsigned int hash = 0x811c9dc5;
	while (*str)
	{
		hash ^= (*str);
		hash *= fnv_prime;
		str++;
	}

	return hash % thisMap->m_numBuckets;
}


UInt32 __fastcall OriginalHash(NiTPointerMap<void*>* thisMap, void* edx, DWORD key)
{
	return key % thisMap->m_numBuckets;
}

/*UInt32 __fastcall MurmurHash2(NiTPointerMap<void*>* thisMap, void* edx, DWORD k)
{
	int len = 4;
	// 'm' and 'r' are mixing constants generated offline.
	// They're not really 'magic', they just happen to work well.
	const UInt32 m = 0x5bd1e995;
	const int r = 24;
	// Initialize the hash to a 'random' value

	UInt32 h = 0x40 ^ len;
	// Mix 4 bytes at a time into the hash

	k *= m;
	k ^= k >> r;
	k *= m;

	h *= m;
	h ^= k;

	// Do a few final mixes of the hash to ensure the last few
	// bytes are well-incorporated.
	h ^= h >> 13;
	h *= m;
	h ^= h >> 15;
	return h % thisMap->m_numBuckets;
}*/

UInt32 ChangeTileLock1()
{
	static volatile UInt32* const SomeAtomicVar = (volatile UInt32*)0x11C5F58;
	if (!*SomeAtomicVar)
	{
		ThisStdCall(0x40FBF0, (void*)0x11C5F80, 0); //EnterLCS
		if (!*SomeAtomicVar)
		{
			ThisStdCall(0x43A080, NULL);
		}
		ThisStdCall(0x40FBA0, (void*)0x11C5F80); //LeaveLCS
	}
	DWORD* OldAtomicVar = (DWORD*) _InterlockedExchange(SomeAtomicVar, *SomeAtomicVar);
	OldAtomicVar[0] = 0;
	OldAtomicVar[1] = 0;
	OldAtomicVar[2] = 0;
	return (UInt32) OldAtomicVar;
}



void __stdcall ChangeTileLock2(DWORD* toExchange)
{
	static volatile UInt32* const SomeAtomicVar = (volatile UInt32*)0x11C5F58;
	toExchange[1] = 0;
	toExchange = (DWORD*)_InterlockedExchange(SomeAtomicVar, *SomeAtomicVar);
}



