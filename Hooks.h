#pragma once

UInt32 GetInit = 0;
/*
int __cdecl zlib_inflate_init_hook(void* stream, const char* a2, int stream_size)
{
	return ((int(__stdcall*)(void*, int, const char*, int))(GetInit))(stream, 15, "1.2.11", stream_size);
}

void HookZLib()
{
	HMODULE zlibdll = LoadLibrary("zlibwapi.dll");


	if (!zlibdll)
	{
		_MESSAGE("zlib dll not found");
	}
	else {
		GetInit  = (DWORD)GetProcAddress(zlibdll, "inflateInit2_");
		UInt32  GetInflate = (DWORD)GetProcAddress(zlibdll, "inflate");
		
		if (!GetInflate || !GetInit) { 
			_MESSAGE("Failed to get zlib_inflate or zlib_init");
			FreeLibrary(zlibdll);
			 }
		else{
			WriteRelCall(0x47434F, GetInflate);
			SafeWriteBuf(0x474354, "\x90\x90\x90", 3);
			WriteRelCall(0xAFC1F4, GetInflate);
			SafeWriteBuf(0x0AFC1F9, "\x90\x90\x90", 3);
			WriteRelCall(0x04742AC, (UInt32)zlib_inflate_init_hook);
			WriteRelCall(0x0AFC537, (UInt32)zlib_inflate_init_hook);

		}
	}
}*/
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
__declspec (naked) void HookCSDebug()
{
	static const char* dec = "0x%x";
	{
		__asm
		{
			push dec
			call _MESSAGE
			add esp, 4
			jmp EnterCriticalSection
		}
	}

}

__declspec (naked) void OurFastcall()
{
	__asm
	{
		movsx eax, cx
		ret
	}
}
__declspec (naked) void FastTESMobile_GETBASEPROCESS()
{
	__asm
	{
		mov eax, dword ptr ds : [ecx + 0x68]
		ret
	}
}
__declspec (naked) void FastGetThis0x28()
{
	__asm
	{
		mov eax, dword ptr ds : [ecx + 0x28]
		ret
	}
}
__declspec (naked) void FastGetThis0x8()
{
	__asm
	{
		mov eax, dword ptr ds : [ecx + 0x8]
		ret
	}
}
__declspec (naked) void FaceGenDataGetSelfDereferenceTwice()
{
	__asm
	{
		mov eax, [ecx]
		test eax, eax
		jz done
		mov eax, [eax]
		done:
		ret
	}
}

__declspec (naked) void Inline576D30()
{
	__asm
	{
		mov ecx, [ecx + 8]
		and ecx, 0x200000
		setnz al
		ret
	}
}

__declspec (naked) void FastDereference()
{
	__asm
	{
		test ecx, ecx
		jz done
		mov eax, [ecx]
		done:
		ret
	}
}



__declspec (naked) void FastThisAndThis_4Check()
{
	__asm
	{
		mov eax, [ecx]
		test eax, eax
		jz done
		mov eax, [ecx + 4]
		test eax, eax
		done :
		setnz al
			ret
	}
}













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






bool __fastcall StringNiMap__Lookup(int ecx, int EDX, int a1, int a2)
{
	int* i = *(int**)(*(DWORD*)(ecx + 8) + 4 * (*(int(__fastcall * *)(int, int, int))(*(DWORD*)ecx + 4))(ecx, 0, a1));
	while (i) {
		if ((*(unsigned __int8(__fastcall * *)(int, int, int, int))(*(DWORD*)ecx + 8))(ecx, 0, a1, i[1]))
		{
			*(DWORD*)a2 = i[2];
			return 1;
		}
		i = (int*)*i;
	}
	return 0;
}

float __fastcall Hook595C80(float* ecx)
{
	return *ecx * *ecx + ecx[1] * ecx[1];
}

bool __fastcall DereferenceNotNull(UInt32* ecx) {
	return (*ecx != 0);
}

float* __fastcall Hook416870(float* ecx, void* edx, float a2, float a3, float a4)
{
	*ecx = a2;
	ecx[1] = a3;
	ecx[2] = a4;
	return ecx;
}



float* __fastcall Hook439EF0(float* ecx, void* edx, float* a2, float* a3)
{
	*a2 = *ecx - *a3;
	a2[1] = ecx[1] - a3[1];
	a2[2] = ecx[2] - a3[2];
	return a2;
}
/*
int __fastcall HookA59E00(DWORD* ecx, DWORD* edx,int a2)
{
	int result; // eax

	if (!(ecx[12] & 1))
		return	(*(int(__thiscall**)(DWORD, DWORD*))((UInt32)(*(DWORD*)a2 + 0x44)))(a2, ecx);
	 //((int (__fastcall*)(DWORD, DWORD*))(()))(a2, ecx);
	return 0;
}*/

