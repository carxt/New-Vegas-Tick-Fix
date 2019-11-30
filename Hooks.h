#pragma once
char* movzxEaxECXplus4 = "\x0F\xB6\x41\x04\xC3";
char* leaEaxEcxPlus4 = "\x8D\x41\x04\xC3"; // lea eax, [ecx+4] ; ret

DWORD __fastcall HookHotSpot1(DWORD** ecx)
{
	DWORD MaxCount = (DWORD)ecx[0x13];
	WORD* CountSpot = (WORD*)ecx[0x12];
	DWORD i = 0;
	while (i < MaxCount)
	{
		if (*(CountSpot + i) == 0xFFFF) return i;
		++i;

	}
	return -1;
}

/*__declspec (naked) void Inline576D30()
{
	__asm
	{
		mov ecx, [ecx + 8]
		and ecx, 0x200000
		setnz al
		ret
	}
}
*/
char* Inline576D30 = "\x8B\x49\x08\x81\xE1\x00\x00\x20\x00\x0F\x95\xC0\xC3";

/*
__declspec(naked) __fastcall void GetStringSetting(char* settingName)
{
	_asm
	{
		test ecx, ecx
		je isNull
		mov eax, [ecx+4]
		ret
	isNull:
		xor eax, eax
		ret
	}
}
*/
char* GetStringSetting = "\x85\xC9\x74\x04\x8B\x41\x04\xC3\x31\xC0\xC3";

void __fastcall InlinedA694E0(DWORD* ecx, DWORD edx, DWORD* a2)
{
	DWORD v2, v9, v4, v5, v8;
	float* v6, * v7;
	v2 = a2[8];
	if (v2 && 0.0 != *(float*)(v2 + 0xC))
	{
		v9 = ecx[0x23];
		v4 = 0;
		v5 = 1;
		v6 = (float*)(ecx + 0xB);
		do
		{
			if (v5 & ecx[0x23])
			{
				v7 = (float*)(a2[8] ? a2[8] : 0x11F4288);
				float a3 = (*v6 * *v7 + v6[1] * v7[1] + v6[2] * v7[2]) - v6[3];;
				v8 = (a3 > -(v7[3])) ? (v7[3] <= a3) : 2;

				if (v8 == 2)
					break;
				if (v8 == 1)
					ecx[35] &= ~v5;
			}
			++v4;
			v6 += 4;
			v5 = _rotl(v5, 1);
		} while (v4 < 6);
		if (v4 == 6)
			(*(void(__stdcall * *)(DWORD*))(*a2 + 0xD4))(ecx);
		ecx[0x23] = v9;
	}
}

bool __fastcall StringNiTMap__Lookup(int ecx, int EDX, int a1, int a2)
{
	int* i = *(int**)(*(DWORD*)(ecx + 8) + 4 * (*(int(__thiscall * *)(int, int))(*(DWORD*)ecx + 4))(ecx, a1));
	while (i) {
		if ((*(unsigned __int8(__thiscall * *)(int, int, int))(*(DWORD*)ecx + 8))(ecx, a1, i[1]))
		{
			*(DWORD*)a2 = i[2];
			return 1;
		}
		i = (int*)* i;
	}
	return 0;
}

/*
float __fastcall Inline595C80(float* ecx)
{
	return *ecx * *ecx + ecx[1] * ecx[1];
}
*/
char* Inline595C80 = "\x51\xF3\x0F\x10\x49\x04\xF3\x0F\x10\x01\xF3\x0F\x59\xC0\xF3\x0F\x59\xC9\xF3\x0F\x58\xC1\xF3\x0F\x11\x04\x24\xD9\x04\x24\x59\xC3";

/*
float* __fastcall Inline416870(float* ecx, void* edx, float a2, float a3, float a4)
{
	*ecx = a2;
	ecx[1] = a3;
	ecx[2] = a4;
	return ecx;
}
*/
char* Inline416870 = "\xF3\x0F\x10\x44\x24\x04\x8B\xC1\xF3\x0F\x11\x01\xF3\x0F\x10\x44\x24\x08\xF3\x0F\x11\x41\x04\xF3\x0F\x10\x44\x24\x0C\xF3\x0F\x11\x41\x08\xC2\x0C\x00";

/*
float* __fastcall Inline439EF0(float* ecx, void* edx, float* a2, float* a3)
{
	*a2 = *ecx - *a3;
	a2[1] = ecx[1] - a3[1];
	a2[2] = ecx[2] - a3[2];
	return a2;
}
*/
char* Inline439EF0 = "\x8B\x54\x24\x08\xF3\x0F\x10\x01\x8B\x44\x24\x04\xF3\x0F\x5C\x02\xF3\x0F\x11\x00\xF3\x0F\x10\x41\x04\xF3\x0F\x5C\x42\x04\xF3\x0F\x11\x40\x04\xF3\x0F\x10\x41\x08\xF3\x0F\x5C\x42\x08\xF3\x0F\x11\x40\x08\xC2\x08\x00";

