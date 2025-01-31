#pragma once
#include "direct3dhooks.h"
#include "d3dx9.h"
#include "nvse/SafeWrite.h"

#pragma comment (lib, "d3dx9.lib")

int g_bAllowDirectXDebugging = 0;
int g_bToggleTripleBuffering = 0;
int g_bUseDefaultPoolForTextures = 0;

namespace D3DHooks {

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

	void Init()
	{
		if (g_bAllowDirectXDebugging)
			SafeWriteBuf(0x09F9968, "\xC2\x04\x00\xCC\xCC\xCC", 6);

		if (g_bToggleTripleBuffering)
			SafeWrite8(0x1189464, 2);

		if (g_bUseDefaultPoolForTextures) {
			SafeWrite32(0xFDF3FC, (UInt32)(CreateTextureFromFileInMemoryHookForD3D9));
			SafeWrite32(0xFDF400, (UInt32)(CreateCubeTextureFromFileInMemoryHookForD3D9));
			SafeWrite32(0xFDF404, (UInt32)(CreateVolumeTextureFromFileInMemoryHookForD3D9));
		}
		
	}
}