unsigned int** __fastcall NiTMapBase_FreeBuckets(unsigned int** ecx)
{
	unsigned int* v1; // ST08_4
	unsigned int** result; // eax
	unsigned int  i = 0; // [esp+8h] [ebp-4h]
	unsigned int ecxplus1 = *(unsigned int*) ecx[1];
	unsigned int* ecxplus2 = ecx[2];

	for (i = 0; i < (unsigned int)ecx[1]; ++i)
	{
		while (ecx[2][i])
		{
			v1 = (unsigned int*)ecx[2][i];
			ecx[2][i] = *(DWORD*)ecx[2][i];
			((void(__thiscall*)(unsigned int**, unsigned int*)) (long) ((*ecx)[4]) )(ecx, v1);
			((void(__thiscall*)(unsigned int**, unsigned int*)) (long) ((*ecx)[6]) )(ecx, v1);
		}
	}
	ecx[3] = 0;
	return ecx;

}

DWORD __cdecl ObsidianIsRetardedHook(DWORD a1, int a2)
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
		v10 = *(void**) (((DWORD)v10) + 4);
		v11 = *(unsigned int*) ((((DWORD)v9) + 4) + 0xC);
		//v11 = *(unsigned int*) (((DWORD)v9) + 0xC);
		if (v11)
		{
			if ((BYTE)a3)
			{
				for (i = v11 - 1; i >= 0; --i)
				{
					v7 = ObsidianIsRetardedHook((DWORD)v9, i);
					if (v7 && (*(DWORD*)(v7 + 0xC)) == a2)
						return v7;
				}
			}
			else
			{
				for (j = 0; j < v11; ++j)
				{
					v5 = ObsidianIsRetardedHook((DWORD)v9, j);
					if (v5 && (*(DWORD*)(v5 + 0xC)) == a2)
						return v5;
				}
			}
		}
	}
	return 0;
}


int __fastcall HookA59E00(unsigned int* ecx, void* edx ,unsigned int* a2)
{
	
	return (!(ecx[12] & 1)) ? (*(int(__thiscall * *)(unsigned int*, unsigned int*))(*a2 + 0x44))(a2, ecx) : 0;
}

/*

int __fastcall sub_B9F480(DWORD* ecx, void* edx, int a2)
{
	int result; // eax
	int v4; // esi
	int v5; // ebp
	DWORD* v6; // edi
	DWORD* v7; // eax
	DWORD* v8; // ecx
	DWORD* v9; // edx
	int* v10; // esi
	int* v11; // esi
	int v12; // [esp+Ch] [ebp+4h]
	DWORD* dword_11FFFE4 =(DWORD*) 0x11FFFE4;
	DWORD* dword_11FFFE0 = (DWORD*)0x11FFFE0;
	DWORD* dword_11F4288 = (DWORD*)0x11F4288;
	DWORD* unk_126F554 = (DWORD*)0x126F554;


	int (__fastcall *sub_C1B4A0)(DWORD ecx, DWORD edx,volatile LONG a2) = (int(__fastcall*)(DWORD, DWORD, volatile LONG )) 0xC1B4A0;
	void(__cdecl * callWithExceptionHandling)(DWORD ecx) = (void(__cdecl*)(DWORD)) 0xC1B4A0;
	/*
		result = 1;
	if (!(dword_11FFFE4 & 1))
	{
		dword_11FFFE4 |= 1u;
		dword_11FFFE0 = 0;
		result = callWithExceptionHandling(au_re_InterlockedDecrement_65);
	}

	//The part above isn't an issue so who cares
	v4 = a2;
	if (*(DWORD*)(a2 + 192))
	{
		result = sub_B4F580(&unk_126F554);
		if (result)
		{
			result = a2 + 156;
			if (a2 != -156)
			{
				v5 = *(DWORD*)(a2 + 168);
				v12 = v5 ? *(DWORD*)(v5 + 28) : -1;
				if ((unsigned int)(v12 - 1) <= 0xB)
				{
					v6 = (DWORD*)ecx[78];
					v7 = v6;
					if (!v6)
						v7 = (DWORD*)ecx[56];
					if (v7)
					{
						while (1)
						{
							v8 = v7 + 2;
							v9 = v7;
							v7 = (DWORD*)* v7;
							if (*v8 == v4)
								break;
							if (!v7)
								goto LABEL_15;
						}
						if (v6)
							sub_B702E0(v9, ecx[78]);
						v11 = *(int**)(v4 + 32);
						if (!v11)
							v11 = &dword_11F4288;
						result = sub_B71560(ecx, v11);
					}
					else
					{
					LABEL_15:
						result = sub_4A2020(12);
						if (!(_BYTE)result || v12 == 4)
						{
							dword_11FFFE0 = v4;
							v10 = *(int**)(v4 + 32);
							if (!v10)
								v10 = &dword_11F4288;
							sub_B71560(ecx, v10);
							result = sub_C1B4A0(&dword_11FFFE0);
							dword_11FFFE0 = 0;
						}
					}
				}
			}
		}
	}
	return result;
}

*/
	/*

	void __ecxcall sub_B9F480(DWORD ecx, int a2)
	{

		if (!(dword_11FFFE4 & 1))
		{
			dword_11FFFE4 |= 1u;
			dword_11FFFE0 = 0;
			callWithExceptionHandling(au_re_InterlockedDecrement_65);
		}
		v3 = a2;
		if (*(DWORD*)(a2 + 192) && sub_B4F580(&unk_126F554) && a2 != -156)
		{
			v4 = *(DWORD*)(a2 + 168);
			v11 = v4 ? *(DWORD*)(v4 + 28) : -1;
			if ((unsigned int)(v11 - 1) <= 0xB)
			{
				v5 = *(DWORD * *)(ecx + 312);
				v6 = v5;
				if (!v5)
					v6 = *(DWORD * *)(ecx + 224);
				if (v6)
				{
					while (1)
					{
						v7 = v6 + 2;
						v8 = v6;
						v6 = (DWORD*)* v6;
						if (*v7 == v3)
							break;
						if (!v6)
							goto LABEL_15;
					}
					if (v5)
						sub_B702E0(v8, *(DWORD*)(ecx + 312));
					v10 = *(int**)(v3 + 32);
					if (!v10)
						v10 = &dword_11F4288;
					sub_B71560(ecx, v10);
				}
				else
				{
				LABEL_15:
					if (!(unsigned __int8)sub_4A2020(12) || v11 == 4)
					{
						dword_11FFFE0 = v3;
						v9 = *(int**)(v3 + 32);
						if (!v9)
							v9 = &dword_11F4288;
						sub_B71560(ecx, v9);
						sub_C1B4A0((DWORD*)(ecx + 224), (volatile LONG * *)& dword_11FFFE0);
						dword_11FFFE0 = 0;
					}
				}
			}
		}
	}*/
