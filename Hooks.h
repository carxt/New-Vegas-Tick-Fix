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

TESTopicInfo* __cdecl TESTopic_getTopicInfoByIndex(TESTopic::Info* info, int index)
{
	if (!info) return NULL;
	
	TopicInfoArray* array = &info->infoArray;
	if (index > array->numObjs) return NULL;

	return array->Get(index);
}

TESTopicInfo* __fastcall TESTopic_getTopicInfoByID(TESTopic* topic, DWORD edx, int ID, char getLastNotFirst)
{
	ListNode<TESTopic::Info>* iter = topic->infos.Head();
	TESTopic::Info* info;
	UInt32 size;
	TESTopicInfo** dataPtr;
	TESTopicInfo* someInfo;
	if (getLastNotFirst)
	{
		do
		{
			if (!(info = iter->data))
				continue;
			size = info->infoArray.numObjs;
			dataPtr = info->infoArray.data + size;
			while (size)
			{
				size--;
				dataPtr--;
				if ((someInfo = *dataPtr) && (someInfo->refID == ID))
					return someInfo;
			}
		} while (iter = iter->next);
	}
	else
	{
		do
		{
			if (!(info = iter->data))
				continue;
			size = info->infoArray.numObjs;
			dataPtr = info->infoArray.data;
			while (size)
			{
				size--;
				if ((someInfo = *dataPtr) && (someInfo->refID == ID))
					return someInfo;
				dataPtr++;
			}
		} while (iter = iter->next);
	}
	return NULL;
}

bool __cdecl TESTopic__Compare(TESTopic* a1, TESTopic* a2, char checkUnk28)
{
	if (!a1 || !a2)	return false;
	
	if (checkUnk28)
	{
		float diff = a2->unk28 - a1->unk28;
		if (diff > 0.0001) return true;
		if (fabs(diff) > 0.001) return false;

		char* name1 = a1->GetName();
		char* name2 = a2->GetName();

		if (*name1 < *name2) return true;
		return stricmp(name1, name2) > 0;
		
	}
	char* name1 = a1->GetName();
	char* name2 = a2->GetName();

	if (*name1 < *name2)
		return 1;
	return stricmp(name1, name2) > 0;
}

/*
bool __fastcall tList_isEmpty(ListNode<void*>* list)
{
	return !list->next && !list->data;
}

_declspec(naked) bool __fastcall tList_isEmpty(ListNode<void*>* list)
{
	_asm
	{
		cmp dword ptr ds : [ecx + 4] , 0
		jne nonEmpty
		cmp dword ptr ds : [ecx] , 0
		jne nonEmpty
		mov al, 1
		ret
	nonEmpty :
		xor al, al
		ret
	}
}
*/
char* tList_isEmpty = "\x3E\x83\x79\x04\x00\x75\x09\x3E\x83\x39\x00\x75\x03\xB0\x01\xC3\x32\xC0\xC3";

__declspec(naked) NiVector4* __cdecl Sub_4A3E00_Hook(NiVector4 *outVec, NiVector3 *inVec)
{
    __asm
    {
        mov      eax, 0x11C582C
        movd     xmm0, dword ptr [eax]
        movsldup xmm1, xmm0
        movlhps  xmm1, xmm1
        mov      eax, [esp+4]
        mov      ecx, [esp+8]
        movdqu   xmm0, xmmword ptr [ecx]
        mulps    xmm0, xmm1
        movdqu   xmmword ptr [eax], xmm0
        mov      dword ptr [eax+0xC], 0
        retn
    }
}

/*
void __stdcall wait(unsigned int a1, unsigned int a2, int a3, void* (__thiscall* a4)(unsigned int))
{
	while (--a3 >= 0)
	{
		a4(a1);
		a1 = a1 + a2;
	}
}

_declspec(naked) void wait()
{
	_asm
	{
		push esi
		mov esi, dword ptr ss : [esp + 0x10]
		dec esi
		js done
		push ebx
		mov ebx, dword ptr ss : [esp + 0x18]
		push ebp
		mov ebp, dword ptr ss : [esp + 0x14]
		push edi
		mov edi, dword ptr ss : [esp + 0x14]
	loopHead:
		mov ecx, edi
		call ebx
		add edi, ebp
		dec esi
		jns loopHead
		pop edi
		pop ebp
		pop ebx
	done:
		pop esi
		ret 0x10
	}
}
*/

