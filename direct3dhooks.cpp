#pragma once
#include "direct3dhooks.h"
#include "nvse/SafeWrite.h"
#include <mutex>
HWND foreWindow = NULL;
int g_bToggleTripleBuffering = 0;
int g_bUseFlipExSwapMode = 0;
int g_bD3D9ManageResources = 0;
int g_iNumBackBuffers  = 0;
int g_bForceD3D9Ex = 0;
int g_bUseDynamicBuffers = 0;
namespace D3DHooks {
	bool* g_DXEx = (bool*)0x126F0D0;
	bool r_d3d9ex = false;
	IDirect3D9** g_D3Device = (IDirect3D9 * *)0x126F0D8;

	bool IsD3D9ExAvailable()
	{
		return r_d3d9ex;
	}

	IDirect3D9* D3DAPI CreateD3D9(bool UseD3D9Ex)
	{
		HMODULE d3d9 = GetModuleHandleA("d3d9.dll");
		UINT SDKVersion = 32;
		if (!d3d9)
			d3d9 = LoadLibraryA("d3d9.dll");

		if (!d3d9)
			return nullptr;

		auto pDirect3DCreate9 = (decltype(&Direct3DCreate9))GetProcAddress(d3d9, "Direct3DCreate9");
		auto pDirect3DCreate9Ex = (decltype(&Direct3DCreate9Ex))GetProcAddress(d3d9, "Direct3DCreate9Ex");


		IDirect3D9* d3d = nullptr;
		IDirect3D9Ex* d3dex = nullptr;

		// Check for Direct3DCreate9Ex first (user must manually enable dvar, autodetection disabled)
		if (pDirect3DCreate9Ex && UseD3D9Ex)
		{
			if (SUCCEEDED(pDirect3DCreate9Ex(SDKVersion, &d3dex)))
			{
				_MESSAGE("Using Direct3D9Ex interface\n");
				r_d3d9ex = true;
				return d3dex;
			}
		}
		_MESSAGE("Using D3D9");
		// Otherwise default to the normal Direct3DCreate9
		if (pDirect3DCreate9)
		{
			if (d3d = pDirect3DCreate9(SDKVersion))
			{
				r_d3d9ex = false;
				return d3d;
			}
		}
		r_d3d9ex = false;
		return nullptr;
	}
	DWORD __cdecl hk_Direct3DCreate9(bool UseEx)
	{

		*g_D3Device = CreateD3D9(g_bForceD3D9Ex);
		*g_DXEx = r_d3d9ex;
		if (!(*g_D3Device)) return -1;
		return 0;
	}
	D3DDISPLAYMODEEX currentDisplayMod;
	bool currentDisplayModExists = false;


	HRESULT D3DAPI hk_CreateDeviceEx(IDirect3D9Ex* This, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* displayMod, IDirect3DDevice9Ex** ppReturnedDeviceInterface)
	{
		HRESULT hr;
		if (displayMod)
		{
			currentDisplayMod = *displayMod;
			currentDisplayModExists = true;
		}
		if (g_bToggleTripleBuffering) pPresentationParameters->BackBufferCount = (UINT)g_iNumBackBuffers;
		if (pPresentationParameters->Windowed) foreWindow = hFocusWindow ? hFocusWindow : pPresentationParameters->hDeviceWindow;
		if (!IsD3D9ExAvailable()) {
			if (g_bD3D9ManageResources) BehaviorFlags |= D3DCREATE_DISABLE_DRIVER_MANAGEMENT;
			_MESSAGE("using CreateDevice for device creation");
			hr = This->CreateDevice(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, (IDirect3DDevice9 * *)ppReturnedDeviceInterface);
		}
		else {
			if (g_bUseFlipExSwapMode)
			{

				pPresentationParameters->SwapEffect = D3DSWAPEFFECT_FLIPEX;
				pPresentationParameters->Flags &= ~D3DPRESENTFLAG_LOCKABLE_BACKBUFFER;

			}
			if (g_bD3D9ManageResources) BehaviorFlags |= D3DCREATE_DISABLE_DRIVER_MANAGEMENT_EX;
			hr = This->CreateDeviceEx(Adapter, DeviceType, hFocusWindow, BehaviorFlags, pPresentationParameters, displayMod, (IDirect3DDevice9Ex * *)ppReturnedDeviceInterface);
			
			_MESSAGE("Using CreateDeviceEx for device creation\n");


		}

		return hr;
	}