void HookInlines(){
	WriteRelCall(0x0AA6AC9, (UInt32)HookHotSpot1);
	WriteRelJump(0x0619F20, (UInt32)ObsidianIsRetardedHook);
	//SafeWrite32(0x10A2904, (UInt32) MemHeapAlloc);
	WriteRelJump(0x0AA946D, 0x0AA94CB);
	WriteRelJump(0x0AA92B1, 0x0AA9310);
	WriteRelJump(0xAA964E, 0x0AA96AF);
	//WriteRelJump(0xED5ABB, (UInt32)OurFastcall);
	WriteRelJump(0x08D8520, (UInt32)FastTESMobile_GETBASEPROCESS);
	//	WriteRelJump(0x045CD60, (UInt32)FastGetThis0x28);
	//	WriteRelJump(0x044DDC0, (UInt32)FastGetThis0x8);
	WriteRelJump(0xA694E0, (UInt32)InlinedA694E0);
	WriteRelJump(0x0576D30, (UInt32)Inline576D30);
	//	WriteRelJump(0x0559450, (UInt32)FastDereference);
		//WriteRelJump(0x0EC4835, (UInt32)memcmp);
	//More hooks
			//WriteRelJump(0x5CA4F0, (UInt32)DereferenceNotNull);
	WriteRelJump(0x595C80, (UInt32)Hook595C80);
	WriteRelJump(0xA1E380, (UInt32)StringNiMap__Lookup);
	WriteRelJump(0x853130, (UInt32)StringNiMap__Lookup);
	WriteRelJump(0x416870, (UInt32)Hook416870);
	WriteRelJump(0x439EF0, (UInt32)Hook439EF0);
	//WriteRelJump(0xA59E00, (UInt32)HookA59E00);
	WriteRelJump(0x0438AF0, (UInt32)NiTMapBase_FreeBuckets);
	WriteRelJump(0x0619E10, (UInt32)TESTopic_getTopicInfoByID);
	//WriteRelCall(0x40886D, (UInt32)ffabs);
	//SafeWriteBuf(0x408860, "\xD9\x44\x24\x04\xD9\xE1\xC3", 7);
	//SafeWriteBuf(0x0EC6CDE, "\xD9\x44\x24\x04\xD9\xE1\xC3", 7);

	//HookZLib();
			//WriteRelJump(0x08256D0, (UInt32)FastThisAndThis_4Check);

}