/*
signed int __fastcall Inline49DA80(NiPoint4* p1, void* dummyEDX, NiPoint3* p2)
{
	float distance = (p1->x * p2->x + p1->y * p2->y + p1->z * p2->z - p1->r);
	if (distance >= 0.0F)
	{
		return distance > 0.0F ? 1 : 0;
	}
	return 2;
}
*/
char* Inline49DA80 = "\x8B\x44\x24\x04\xF3\x0F\x10\x01\xF3\x0F\x10\x49\x04\xF3\x0F\x59\x00\xF3\x0F\x59\x48\x04\xF3\x0F\x58\xC8\xF3\x0F\x10\x41\x08\xF3\x0F\x59\x40\x08\xF3\x0F\x58\xC8\x0F\x57\xC0\xF3\x0F\x5C\x49\x0C\x0F\x2F\xC8\x72\x0B\x33\xC0\x0F\x2F\xC8\x0F\x97\xC0\xC2\x04\x00\xB8\x02\x00\x00\x00\xC2\x04\x00";

unsigned int** __fastcall NiTMapBase_FreeBuckets(unsigned int** ecx)
{
	unsigned int* v1; // ST08_4
	unsigned int** result; // eax
	unsigned int  i = 0; // [esp+8h] [ebp-4h]
	unsigned int ecxplus1 = *(unsigned int*)ecx[1];
	unsigned int* ecxplus2 = ecx[2];

	for (i = 0; i < (unsigned int)ecx[1]; ++i)
	{
		while (ecx[2][i])
		{
			v1 = (unsigned int*)ecx[2][i];
			ecx[2][i] = *(DWORD*)ecx[2][i];
			((void(__thiscall*)(unsigned int**, unsigned int*)) (long) ((*ecx)[4]))(ecx, v1);
			((void(__thiscall*)(unsigned int**, unsigned int*)) (long) ((*ecx)[6]))(ecx, v1);
		}
	}
	ecx[3] = 0;
	return ecx;
}

DWORD __cdecl TESTopic_getTopicInfoByIndex(DWORD a1, int a2)
{
	int* test = (int*)(a1 + 4);
	return (a1 == 0 || (test[3] < a2)) ? 0 : *(DWORD*)(test[1] + 4 * a2);

}

unsigned int __fastcall TESTopic_getTopicInfoByID(DWORD ecx, DWORD edx, int a2, char a3)
{
	char* v3; // eax
	int v5; // [esp+4h] [ebp-1Ch]
	unsigned int j; // [esp+8h] [ebp-18h]
	int v7; // [esp+Ch] [ebp-14h]
	int i; // [esp+10h] [ebp-10h]
	char* v9; // [esp+14h] [ebp-Ch]
	void* v10; // [esp+18h] [ebp-8h]
	unsigned int v11; // [esp+1Ch] [ebp-4h]

	v10 = (DWORD*)(ecx + 44);
	while (v10 && *(DWORD*)v10)
	{
		v9 = *(char**)(v10);
		v10 = *(void**)(((DWORD)v10) + 4);
		v11 = *(unsigned int*)((((DWORD)v9) + 4) + 0xC);
		//v11 = *(unsigned int*) (((DWORD)v9) + 0xC);
		if (v11)
		{
			if ((BYTE)a3)
			{
				for (i = v11 - 1; i >= 0; --i)
				{
					v7 = TESTopic_getTopicInfoByIndex((DWORD)v9, i);
					if (v7 && (*(DWORD*)(v7 + 0xC)) == a2)
						return v7;
				}
			}
			else
			{
				for (j = 0; j < v11; ++j)
				{
					v5 = TESTopic_getTopicInfoByIndex((DWORD)v9, j);
					if (v5 && (*(DWORD*)(v5 + 0xC)) == a2)
						return v5;
				}
			}
		}
	}
	return 0;
}