char* wait = "\x56\x8B\x74\x24\x10\x4E\x78\x1B\x53\x8B\x5C\x24\x18\x55\x8B\x6C\x24\x14\x57\x8B\x7C\x24\x14\x8B\xCF\xFF\xD3\x03\xFD\x4E\x79\xF7\x5F\x5D\x5B\x5E\xC2\x10\x00";

/*
bool __stdcall intsAreEqual(int a1, int a2)
{
	return a1 == a2;
}

_declspec(naked) bool __stdcall intsAreEqual(int a1, int a2)
{
	_asm
	{
		mov ecx, dword ptr ss : [esp + 4]
		cmp ecx, dword ptr ss : [esp + 8]
		sete al
		ret 8
	}
}*/

char* intsAreEqual = "\x8B\x4C\x24\x04\x3B\x4C\x24\x08\x0F\x94\xC0\xC2\x08\x00";

/*
int __fastcall sub_6C6A60(int* thiss, void* edx, unsigned int a2)
{
	return a2 % thiss[1];
}


_declspec(naked) int __fastcall sub_6C6A60(int* thiss, void* edx, unsigned int a2)
{
	_asm
	{
		mov eax, dword ptr ss : [esp + 4]
		xor edx, edx
		div dword ptr ds : [ecx + 4]
		mov eax, edx
		ret 4
	}
}
*/
char* sub_6C6A60 = "\x36\x8B\x44\x24\x04\x33\xD2\x3E\xF7\x71\x04\x8B\xC2\xC2\x04\x00";

void HookInlines() 
{
	// replace functions with compiler optimised versions
	WriteRelCall(0x0AA6AC9, UInt32(HookHotSpot1));
	WriteRelCall(0x61A3F9, UInt32(TESTopic__Compare));
	WriteRelCall(0x61A654, UInt32(TESTopic__Compare));

	for (UInt32 patchAddr : {0x618779, 0x6199E1, 0x619CD0, 0x619DBC, 0x619EA4, 0x619EED})
	{
		WriteRelCall(patchAddr, UInt32(TESTopic_getTopicInfoByIndex));
	}

	WriteRelCall(0x6198A9, UInt32(TESTopic_getTopicInfoByID));
	WriteRelCall(0x61A35D, UInt32(TESTopic_getTopicInfoByID));

	WriteRelCall(0xA6967D, UInt32(InlinedA694E0));
	WriteRelCall(0xC4EF85, UInt32(InlinedA694E0));

	WriteRelJump(0xA1E380, UInt32(StringNiTMap__Lookup));
	WriteRelJump(0x853130, UInt32(StringNiTMap__Lookup));
	WriteRelJump(0x0438AF0, UInt32(NiTMapBase_FreeBuckets));
	
	SafeWriteBuf(0x0576D30, Inline576D30, 13);
	SafeWriteBuf(0x595C80, Inline595C80, 32);
	SafeWriteBuf(0x416870, Inline416870, 37);
	SafeWriteBuf(0x439EF0, Inline439EF0, 53);
	SafeWriteBuf(0x49DA80, Inline49DA80, 72); 
	SafeWriteBuf(0x6C6A60, sub_6C6A60, 18);
	SafeWriteBuf(0xAE8DC0, intsAreEqual, 14);
	SafeWriteBuf(0x6B81D0, intsAreEqual, 14);
	SafeWriteBuf(0x401050, wait, 39);

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

	// BaseProcess__GetProcessLevel
	SafeWriteBuf(0x45CD60, "\x8B\x41\x28\xC3", 4);

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

	// GetFormHeap
	SafeWriteBuf(0x401020, "\xB8\x38\x62\x1F\x01\xC3", 6);

	// tList_isEmpty
	SafeWriteBuf(0x8256D0, tList_isEmpty, 19);

	// some havok
	SafeWriteBuf(0x4A3E00, (char*)Sub_4A3E00_Hook, 0x2B);
}
