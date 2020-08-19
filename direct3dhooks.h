#pragma once
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment (lib, "d3d9.lib")
#pragma comment (lib, "d3dx9.lib")
#pragma comment (lib, "Gdiplus.lib")
HWND foreWindow = NULL;

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
		//printf("get fucked");
		return 0;
	}

	HRESULT D3DAPI hk_CreateDeviceEx(IDirect3D9Ex* This, UINT Adapter, D3DDEVTYPE DeviceType, HWND hFocusWindow, DWORD BehaviorFlags, D3DPRESENT_PARAMETERS* pPresentationParameters, D3DDISPLAYMODEEX* displayMod, IDirect3DDevice9Ex** ppReturnedDeviceInterface)
	{
		HRESULT hr;
		//pPresentationParameters->SwapEffect = D3DSWAPEFFECT_DISCARD;
		//BehaviorFlags &= ~D3DCREATE_MULTITHREADED;
		//BehaviorFlags &= ~D3DCREATE_MULTITHREADED;

		if (g_bToggleTripleBuffering) pPresentationParameters->BackBufferCount = (UINT)g_iNumBackBuffers;
		if (pPresentationParameters->Windowed) foreWindow = hFocusWindow ? hFocusWindow : pPresentationParameters->hDeviceWindow;
		//BehaviorFlags |= D3DCREATE_NOWINDOWCHANGES;
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
		//_MESSAGE("present vtable: 0x%X", **(UINT32 * *)ppReturnedDeviceInterface);
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



	HRESULT __stdcall hk_CreateTexture(LPDIRECT3DDEVICE9 pDevice, UINT Width, UINT Height, UINT Levels, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DTexture9** ppTexture, HANDLE* pSharedHandle)
	{
		//Pool = IsD3D9ExAvailable() == 0 ? Pool : D3DPOOL_DEFAULT;
		Usage &= ~D3DUSAGE_DYNAMIC;
		return pDevice->CreateTexture(Width, Height, Levels, Usage, Format, Pool, ppTexture, pSharedHandle);
	}


	HRESULT __stdcall hk_CreateVertexBuffer(LPDIRECT3DDEVICE9 pDevice, UINT Length, DWORD Usage, DWORD FVF, D3DPOOL Pool, IDirect3DVertexBuffer9** ppVertexBuffer, HANDLE* pSharedHandle)
	{
		//_MESSAGE("vtable here: %X", *(UINT * )ppVertexBuffer);
		//Pool = IsD3D9ExAvailable() == 0 ? Pool : D3DPOOL_DEFAULT;
		Usage &= ~D3DUSAGE_SOFTWAREPROCESSING;
		Usage |= D3DUSAGE_DYNAMIC;
		Usage |= D3DUSAGE_WRITEONLY;

		HRESULT hr = pDevice->CreateVertexBuffer(Length, Usage, FVF, Pool, ppVertexBuffer, pSharedHandle);
		//(*ppVertexBuffer)->Lock(1, 1, NULL, 1);
		return hr;
	}
	HRESULT __stdcall hk_CreateIndexBuffer(LPDIRECT3DDEVICE9 pDevice, UINT Length, DWORD Usage, D3DFORMAT Format, D3DPOOL Pool, IDirect3DIndexBuffer9** ppIndexBuffer, HANDLE* pSharedHandle)
	{
		Usage &= ~D3DUSAGE_SOFTWAREPROCESSING;
		Usage |= D3DUSAGE_DYNAMIC;
		Usage |=  D3DUSAGE_WRITEONLY;
		HRESULT hr = pDevice->CreateIndexBuffer(Length, Usage, Format, Pool, ppIndexBuffer, pSharedHandle);
		//_MESSAGE("vtable here: %X", *(UINT*)ppIndexBuffer);
		return hr;
	}
	HRESULT __stdcall hk_VertexBufferLock(IDirect3DVertexBuffer9* ppVertexBuffer, UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags)
	{
		Flags |= D3DLOCK_NOOVERWRITE;
		Flags |= D3DLOCK_DISCARD;
		Flags |= D3DLOCK_NOSYSLOCK;
		HRESULT hr = ppVertexBuffer->Lock(OffsetToLock, SizeToLock, ppbData, Flags);
		return hr;
	}
	HRESULT __stdcall hk_IndexBufferLock(IDirect3DIndexBuffer9* ppIndexBuffer, UINT OffsetToLock, UINT SizeToLock, void** ppbData, DWORD Flags)
	{
		//_MESSAGE("flegs: 0%X", Flags);
		Flags |= D3DLOCK_NOOVERWRITE;
		Flags |= D3DLOCK_DISCARD;
		Flags |= D3DLOCK_NOSYSLOCK;
		HRESULT hr = ppIndexBuffer->Lock(OffsetToLock, SizeToLock, ppbData, Flags);
		return hr;
	}
	__declspec(naked) void AsmHandleCreateTextureHook()
	{
		__asm {
			pop edx
			push ecx
			push eax
			push edx
			jmp hk_CreateTexture
		}
	}



	__declspec(naked) void AsmHandleCreateVertexBufferHook()
	{
		__asm {
			pop eax
			push edi
			push esi
			push ecx
			push eax
			jmp hk_CreateVertexBuffer
		}
	}

	__declspec(naked) void AsmHandleVertexBufferLock()
	{
		__asm {
			mov dword ptr [esp+0x24], 0
			jmp hk_VertexBufferLock
		}
	}
	__declspec(naked) void AsmHandleIndexBufferLock()
	{
		__asm {
			mov dword ptr[esp + 0x58], 0
			jmp hk_IndexBufferLock
		}
	}

	HRESULT __stdcall CreateTextureFromFileInMemoryHookForD3D9(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, LPDIRECT3DTEXTURE9* ppTexture) {

		return D3DXCreateTextureFromFileInMemoryEx(pDevice, pSrcData, SrcDataSize, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, ppTexture);
	}

	HRESULT __stdcall CreateTextureFromFileInMemoryHookForD3D9Ex(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, LPDIRECT3DTEXTURE9* ppTexture) {

		return D3DXCreateTextureFromFileInMemoryEx(pDevice, pSrcData, SrcDataSize, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, ppTexture);
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
	

	void UseD3D9xPatchMemory(int ForceD3D9Ex, int ToggleDynamicResources)																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																																										
	{
		g_iNumBackBuffers = g_iNumBackBuffers > 4 ? 4 : (g_iNumBackBuffers < 1 ? 1 : g_iNumBackBuffers);

		//SafeWriteBuf(0xE69482, "\x90\x90\x90\x90\x90\x90\x90", 7);
		WriteRelJump(0xE731C1, (UInt32)(AsmHandleDirectXExCreation));
		SafeWriteBuf(0xE731C6, "\x90\x90\x90\x90\x90", 5);
		SafeWrite16(0x0E73191, 0x9090);
		WriteRelCall(0xE76215, (uintptr_t)(hk_Direct3DCreate9));

		
		if (ForceD3D9Ex)
		{

			SafeWrite16(0xE94E87, 0xB890);
			SafeWrite32(0xE94E89, (uintptr_t)AsmHandleCreateVertexBufferHook);
			WriteRelCall(0xE8F42B, (uintptr_t)hk_CreateIndexBuffer);
			SafeWrite8(0xE8F2A0, 0xBA);
			SafeWrite32(0xE8F2A1, (uintptr_t)hk_CreateIndexBuffer);
			/*//VertexLock
			SafeWrite32(0xE8C014, 0xB8909090);
			SafeWrite32(0xE8C018, (uintptr_t)AsmHandleVertexBufferLock);
			//Index Lock
			SafeWrite32(0xE8F2FD, 0xB8909090);
			SafeWrite32(0xE8F301, (uintptr_t)AsmHandleIndexBufferLock);*/
			if (!ToggleDynamicResources)
			{
				SafeWriteBuf(0xE680D7, "\xB3\x00\x90", 3);
				SafeWriteBuf(0xE6835F, "\xB3\x00\x90", 3);
				SafeWriteBuf(0xE8AA93, "\xB3\x00\x90", 3);
				//WriteRelCall(0xE68DCD, (UInt32)(CreateTextureFromFileInMemoryHookForD3D9));
				SafeWriteBuf(0xE73191, "\x90\x90", 2);
				WriteRelCall(0xE68388, (UInt32)AsmHandleCreateTextureHook);
				SafeWrite32(0xE6838D, 0x90909090);
				return;
			}

			//WriteRelCall(0x00E68DCD, (UInt32)(CreateTextureFromFileInMemoryHookForD3D9Ex));
		}
		else
		{

			WriteRelCall(0x00E68DCD, (UInt32)(CreateTextureFromFileInMemoryHookForD3D9));
		}
	}
}