	__declspec(naked) void AsmHandleDirectXExCreation()
	{
		static const uintptr_t retAddr = 0xE731CA;
		__asm {
			mov ecx, dword ptr ds : [esi + 0x5C4]
			push ecx
			push eax
			call hk_CreateDeviceEx
			jmp retAddr
		}
	}


	bool Game_GetIsMenuMode()
	{

		return ((bool(__cdecl*)())(0x0702360))();
		return true;
	}

	//replace our managed pool for DirectX9 DEFAULT pool, this is bound to cause a significant memory reduction
	HRESULT __stdcall CreateCubeTextureFromFileInMemoryHookForD3D9(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, LPDIRECT3DCUBETEXTURE9* ppCubeTexture)
	{
		return D3DXCreateCubeTextureFromFileInMemoryEx(pDevice, pSrcData, SrcDataSize, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, ppCubeTexture);
	}
	HRESULT __stdcall CreateTextureFromFileInMemoryHookForD3D9(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, LPDIRECT3DTEXTURE9* ppTexture) {

		return D3DXCreateTextureFromFileInMemoryEx(pDevice, pSrcData, SrcDataSize, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, ppTexture);
	}
	HRESULT __stdcall CreateVolumeTextureFromFileInMemoryHookForD3D9(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcFile, UINT SrcData, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture)
	{
		return D3DXCreateVolumeTextureFromFileInMemoryEx(pDevice, pSrcFile, SrcData, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, ppVolumeTexture);
	}

	bool SetGammaRampInit = false;
	D3DGAMMARAMP StartingGammaRamp;
	void hk_SetGammaRamp(LPDIRECT3DDEVICE9 pDevice, UINT iSwapChain, DWORD Flags, const D3DGAMMARAMP* pRamp)
	{
		if (!foreWindow)
		{
			pDevice->SetGammaRamp(iSwapChain, Flags, pRamp);
		}
		else
		{
			HDC Devic = ::GetDC(foreWindow);
			if (!SetGammaRampInit) { GetDeviceGammaRamp(Devic, &StartingGammaRamp); SetGammaRampInit = true; }
			SetDeviceGammaRamp(Devic, (LPVOID)pRamp);
			::ReleaseDC(foreWindow, Devic);
		}
	}


	void UseD3D9xPatchMemory()
	{
		g_iNumBackBuffers = g_iNumBackBuffers > 4 ? 4 : (g_iNumBackBuffers < 1 ? 1 : g_iNumBackBuffers);
		//SafeWriteBuf(0xE69482, "\x90\x90\x90\x90\x90\x90\x90", 7);
		WriteRelJump(0xE731C1, (UInt32)(AsmHandleDirectXExCreation));
		SafeWriteBuf(0xE731C6, "\x90\x90\x90\x90\x90", 5);
		SafeWrite16(0x0E73191, 0x9090);
		WriteRelCall(0xE76215, (uintptr_t)(hk_Direct3DCreate9));
		SafeWrite32(0xFDF3FC, (UInt32)(CreateTextureFromFileInMemoryHookForD3D9));
		SafeWrite32(0xFDF400, (UInt32)(CreateCubeTextureFromFileInMemoryHookForD3D9));
		SafeWrite32(0xFDF404, (UInt32)(CreateVolumeTextureFromFileInMemoryHookForD3D9));

		
	}
}