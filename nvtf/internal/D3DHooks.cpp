#pragma once
#include "D3DHooks.hpp"
#include <d3dx9.h>

#pragma comment (lib, "d3dx9.lib")

namespace D3DHooks {
	namespace Setting {
		bool bAllowDirectXDebugging		= false;
		bool bToggleTripleBuffering		= false;
		bool bUseDefaultPoolForTextures = false;
	}

	HRESULT __stdcall D3DXCreateCubeTextureFromFileInMemoryHook(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, LPDIRECT3DCUBETEXTURE9* ppCubeTexture) {
		return D3DXCreateCubeTextureFromFileInMemoryEx(pDevice, pSrcData, SrcDataSize, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, ppCubeTexture);
	}

	HRESULT __stdcall D3DXCreateTextureFromFileInMemoryHook(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcData, UINT SrcDataSize, LPDIRECT3DTEXTURE9* ppTexture) {
		return D3DXCreateTextureFromFileInMemoryEx(pDevice, pSrcData, SrcDataSize, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, ppTexture);
	}

	HRESULT __stdcall D3DXCreateVolumeTextureFromFileInMemoryHook(LPDIRECT3DDEVICE9 pDevice, LPCVOID pSrcFile, UINT SrcData, LPDIRECT3DVOLUMETEXTURE9* ppVolumeTexture) {
		return D3DXCreateVolumeTextureFromFileInMemoryEx(pDevice, pSrcFile, SrcData, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, ppVolumeTexture);
	}

	void ReadINI(const char* iniPath) {
		Setting::bToggleTripleBuffering		= GetPrivateProfileInt("DirectX", "bToggleTripleBuffering", 0, iniPath);
		Setting::bUseDefaultPoolForTextures	= GetPrivateProfileInt("DirectX", "bUseDefaultPoolForTextures", 0, iniPath);
		Setting::bAllowDirectXDebugging		= GetPrivateProfileInt("DirectX", "bAllowDirectXDebugging", 0, iniPath);
	}

	void InitHooks() {
		if (Setting::bAllowDirectXDebugging) [[unlikely]]
			SafeWriteBuf(0x09F9968, "\xC2\x04\x00\xCC\xCC\xCC", 6);

		if (Setting::bToggleTripleBuffering) [[likely]]
			SafeWrite8(0x1189464, 2);

		if (Setting::bUseDefaultPoolForTextures) [[likely]] {
			SafeWrite32(0xFDF3FC, (uint32_t)(D3DXCreateTextureFromFileInMemoryHook));
			SafeWrite32(0xFDF400, (uint32_t)(D3DXCreateCubeTextureFromFileInMemoryHook));
			SafeWrite32(0xFDF404, (uint32_t)(D3DXCreateVolumeTextureFromFileInMemoryHook));
		}
	}
}