void HookInlines() {
	// replace functions with compiler optimised versions
	WriteRelCall(0x0AA6AC9, (UInt32)HookHotSpot1);
	WriteRelJump(0x0619F20, (UInt32)TESTopic_getTopicInfoByIndex);
	WriteRelJump(0xA694E0, (UInt32)InlinedA694E0);
	WriteRelJump(0xA1E380, (UInt32)StringNiTMap__Lookup);
	WriteRelJump(0x853130, (UInt32)StringNiTMap__Lookup);
	WriteRelJump(0x0438AF0, (UInt32)NiTMapBase_FreeBuckets);
	WriteRelJump(0x0619E10, (UInt32)TESTopic_getTopicInfoByID);
	SafeWriteBuf(0x0576D30, Inline576D30, 13);
	SafeWriteBuf(0x595C80, Inline595C80, 32);
	SafeWriteBuf(0x416870, Inline416870, 37);
	SafeWriteBuf(0x439EF0, Inline439EF0, 53);
	SafeWriteBuf(0x49DA80, Inline49DA80, 72); 

	// optimise GetSettingValue's
	// int
	SafeWriteBuf(0x43D4D0, leaEaxEcxPlus4, 4); 

	// float
	SafeWriteBuf(0x403E20, leaEaxEcxPlus4, 4);

	// string
	SafeWriteBuf(0x403DF0, GetStringSetting, 11);
	
	// float*
	SafeWriteBuf(0x408D60, leaEaxEcxPlus4, 4);
	
	// remove calls to FormatString in memory allocation functions
	WriteRelJump(0x0AA946D, 0x0AA94CB);
	WriteRelJump(0x0AA92B1, 0x0AA9310);
	WriteRelJump(0xAA964E, 0x0AA96AF);

	// TESForm::GetTypeID
	SafeWriteBuf(0x401150, movzxEaxECXplus4, 5);

	// TESForm::GetTypeID
	SafeWriteBuf(0x401170, movzxEaxECXplus4, 5);

	// inlines calls to set tls
	SafeWriteBuf(0x404EE0, "\xFF\x31\xE8\x49\x00\x00\x00\x83\xC4\x04\xC3", 11); // push [ecx]; call ...  ; add esp, 4; ret

	// modelToWorld
	// FLD ST0, DWORD PTR DS:[0x011C582C]
	// FMUL ST0, DWORD PTR SS : [ESP + 0x4]
	// ret
	SafeWriteBuf(0x4A3E90, "\xD9\x05\x2C\x58\x1C\x01\xD8\x4C\x24\x04\xC3", 11);

	// FormHeap::Free
	SafeWriteBuf(0x401030, "\xFF\x74\x24\x04\xB9\x38\x62\x1F\x01\xE8\x22\x30\x6A\x00\xC3", 15); // push [esp+4]; mov ecx, g_formHeap; call Game__MemoryPoolFree; ret

	// FormHeap::Allocate
	SafeWriteBuf(0x401000, "\xFF\x74\x24\x04\xB9\x38\x62\x1F\x01\xE8\x32\x2E\x6A\x00\xC3", 15); // push [esp+4]; mov ecx, g_formHeap; call Game__MemoryPoolAllocate; ret

	// this
	SafeWriteBuf(0x6815C0, "\x8B\xC1\xC3", 3); // mov eax, ecx; ret

	// this[1]
	SafeWriteBuf(0x726070, "\x8B\x41\x04\xC3", 4);

	// this[3]
	SafeWriteBuf(0x84E3A0, "\x8B\x41\x0C\xC3", 4);

	// this[5]
	SafeWriteBuf(0x825C00, "\x8B\x41\x14\xC3", 4);

	// NiTArray__calcOffsetForArrayOfUInt32
	SafeWriteBuf(0x877A30, "\x8B\x49\x04\x8B\x44\x24\x04\x8D\x04\x81\xC2\x04\x00", 13);

	// skip over useless formatString when loading
	WriteRelJump(0x467A84, 0x467B0E);

	// this + 4
	SafeWriteBuf(0x717E50, "\x8B\xC1\x83\xC0\x04\xC3", 6);

	// TESMobileObject__GetBaseProcess
	SafeWriteBuf(0x8D8520, "\x8B\x41\x68\xC3", 4);

	// this + 8
	SafeWriteBuf(0x413F40, "\x8B\xC1\x83\xC0\x08\xC3", 6);

	// return 0
	SafeWriteBuf(0x8D0370, "\x32\xC0\xC2\x04\x00", 5);

	// unused array function
	SafeWriteBuf(0x72BA80, "\xC2\x08\x00", 3); // ret 8

	// this[2]isZero
	SafeWriteBuf(0x76B610, "\x83\x79\x08\x00\x0F\x94\xC0\xC3", 8); // cmp [ecx + 8], 0; sete al; ret

	// return1
	SafeWriteBuf(0x401290, "\xB0\x01\xC2\x04\x00", 5); // mov al, 1; ret

	// return empty string
	SafeWriteBuf(0x401280, "\xB8\x84\x15\x01\x01\xC3", 6);

	// fillchar
	WriteRelJump(0x403D30, 0xEC61C0);

	// nullsub
	SafeWrite8(0x483710, 0xC3);
}
