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

		
	}


}

struct NiFrustum
{
	float l;
	float r;
	float t;
	float b;
	float n;
	float f;
	UInt8 o;
	UInt8 pad19[3];
};


struct NiCameraAlt //Defined here because the one in NVSE is wrong.
{
	UInt8			undefinedDataStruct[156];
	float			m_aafWorldToCam[4][4];	// 09C
	NiFrustum		frustum;			// 0DC
	float			minNearPlaneDist;	// 0F8
	float			maxFarNearRatio;	// 0FC
	float			m_kPort[4];			// 100
	float			LODAdjust;			// 110
};

void __fastcall hook_WaterCullCheck(
	uintptr_t* thisObj,
	uintptr_t* unusedEDX,
	uintptr_t* multiboundNode,
	float x,
	float y,
	float z,
	uintptr_t* bsCullingProcess)
{
	uintptr_t* bsMultiBoundAABB; 
	uintptr_t iCullMode; 
	uintptr_t portalGraph; 
	DWORD* unkListChildMaybe; 
	int unkListCurrentCulledMaybe; 
	char shouldUseCull;
	bsMultiBoundAABB = *(uintptr_t**)(multiboundNode[0x2B] + 0xC);
	iCullMode = bsCullingProcess[0x24];
	if (iCullMode != 0x1 && iCullMode != 0x3 && !bsMultiBoundAABB[0x2]) //bsMultiBoundAABB[2] checks the cull result
	{
		if (*((WORD*)multiboundNode + 84))
		{
			shouldUseCull = true;
			*(DWORD*)(*(DWORD*)(multiboundNode[0x2B] + 0xC) + 8) = (shouldUseCull == false) + 1;
			//shouldUseCull = (*(int(__thiscall**)(uintptr_t*, uintptr_t*))(*bsMultiBoundAABB + 0x9C))(bsMultiBoundAABB, bsCullingProcess + 0xB);            // WithinFrustum
			if (shouldUseCull)
			{
				portalGraph = thisObj[0x78];

				if (portalGraph)
				{
					unkListChildMaybe = *(DWORD**)(portalGraph + 8);
					if (unkListChildMaybe)
					{
						do
						{
							unkListCurrentCulledMaybe = unkListChildMaybe[2];
							unkListChildMaybe = (DWORD*)*unkListChildMaybe;
						} while ((!unkListCurrentCulledMaybe
							|| *(DWORD*)(unkListCurrentCulledMaybe + 0x40) != 1
							|| ThisStdCall<bool>(0xC47150,
								multiboundNode,
								x,
								y,
								z,
								bsCullingProcess,
								unkListCurrentCulledMaybe))
							&& unkListChildMaybe);
					}
				}
			}
		}
	}
}

NiFrustum fakeReturnedFrustum;
NiFrustum* __fastcall FakeFrustumHook(NiCameraAlt* thisObj) {
	fakeReturnedFrustum = thisObj->frustum;
	float frustumValMax = 2.53864789566; // tan(137.5/2 deg), so we go with the assumption no one is going to use stupid fov values
	float frustumRatio = 1;
	if ((fabs(fakeReturnedFrustum.r) > FLT_EPSILON) && (fabs(fakeReturnedFrustum.t) > FLT_EPSILON)) { //Juuuuuuuust in case
		frustumRatio = fabs(fakeReturnedFrustum.r) / fabs(fakeReturnedFrustum.t);
	}
	if (fakeReturnedFrustum.t < frustumValMax) fakeReturnedFrustum.t = frustumValMax;
	if (fakeReturnedFrustum.b > -frustumValMax) fakeReturnedFrustum.b = -frustumValMax;
	if (fakeReturnedFrustum.l > -frustumValMax * frustumRatio) fakeReturnedFrustum.l = -frustumValMax * frustumRatio;
	if (fakeReturnedFrustum.r > frustumValMax * frustumRatio) fakeReturnedFrustum.r = frustumValMax * frustumRatio;
	return &fakeReturnedFrustum;

}