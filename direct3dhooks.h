#pragma once
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "Gdiplus.lib")
//ODT D3D9Ex
//#include "D3D9Device.h"
#define HAS_D3D9EX_CONTAINER_ODT 1

extern HWND foreWindow;
extern int g_bToggleTripleBuffering;
extern int g_bUseFlipExSwapMode;
extern int g_bD3D9ManageResources;
extern int g_iNumBackBuffers;
extern int g_bForceD3D9Ex;
extern int g_bUseDynamicBuffers;
namespace D3DHooks {
	extern bool* g_DXEx;
	extern bool r_d3d9ex;
	extern IDirect3D9** g_D3Device;
	extern D3DGAMMARAMP StartingGammaRamp;
	extern bool SetGammaRampInit;
	bool IsD3D9ExAvailable();
	IDirect3D9* D3DAPI CreateD3D9(bool UseD3D9Ex);
	DWORD __cdecl hk_Direct3DCreate9(bool UseEx);
	HRESULT D3DAPI hk_CreateDeviceEx(IDirect3D9Ex* This, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* displayMod, IDirect3DDevice9Ex** ppReturnedDeviceInterface);
	void AsmHandleDirectXExCreation();
	void hk_SetGammaRamp(LPDIRECT3DDEVICE9 pDevice, UINT iSwapChain, DWORD Flags, const D3DGAMMARAMP* pRamp);
	void UseD3D9xPatchMemory(int ForceD3D9Ex, int ToggleDynamicResources);
}