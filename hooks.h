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

/*
TESTopicInfo* __fastcall TESTopic_getTopicInfoByID(TESTopic* topic, DWORD edx, int ID, char getLastNotFirst)
{
	tList<TESTopic::Info> topicInfos = topic->infos;
	if (getLastNotFirst)
	{
		for (auto iter = topicInfos.Begin(); !iter.End(); ++iter)
		{
			TESTopic::Info* info = iter.Get();
			if (!info) continue;

			UInt32 size = info->infoArray.numObjs;
			for (int i = size - 1; i >= 0; --i)
			{
				TESTopicInfo* someInfo = info->infoArray.data[i];
				if (someInfo && someInfo->refID == ID)
					return someInfo;
			}
		}
	}
	else
	{
		for (auto iter = topicInfos.Begin(); !iter.End(); ++iter)
		{
			TESTopic::Info* info = iter.Get();
			if (!info) continue;

			UInt32 size = info->infoArray.numObjs;
			for (int i = 0; i < size; ++i)
			{
				TESTopicInfo* someInfo = info->infoArray.data[i];
				if (someInfo && someInfo->refID == ID)
					return someInfo;
			}
		}
	}
	return 0;
}
*/

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
		return _stricmp(name1, name2) > 0;

	}
	char* name1 = a1->GetName();
	char* name2 = a2->GetName();

	if (*name1 < *name2)
		return 1;
	return _stricmp(name1, name2) > 0;
}

/*
bool __cdecl Test_TESTopicCompare(TESTopic* a1, TESTopic* a2, char a3)
{
	bool vanillaResult = ((bool(__cdecl*)(TESTopic * a1, TESTopic * a2, char a3))(0x61A450))(a1, a2, a3);
	bool myResult = TESTopic__Compare(a1, a2, a3);
	if (vanillaResult != myResult) { _asm int 3 };
	return vanillaResult;
}
*/

/*
DWORD __fastcall Test_HookHotSpot1(DWORD** ecx)
{
	DWORD vanilla = ((DWORD(__thiscall*)(DWORD * *ecx))(0xAA6DB0))(ecx);
	DWORD myResult = HookHotSpot1(ecx);
	if (vanilla != myResult) { _asm int 3 };
	return vanilla;
}
*/

/*
DWORD __cdecl Test_TESTopic_getTopicInfoByIndex(DWORD a1, int a2)
{
	DWORD vanilla = ((int(__cdecl*)(DWORD a1, unsigned int a2))(0x619F20))(a1, a2);
	DWORD myResult = TESTopic_getTopicInfoByIndex(a1, a2);
	if (vanilla != myResult) { _asm int 3 };
	return vanilla;
}
*/


TESTopicInfo* __fastcall Test_TESTopic_getTopicInfoByID(TESTopic* ecx, DWORD edx, int id, char getLastNotFirst)
{
	TESTopicInfo* vanilla = ((TESTopicInfo * (__thiscall*)(TESTopic * ecx, int a2, char a3))(0x619E10))(ecx, id, getLastNotFirst);
	TESTopicInfo* myResult = TESTopic_getTopicInfoByID(ecx, 0, id, getLastNotFirst);
	if (vanilla != myResult) { _asm int 3 }
	return vanilla;
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

/*
untested
void __cdecl sub_61F360(TESTopicInfo* toAdd, tList<TESTopicInfo>* topicsList)
{
	if (toAdd && topicsList)
	{
		if (topicsList->Empty())
		{
			topicsList->Insert(toAdd);
		}
		else
		{
			for (auto iter = topicsList->Begin(); !iter.End(); ++iter)
			{
				TESTopicInfo* iterTopic = iter.Get();

				// if topic is already in list, return
				if (toAdd == iterTopic) return;

				if (iterTopic > toAdd)
				{
					topicsList->Insert(toAdd);
					return;
				}
			}
			topicsList->Append(toAdd);
		}
	}
}
*/

__declspec(naked) NiVector4* __cdecl Sub_4A3E00_Hook(NiVector4* outVec, NiVector3* inVec)
{
	__asm
	{
		mov      eax, 0x11C582C
		movd     xmm0, dword ptr[eax]
		movsldup xmm1, xmm0
		movlhps  xmm1, xmm1
		mov      eax, [esp + 4]
		mov      ecx, [esp + 8]
		movdqu   xmm0, xmmword ptr[ecx]
		mulps    xmm0, xmm1
		movdqu   xmmword ptr[eax], xmm0
		mov      dword ptr[eax + 0xC], 0
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

/*
BOOL __stdcall Tile__MenuStringMap__Func0002(char* a1, char* a2)
{
	return stricmp(a1, a2) == 0;
}

_declspec(naked) bool Tile__MenuStringMap__Func0002(char* a1, char* a2)
{
	_asm
	{
		push dword ptr ss : [esp + 8]
		push dword ptr ss : [esp + 8]
		call 0xEC68E4
		add esp, 8
		neg eax
		sbb eax, eax
		inc eax
		ret 8
	}
}
*/

char* Tile__MenuStringMap__Func0002 = "\xFF\x74\x24\x08\xFF\x74\x24\x08\xE8\x55\x97\x4B\x00\x83\xC4\x08\xF7\xD8\x1B\xC0\x40\xC2\x08\x00";
//	SafeWriteBuf(0xA0D180, Tile__MenuStringMap__Func0002, 0x21); NOT WORKING for some reason

void __cdecl sub_AC3300(byte* a1, int a2, byte* a3)
{
	while (a2)
	{
		*a1 = *a3;
		a2--;
		++a1;
	}
}

/*
int __fastcall sub_60B980(int** ecx)
{
	if (*ecx)
	{
		return **ecx;
	}
	return 0;
}
*/

_declspec(naked) int __fastcall sub_60B980(int** ecx)
{
	_asm
	{
		mov eax, [ecx]
		test eax, eax
		je done
		mov eax, [eax]
		done:
		ret
	}
}

/*
NiMatrix33* __fastcall NiMatrix33__Multiply(NiMatrix33* a1, void* edx, NiMatrix33* out, NiMatrix33* a3)
{
	out->cr[0][0] = a1->cr[0][0] * a3->cr[0][0] + a1->cr[0][1] * a3->cr[1][0] + a1->cr[0][2] * a3->cr[2][0];
	out->cr[1][0] = a1->cr[1][0] * a3->cr[0][0] + a1->cr[1][1] * a3->cr[1][0] + a1->cr[1][2] * a3->cr[2][0];
	out->cr[2][0] = a1->cr[2][0] * a3->cr[0][0] + a1->cr[2][1] * a3->cr[1][0] + a1->cr[2][2] * a3->cr[2][0];
	out->cr[0][1] = a1->cr[0][0] * a3->cr[0][1] + a1->cr[0][1] * a3->cr[1][1] + a1->cr[0][2] * a3->cr[2][1];
	out->cr[1][1] = a1->cr[1][0] * a3->cr[0][1] + a1->cr[1][1] * a3->cr[1][1] + a1->cr[1][2] * a3->cr[2][1];
	out->cr[2][1] = a1->cr[2][0] * a3->cr[0][1] + a1->cr[2][1] * a3->cr[1][1] + a1->cr[2][2] * a3->cr[2][1];
	out->cr[0][2] = a1->cr[0][0] * a3->cr[0][2] + a1->cr[0][1] * a3->cr[1][2] + a1->cr[0][2] * a3->cr[2][2];
	out->cr[1][2] = a1->cr[1][0] * a3->cr[0][2] + a1->cr[1][1] * a3->cr[1][2] + a1->cr[1][2] * a3->cr[2][2];
	out->cr[2][2] = a1->cr[2][0] * a3->cr[0][2] + a1->cr[2][1] * a3->cr[1][2] + a1->cr[2][2] * a3->cr[2][2];
	return out;
}
*/

char* NiMatrix33__Multiply = "\x8B\x54\x24\x08\xF3\x0F\x10\x09\x8B\x44\x24\x04\xF3\x0F\x59\x0A\xF3\x0F\x10\x42\x0C\xF3\x0F\x59\x41\x04\xF3\x0F\x58\xC8\xF3\x0F\x10\x42\x18\xF3\x0F\x59\x41\x08\xF3\x0F\x58\xC8\xF3\x0F\x11\x08\xF3\x0F\x10\x49\x0C\xF3\x0F\x59\x0A\xF3\x0F\x10\x41\x10\xF3\x0F\x59\x42\x0C\xF3\x0F\x58\xC8\xF3\x0F\x10\x41\x14\xF3\x0F\x59\x42\x18\xF3\x0F\x58\xC8\xF3\x0F\x11\x48\x0C\xF3\x0F\x10\x49\x18\xF3\x0F\x59\x0A\xF3\x0F\x10\x41\x1C\xF3\x0F\x59\x42\x0C\xF3\x0F\x58\xC8\xF3\x0F\x10\x41\x20\xF3\x0F\x59\x42\x18\xF3\x0F\x58\xC8\xF3\x0F\x11\x48\x18\xF3\x0F\x10\x4A\x04\xF3\x0F\x59\x09\xF3\x0F\x10\x42\x10\xF3\x0F\x59\x41\x04\xF3\x0F\x58\xC8\xF3\x0F\x10\x42\x1C\xF3\x0F\x59\x41\x08\xF3\x0F\x58\xC8\xF3\x0F\x11\x48\x04\xF3\x0F\x10\x49\x10\xF3\x0F\x59\x4A\x10\xF3\x0F\x10\x41\x0C\xF3\x0F\x59\x42\x04\xF3\x0F\x58\xC8\xF3\x0F\x10\x41\x14\xF3\x0F\x59\x42\x1C\xF3\x0F\x58\xC8\xF3\x0F\x11\x48\x10\xF3\x0F\x10\x49\x1C\xF3\x0F\x59\x4A\x10\xF3\x0F\x10\x41\x18\xF3\x0F\x59\x42\x04\xF3\x0F\x58\xC8\xF3\x0F\x10\x41\x20\xF3\x0F\x59\x42\x1C\xF3\x0F\x58\xC8\xF3\x0F\x11\x48\x1C\xF3\x0F\x10\x09\xF3\x0F\x59\x4A\x08\xF3\x0F\x10\x41\x04\xF3\x0F\x59\x42\x14\xF3\x0F\x58\xC8\xF3\x0F\x10\x41\x08\xF3\x0F\x59\x42\x20\xF3\x0F\x58\xC8\xF3\x0F\x11\x48\x08\xF3\x0F\x10\x49\x10\xF3\x0F\x59\x4A\x14\xF3\x0F\x10\x41\x0C\xF3\x0F\x59\x42\x08\xF3\x0F\x58\xC8\xF3\x0F\x10\x41\x14\xF3\x0F\x59\x42\x20\xF3\x0F\x58\xC8\xF3\x0F\x11\x48\x14\xF3\x0F\x10\x41\x18\xF3\x0F\x10\x49\x1C\xF3\x0F\x59\x42\x08\xF3\x0F\x59\x4A\x14\xF3\x0F\x58\xC8\xF3\x0F\x10\x41\x20\xF3\x0F\x59\x42\x20\xF3\x0F\x58\xC8\xF3\x0F\x11\x48\x20\xC2\x08\x00";

/*
bool __fastcall NiTimeController__ChecksArg0andUnk030isNotNull(UInt32* a1, void* edx, int a2)
{
	return (a2 & a1[12]) != 0;
}

_declspec(naked) void NiTimeController__ChecksArg0andUnk030isNotNull()
{
	_asm
	{
		mov eax, dword ptr ds : [ecx + 0x30]
		and eax, dword ptr ss : [esp + 0x4]
		neg eax
		sbb eax, eax
		neg eax
		ret 4
	}
}
*/
char* NiTimeController__ChecksArg0andUnk030isNotNull = "\x8B\x41\x30\x23\x44\x24\x04\xF7\xD8\x1B\xC0\xF7\xD8\xC2\x04\x00";

/*
void __fastcall sub_A7FDD0(float* thiss, void* edx, float* a2, float* a3)
{
	float v4, v5, v6, v7, v11, v12, v13, v14;

	v4 = a3[1] * a2[1] + *a2 * *a3 + a3[2] * a2[2];
	v5 = a3[3] * *a2 + a3[4] * a2[1] + a3[5] * a2[2];
	v6 = a3[6] * *a2 + a3[7] * a2[1] + a3[8] * a2[2];
	v7 = a3[12];
	v11 = a3[9] + v4 * v7;
	v12 = a3[10] + v5 * v7;
	v13 = a3[11];
	*thiss = v11;
	thiss[1] = v12;
	v14 = v13 + v7 * v6;
	thiss[2] = v14;
	thiss[3] = a2[3] * a3[12];
}
*/
char* sub_A7FDD0 = "\x8B\x44\x24\x08\x8B\x54\x24\x04\xF3\x0F\x10\x48\x18\xF3\x0F\x10\x6A\x04\xF3\x0F\x10\x3A\xF3\x0F\x10\x40\x1C\xF3\x0F\x10\x72\x08\xF3\x0F\x10\x58\x0C\xF3\x0F\x59\xC5\xF3\x0F\x59\xCF\xF3\x0F\x10\x60\x30\xF3\x0F\x10\x50\x2C\xF3\x0F\x58\xC8\xF3\x0F\x59\xDF\xF3\x0F\x10\x40\x20\xF3\x0F\x59\x38\xF3\x0F\x59\xC6\xF3\x0F\x58\xC8\xF3\x0F\x10\x40\x10\xF3\x0F\x59\xC5\xF3\x0F\x58\xD8\xF3\x0F\x11\x4C\x24\x04\xF3\x0F\x10\x40\x14\xF3\x0F\x10\x48\x04\xF3\x0F\x59\xC6\xF3\x0F\x59\xCD\xF3\x0F\x58\xD8\xF3\x0F\x10\x40\x08\xF3\x0F\x59\xC6\xF3\x0F\x58\xCF\xF3\x0F\x59\xDC\xF3\x0F\x58\xC8\xF3\x0F\x58\x58\x28\xF3\x0F\x59\xCC\xF3\x0F\x59\x64\x24\x04\xF3\x0F\x58\x48\x24\xF3\x0F\x58\xE2\xF3\x0F\x11\x09\xF3\x0F\x11\x59\x04\xF3\x0F\x11\x61\x08\xF3\x0F\x10\x42\x0C\xF3\x0F\x59\x40\x30\xF3\x0F\x11\x41\x0C\xC2\x08\x00";

/*
void __fastcall sub_65B390(UInt32* thiss, void* edx, int a2, UInt32* a3)
{
	UInt32* v4;

	v4 = (UInt32*)(*thiss + thiss[1] * a2);
	*v4 = *a3;
	v4[1] = a3[1];
	v4[2] = a3[2];
}
*/

char* sub_65B390 = "\x8B\x51\x04\x0F\xAF\x54\x24\x04\x03\x11\x8B\x4C\x24\x08\x8B\x01\x89\x02\x8B\x41\x04\x89\x42\x04\x8B\x41\x08\x89\x42\x08\xC2\x08\x00";

/*
void __fastcall sub_476930(float* thiss, void* edx, int a2, float* a3)
{
	*a3 = thiss[3 * a2];
	a3[1] = thiss[3 * a2 + 1];
	a3[2] = thiss[3 * a2 + 2];
}
*/
char* sub_476930 = "\x8B\x44\x24\x04\x8B\x54\x24\x08\x56\x8D\x34\x40\x8B\x04\xB1\x89\x02\x8B\x44\xB1\x04\x89\x42\x04\x8B\x44\xB1\x08\x89\x42\x08\x5E\xC2\x08\x00";

/*
UInt32* __stdcall sub_57CBE0(UInt32** a1)
{
	UInt32* v1;

	v1 = *a1 + 2;
	*a1 = (UInt32*) **a1;
	return v1;
}
*/
char* sub_57CBE0 = "\x8B\x54\x24\x04\x8B\x0A\x8D\x41\x08\x8B\x09\x89\x0A\xC2\x04\x00";


namespace Jazz
{
	__declspec(naked) void __fastcall RotationMatrixHook(NiMatrix33* resMat, int EDX, float angX, float angY, float angZ)
	{
		__asm
		{
			push    ecx
			fld     dword ptr[esp + 8]
			fsincos
			fstp    dword ptr[esp]
			movd    xmm1, dword ptr[esp]
			fstp    dword ptr[esp]
			movd    xmm0, dword ptr[esp]
			fld     dword ptr[esp + 0xC]
			fsincos
			fstp    dword ptr[esp]
			movd    xmm3, dword ptr[esp]
			fstp    dword ptr[esp]
			movd    xmm2, dword ptr[esp]
			fld     dword ptr[esp + 0x10]
			fsincos
			fstp    dword ptr[esp]
			movd    xmm5, dword ptr[esp]
			fstp    dword ptr[esp]
			movd    xmm4, dword ptr[esp]
			movss   xmm6, xmm3
			mulss   xmm6, xmm5
			movd[ecx], xmm6
			movss   xmm6, xmm3
			mulss   xmm6, xmm4
			movd[ecx + 4], xmm6
			xorps   xmm6, xmm6
			subss   xmm6, xmm2
			movd[ecx + 8], xmm6
			movss   xmm6, xmm0
			mulss   xmm6, xmm2
			mulss   xmm6, xmm5
			movss   xmm7, xmm1
			mulss   xmm7, xmm4
			subss   xmm6, xmm7
			movd[ecx + 0xC], xmm6
			movss   xmm6, xmm1
			mulss   xmm6, xmm5
			movss   xmm7, xmm0
			mulss   xmm7, xmm2
			mulss   xmm7, xmm4
			addss   xmm6, xmm7
			movd[ecx + 0x10], xmm6
			movss   xmm6, xmm0
			mulss   xmm6, xmm3
			movd[ecx + 0x14], xmm6
			movss   xmm6, xmm0
			mulss   xmm6, xmm4
			movss   xmm7, xmm1
			mulss   xmm7, xmm2
			mulss   xmm7, xmm5
			addss   xmm6, xmm7
			movd[ecx + 0x18], xmm6
			movss   xmm6, xmm1
			mulss   xmm6, xmm2
			mulss   xmm6, xmm4
			movss   xmm7, xmm0
			mulss   xmm7, xmm5
			subss   xmm6, xmm7
			movd[ecx + 0x1C], xmm6
			movss   xmm6, xmm1
			mulss   xmm6, xmm3
			movd[ecx + 0x20], xmm6
			pop     ecx
			retn    0xC
		}
	}



	_declspec(naked) NiMatrix33* __fastcall MatrixMultiplyHook(NiMatrix33* matrixA, int EDX, NiMatrix33* matrixAB, NiMatrix33* matrixB)
	{
		__asm
		{
			mov     eax, [esp + 4]
			mov     edx, [esp + 8]
			movd    xmm0, [ecx]
			mulss   xmm0, [edx]
			movd    xmm1, [ecx + 4]
			mulss   xmm1, [edx + 0xC]
			addss   xmm0, xmm1
			movd    xmm1, [ecx + 8]
			mulss   xmm1, [edx + 0x18]
			addss   xmm0, xmm1
			movd[eax], xmm0
			movd    xmm0, [ecx + 0xC]
			mulss   xmm0, [edx]
			movd    xmm1, [ecx + 0x10]
			mulss   xmm1, [edx + 0xC]
			addss   xmm0, xmm1
			movd    xmm1, [ecx + 0x14]
			mulss   xmm1, [edx + 0x18]
			addss   xmm0, xmm1
			movd[eax + 0xC], xmm0
			movd    xmm0, [ecx + 0x18]
			mulss   xmm0, [edx]
			movd    xmm1, [ecx + 0x1C]
			mulss   xmm1, [edx + 0xC]
			addss   xmm0, xmm1
			movd    xmm1, [ecx + 0x20]
			mulss   xmm1, [edx + 0x18]
			addss   xmm0, xmm1
			movd[eax + 0x18], xmm0
			movd    xmm0, [ecx]
			mulss   xmm0, [edx + 4]
			movd    xmm1, [ecx + 4]
			mulss   xmm1, [edx + 0x10]
			addss   xmm0, xmm1
			movd    xmm1, [ecx + 8]
			mulss   xmm1, [edx + 0x1C]
			addss   xmm0, xmm1
			movd[eax + 4], xmm0
			movd    xmm0, [ecx + 0xC]
			mulss   xmm0, [edx + 4]
			movd    xmm1, [ecx + 0x10]
			mulss   xmm1, [edx + 0x10]
			addss   xmm0, xmm1
			movd    xmm1, [ecx + 0x14]
			mulss   xmm1, [edx + 0x1C]
			addss   xmm0, xmm1
			movd[eax + 0x10], xmm0
			movd    xmm0, [ecx + 0x18]
			mulss   xmm0, [edx + 4]
			movd    xmm1, [ecx + 0x1C]
			mulss   xmm1, [edx + 0x10]
			addss   xmm0, xmm1
			movd    xmm1, [ecx + 0x20]
			mulss   xmm1, [edx + 0x1C]
			addss   xmm0, xmm1

			movd[eax + 0x1C], xmm0
			movd    xmm0, [ecx]
			mulss   xmm0, [edx + 8]
			movd    xmm1, [ecx + 4]
			mulss   xmm1, [edx + 0x14]
			addss   xmm0, xmm1
			movd    xmm1, [ecx + 8]
			mulss   xmm1, [edx + 0x20]
			addss   xmm0, xmm1
			movd[eax + 8], xmm0
			movd    xmm0, [ecx + 0xC]
			mulss   xmm0, [edx + 8]
			movd    xmm1, [ecx + 0x10]
			mulss   xmm1, [edx + 0x14]
			addss   xmm0, xmm1
			movd    xmm1, [ecx + 0x14]
			mulss    xmm1, [edx + 0x20]
			addss   xmm0, xmm1
			movd[eax + 0x14], xmm0
			movd    xmm0, [ecx + 0x18]
			mulss   xmm0, [edx + 8]
			movd    xmm1, [ecx + 0x1C]
			mulss   xmm1, [edx + 0x14]
			addss   xmm0, xmm1
			movd    xmm1, [ecx + 0x20]
			mulss   xmm1, [edx + 0x20]
			addss   xmm0, xmm1
			movd[eax + 0x20], xmm0
			retn    8
		}
	}

	__declspec(naked) NiVector3* __fastcall UpdateVector3_ToddSnig_Hook(NiVector3* thisVec, int EDX, float X, float Y, float Z)
	{
		__asm
		{
			lea     eax, [esp + 4]
			movq    xmm0, qword ptr[eax]
			movq    qword ptr[ecx], xmm0
			mov     eax, [eax + 8]
			mov[ecx + 8], eax
			mov     eax, ecx
			retn    0xC
		}
	}

	__declspec(naked) NiVector3* __fastcall MatrixVectorMultiplyHook(NiMatrix33* matrix, int EDX, NiVector3* result, NiVector3* inVec3)
	{
		__asm
		{
			mov     eax, [esp + 4]
			mov     edx, [esp + 8]
			movd    xmm0, [edx]
			movd    xmm1, [edx + 4]
			movd    xmm2, [edx + 8]
			movd    xmm3, [ecx]
			mulss   xmm3, xmm0
			movd    xmm4, [ecx + 4]
			mulss   xmm4, xmm1
			addss   xmm3, xmm4
			movd    xmm4, [ecx + 8]
			mulss   xmm4, xmm2
			addss   xmm3, xmm4
			movd[eax], xmm3
			movd    xmm3, [ecx + 0xC]
			mulss   xmm3, xmm0
			movd    xmm4, [ecx + 0x10]
			mulss   xmm4, xmm1
			addss   xmm3, xmm4
			movd    xmm4, [ecx + 0x14]
			mulss   xmm4, xmm2
			addss   xmm3, xmm4
			movd[eax + 4], xmm3
			movd    xmm3, [ecx + 0x18]
			mulss   xmm3, xmm0
			movd    xmm4, [ecx + 0x1C]
			mulss   xmm4, xmm1
			addss   xmm3, xmm4
			movd    xmm4, [ecx + 0x20]
			mulss   xmm4, xmm2
			addss   xmm3, xmm4
			movd[eax + 8], xmm3
			retn    8
		}
	}

	__declspec(naked) void __fastcall Vector3NormalizeHook(NiVector3* inVect)
	{
		__asm
		{
			movd    xmm0, [ecx]
			mulss   xmm0, xmm0
			movd    xmm1, [ecx + 4]
			mulss   xmm1, xmm1
			addss   xmm0, xmm1
			movd    xmm1, [ecx + 8]
			mulss   xmm1, xmm1
			addss   xmm0, xmm1
			sqrtss  xmm0, xmm0
			xorpd   xmm1, xmm1
			comiss  xmm0, xmm1
			jz        zeroLen
			movd    xmm1, [ecx]
			divss   xmm1, xmm0
			movd[ecx], xmm1
			movd    xmm1, [ecx + 4]
			divss   xmm1, xmm0
			movd[ecx + 4], xmm1
			movd    xmm1, [ecx + 8]
			divss   xmm1, xmm0
			movd[ecx + 8], xmm1
			retn
			zeroLen :
			movq    qword ptr[ecx], xmm1
				mov     dword ptr[ecx + 8], 0
				retn
		}
	}

	void WriteHooks() {
		WriteRelJump(0xA59540, (UInt32)RotationMatrixHook);
		WriteRelJump(0x43F8D0, (UInt32)MatrixMultiplyHook);
		WriteRelJump(0x416870, (UInt32)UpdateVector3_ToddSnig_Hook);
		WriteRelJump(0x4B4500, (UInt32)MatrixVectorMultiplyHook);
		WriteRelJump(0x4A0C10, (UInt32)Vector3NormalizeHook);

	}
}

/*
UInt32 tListGetSize(tList<void*>* list)
{
	return list->Count();
}

_declspec(naked) UInt32 __fastcall tListGetSize(tList<void*>* list)
{
	_asm
	{
		xor eax, eax
		cmp [ecx], 0
		jne nonEmpty
		ret
	nonEmpty:
		mov ecx, [ecx + 4]
		inc eax
		test ecx, ecx
		je done
	iterHead:
		mov ecx, [ecx + 4]
		inc eax
		test ecx, ecx
		jne iterHead
	done:
		ret
	}
}
*/
char* tListGetSize = "\x33\xC0\x80\x39\x00\x75\x01\xC3\x8B\x49\x04\x40\x85\xC9\x74\x08\x8B\x49\x04\x40\x85\xC9\x75\xF8\xC3";


void HookInlines()
{

	SafeWriteBuf(0x57CBE0, sub_57CBE0, 16);
	//	SafeWriteBuf(0x476930, sub_476930, 35);
	SafeWriteBuf(0x65B390, sub_65B390, 33);
	SafeWriteBuf(0x456630, NiTimeController__ChecksArg0andUnk030isNotNull, 16);
	SafeWriteBuf(0x43F8D0, NiMatrix33__Multiply, 391);
	SafeWriteBuf(0xA7FDD0, sub_A7FDD0, 198);

	// replace calls to sub_AC3300
	for (UInt32 patchAddr : {0xAB35E4, 0xABC03A, 0xACADF2, 0xAD1358, 0xAD1772})
	{
		WriteRelCall(patchAddr, UInt32(sub_AC3300));
	}

	// replace calls to FaceGenData__Iterator__FaceGenData__GetUnk00
	for (UInt32 patchAddr : {0x60B92F, 0x60B948, 0x64DDC2, 0x64DE12, 0x6591B2, 0x65DB02})
	{
		WriteRelCall(patchAddr, UInt32(sub_60B980));
	}
	SafeWriteBuf(0x5CA4F0, "\x83\x39\x00\x0F\x95\xC0\xC3", 7);

	SafeWriteBuf(0x64DDA0, "\x8B\x41\x04\xC3", 4);

	SafeWriteBuf(0x6C6A60, sub_6C6A60, 18);

	SafeWriteBuf(0xAE8DC0, intsAreEqual, 18);
	SafeWriteBuf(0x6B81D0, intsAreEqual, 18);

	SafeWriteBuf(0x401050, wait, 39);

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

	//	WriteRelCall(0x4617C4, UInt32(sub_61F360)); untested

	WriteRelJump(0xA1E380, UInt32(StringNiTMap__Lookup));
	WriteRelJump(0x853130, UInt32(StringNiTMap__Lookup));
	WriteRelJump(0x0438AF0, UInt32(NiTMapBase_FreeBuckets));

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
	// inline frequent calls to it
	SafeWriteBuf(0x96B1FA, "\x8B\x41\x68\x90\x90", 5);
	SafeWriteBuf(0x96B3DD, "\x8B\x41\x68\x90\x90", 5);

	// BaseProcess__GetProcessLevel
	SafeWriteBuf(0x45CD60, "\x8B\x41\x28\xC3", 4);

	// actually inline BaseProcess__GetProcessLevel in some function that calls it repeatedly
	SafeWriteBuf(0x96B240, "\x83\x79\x28\x03\x74\x15\x90\x90\x90\x90", 10);

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
	SafeWrite32(0x4534F0, 0xCC0004C2);

	// GetFormHeap
	SafeWriteBuf(0x401020, "\xB8\x38\x62\x1F\x01\xC3", 6);

	// tList_isEmpty
	SafeWriteBuf(0x8256D0, tList_isEmpty, 19);

	// some havok
	SafeWriteBuf(0x4A3E00, (char*)Sub_4A3E00_Hook, 0x2B);

	// optimise an audio function's loop
	SafeWrite32(0xAEBE01, 0x1BEBE87D);
	SafeWriteBuf(0xAEBE20, "\x3B\x7D\xCC\x73\x62\x8B\x75\x94\x3B\x75", 10);
	SafeWriteBuf(0xAEBE6E, "\x8A\x0E\x88\x0C\x3A\xFF\x45\x94\x47\xEB\xA7", 11);

	// remove useless push args call pop args for calls to __cdecl function by just jumping to it
	WriteRelJump(0x4EDE70, 0xAA10F0);

	// remove calls to dereference in frequently called places
	// NiNode__GetParent
	SafeWrite32(0x6838B0, 0xC31C418B);
	// SceneGraph__GetCamera
	SafeWriteBuf(0x6629F0, "\x8B\x81\xAC\x00\x00\x00\xC3", 7);
	// inline calls to it
	SafeWriteBuf(0x6FA89E, "\x8B\x89\xAC\x00\x00\x00\x90", 7);
	SafeWriteBuf(0x6FA8C8, "\x8B\x89\xAC\x00\x00\x00\x90", 7);
	SafeWriteBuf(0x6FA8F2, "\x8B\x89\xAC\x00\x00\x00\x90", 7);

	// TESObjectREFR__GetNiNode
	SafeWriteBuf(0x43FD1D, "\x8B\x41\x14\x89\x45\xF4\xEB\x0C", 8);
	// NiObjectNet_GetController
	SafeWrite32(0x43B230, 0xC30C418B); // replace return dereference(&(ecx+c)); with return ecx+c
	// inline some frequent calls to NiObjectNet_GetController
	SafeWriteBuf(0x4EFFA6, "\x8B\x41\x0C\x85\xC0\x74\x16\x90\x90", 9);
	SafeWriteBuf(0x4EFFC8, "\x8B\x41\x0C\x90\x90", 5);
	SafeWriteBuf(0x6FA8A5, "\x8B\x41\x0C\x90\x90", 5);
	SafeWriteBuf(0x6FA8CF, "\x8B\x41\x0C\x90\x90", 5);
	SafeWriteBuf(0x6FA8F9, "\x8B\x41\x0C\x90\x90", 5);

	// TESForm__GetFlags
	SafeWrite32(0x44DDC0, 0xC308418B);
	SafeWrite32(0X620B80, 0xC308418B);

	// inline call to TESForm__GetFlags
	SafeWriteBuf(0x43B540, "\x83\xC1\x14\x8B\x41\x08\xC3", 7);

	// BSExtraData__GetType
	SafeWriteBuf(0x4F1540, "\x8A\x41\x04\xC3", 4);
	// inline call to it
	SafeWriteBuf(0x410286, "\x8A\x41\x04\x3A\x45\x08\x75\x20\xEB\x06\x90\x90\x90\x90\x90\x90", 16);

	// minor inlines for sub_61F360 till the function is rewritten
	SafeWriteBuf(0x61F382, "\x84\xC0\x74\x11\x90\x90\x90", 7);
	SafeWriteBuf(0x61F3B3, "\x45\xFC\x8B\x08\x89\x4D\xF8\x3B\x4D\x08\x75\x0A\xEB\x54", 16);
	SafeWriteBuf(0x61F3F1, "\x8B\x41\x04\x85\xC0\x75\x10\x90\x90", 9);
	SafeWriteBuf(0x61F40B, "\x8B\x41\x04\xEB\x8A", 5);
	SafeWriteBuf(0x61F398, "\x45\x0C\x89\x45\xFC\x85\xC0\x74\x74\x90\x90", 11);

	// TESForm__IsDisabled
	SafeWriteBuf(0x440DA0, "\x8B\x41\x08\xA9\x00\x08\x00\x00\x0F\x95\xC0\xC3", 12);

	// TESObjectREFR__GetBaseForm_getUnk020
	SafeWrite32(0x7AF430, 0xC320418B);
	SafeWrite32(0x4181E0, 0xC320418B);

	Jazz::WriteHooks();

	// skip redundant check whether item is in right items list in ContainerMenu::ShouldHideItem
	SafeWrite8(0x75E694, 0x29);

	// remove calls to InterlockedIncrement and InterlockedDecrement for a counter for the number of Nifs (it's never checked)
	const UInt32 numRefsInterlockIndirectCalls[] =
	{
		0xA5D3B5, 0xA5D3DB, 0xA5D4EE, 0xA76665, 0xA766BB, 0xA76BEB, 0xA8F455, 0xA9BE95, 0xA9BEBE, 0xAA03A4, 0xAF7F58, 0xAF8EE7, 0xB592DC, 0xB60B24, 0xB66825, 0xB668A2,
		0xB668EA, 0xB9FDDB, 0xBB33D5, 0xBB34D2, 0xBB398A, 0xBB3B6B, 0xC1CF35, 0xC5AA10, 0xC5AE55, 0xE7D20B, 0xE7D277, 0xE7D3B6, 0xE7D53B, 0xE7D65F, 0xE7D6D7, 0xE7D876,
		0xE7D954, 0xE7DC2F, 0xE81BB4, 0xE877E6, 0xE881B7, 0xE897E4, 0xE89896, 0xE8D0B9, 0xE8D0F6, 0xE8EAE4, 0xE8FA60, 0xE8FB36, 0xE90A95, 0xE90AE6, 0xE93AE5, 0xE93E59,
		0xE98924, 0xE98A5F, 0xEE9C04, 0xEE9DED, 0xEEA594,
	};

	for (UInt32 patchAddr : numRefsInterlockIndirectCalls)
	{
		NopIndirectCall(patchAddr, 1);
	}

	const UInt32 numRefsInterlockRegCalls[] =
	{
		0xA76ADD, 0xA8F4BC, 0xA8F4FC, 0xA8F84B, 0xA8F96E, 0xA8F9EB, 0xA8FE0F, 0xA8FE8D, 0xAA00EC, 0xAA01C8, 0xAA06A3, 0xB58299, 0xB60C4D, 0xB66AB9, 0xB9F463, 0xC1D690,
		0xE81C4F, 0xE88BA5, 0xE8EB55, 0xEE9AFE, 0xEEA64A
	};

	for (UInt32 patchAddr : numRefsInterlockRegCalls)
	{
		SafeWrite16(patchAddr, 0x9058); // pop eax
	}

	NopFunctionCall(0x4968CF);
	NopFunctionCall(0x496925);

	// remove unnecessary call checking ref type when rendering a ref
	SafeWrite16(0xB55B89, 0x10EB);
	SafeWrite8(0xB55360, 0xC3);


	// remove shadow scene critical sections
//	SafeWrite32(0xB5D9F0, 0xCC0008C2);

	// remove some threading exterior cell loader task check that would lock up if moving around too quickly
//	SafeWrite16(0x5289D3, 0x9090);
}

