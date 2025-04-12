#include "D3DHooks.hpp"
#include "INIUtils.hpp"

#include "Game/Gamebryo/NiDX9Renderer.hpp"

#include "external/Sig/Sig.hpp"

#include "d3dx9.h"
#include "psapi.h"

#pragma comment (lib, "d3dx9.lib")

namespace D3DHooks {
	namespace Setting {
		bool bAllowDirectXDebugging		= false;
		bool bToggleTripleBuffering		= false;
		bool bUseDefaultPoolForTextures = false;
		bool bUseFlipModel				= false;
	}

	namespace FlipModel {

		bool bHasSetShim = false;

		constexpr const char* pCategory = "HookCache";
		constexpr const char* pSwapChainResetAddr = "SwapChainResetAddr";
		constexpr const char* pDirectFlipModeMatchAddr = "DirectFlipModeMatchAddr";


		CallDetour kDirectFlipModeMatchDetour;
		struct _ADAPTERINFO {
			char unk0[16];
			char cDisplayName[32];
		};

		class CEnum {
		public:
			_ADAPTERINFO* DirectFlipModeMatchHook(_ADAPTERINFO* apAdapterInfo, uint32_t a3, uint32_t auiHeight, uint32_t auiWidth, HMONITOR ahMonitor, uint32_t uiFlags, struct D3DDISPLAYMODEEX* apDisplayMode) {
				_ADAPTERINFO* pResult = ThisCall<_ADAPTERINFO*>(kDirectFlipModeMatchDetour.GetOverwrittenAddr(), this, apAdapterInfo, a3, auiHeight, auiWidth, ahMonitor, uiFlags, apDisplayMode);
				// In all of my tests, result is null
				if (!pResult && apAdapterInfo)
					return apAdapterInfo;

				return pResult;
			}
		};

		bool GetRegistryString(HKEY key, const char* name, char* buffer, SIZE_T bufferSize) {
			return RegQueryValueExA(key, name, nullptr, nullptr, (BYTE*)buffer, &bufferSize) == ERROR_SUCCESS;
		}

		uint32_t __declspec(noinline) GetWindowsBuild() {
			char cBuildNumber[8];
			uint32_t uiBuildNumberInt = 0;
			HKEY hKey;
			if (RegOpenKeyEx(HKEY_LOCAL_MACHINE, "SOFTWARE\\Microsoft\\Windows NT\\CurrentVersion", 0, KEY_READ, &hKey) == ERROR_SUCCESS) {
				GetRegistryString(hKey, "CurrentBuild", cBuildNumber, sizeof(cBuildNumber));
				uiBuildNumberInt = atol(cBuildNumber);

				RegCloseKey(hKey);
			}

			return uiBuildNumberInt;
		}

		bool __declspec(noinline) GetSysDirPath(char* apPath, size_t auiSize) {
			if (!apPath || !auiSize)
				return false;

			char cSysDirPath[MAX_PATH] = {};
			if (GetSystemDirectory(cSysDirPath, sizeof(cSysDirPath))) {
				sprintf_s(apPath, auiSize, "%s\\d3d9.dll", cSysDirPath);
				return true;
			}

			return false;
		}

		bool __declspec(noinline) ReadCache(const char* apPath, uintptr_t& auiSwapChainResetAddr, uintptr_t& auiDirectFlipModeMatchAddr) {
			if (!apPath || !apPath[0])
				return false;

			char cSwapChainResetAddr[9] = {};
			char cDirectFlipModeMatchAddr[9] = {};
			GetPrivateProfileString(pCategory, pSwapChainResetAddr, "O", cSwapChainResetAddr, sizeof(cSwapChainResetAddr), apPath);
			GetPrivateProfileString(pCategory, pDirectFlipModeMatchAddr, "O", cDirectFlipModeMatchAddr, sizeof(cDirectFlipModeMatchAddr), apPath);
			if (cSwapChainResetAddr[0] != 'O')
				auiSwapChainResetAddr = strtoul(cSwapChainResetAddr, nullptr, 16);

			if (cDirectFlipModeMatchAddr[0] != 'O')
				auiDirectFlipModeMatchAddr = strtoul(cDirectFlipModeMatchAddr, nullptr, 16);

			return (auiSwapChainResetAddr || auiDirectFlipModeMatchAddr) != 0;
		}

		bool __declspec(noinline) WriteCache(const char* apPath, uintptr_t auiSwapChainResetAddr, uintptr_t auiDirectFlipModeMatchAddr) {
			if (!apPath || !apPath[0])
				return false;

			char cSwapChainResetAddr[9] = {};
			char cDirectFlipModeMatchAddr[9] = {};
			if (auiSwapChainResetAddr && auiDirectFlipModeMatchAddr) {
				sprintf_s(cSwapChainResetAddr, "%08X", auiSwapChainResetAddr);
				sprintf_s(cDirectFlipModeMatchAddr, "%08X", auiDirectFlipModeMatchAddr);
			}
			else {
				cSwapChainResetAddr[0] = 'O';
				cDirectFlipModeMatchAddr[0] = 'O';
			}
			WritePrivateProfileString(pCategory, pSwapChainResetAddr, cSwapChainResetAddr, apPath);
			WritePrivateProfileString(pCategory, pDirectFlipModeMatchAddr, cDirectFlipModeMatchAddr, apPath);
			return true;
		}

		bool IsD3D9CreateHook() {
			bool bResult = NiDX9Renderer::IsD3D9Create();
			bool bWindowed = !CdeclCall<bool>(0x446E10);
			if (bWindowed && bResult && !bHasSetShim) {
				char cD3D9Path[MAX_PATH];
				GetSysDirPath(cD3D9Path, sizeof(cD3D9Path));

				HMODULE hD3D9 = GetModuleHandle(cD3D9Path);

				// Will fail on 1st call if D3D9 spoofers (like ReShade) are loaded
				// Game calls this function for the second time when getting device info - by that time the real D3D9 is usually already loaded
				if (!hD3D9)
					return bResult;

				// Will fail on DXVK and old Windows versions - this allows us to easily ignore those cases
				constexpr uint32_t uiMaximizedWindowedShimOrdinal = 17;
				if (!GetProcAddress(hD3D9, MAKEINTRESOURCE(uiMaximizedWindowedShimOrdinal)))
					return bResult;

				MODULEINFO kModInfo;
				if (!GetModuleInformation(GetCurrentProcess(), hD3D9, &kModInfo, sizeof(kModInfo))) {
					MessageBox(nullptr, "Failed to get d3d9.dll info", "Error", MB_OK | MB_ICONERROR);
					return bResult;
				}

				// W11 and W10 DLLs differ, so we need to know which signature to use
				uint32_t uiWindowsBuild = GetWindowsBuild();
				if (uiWindowsBuild == 0) {
					MessageBox(nullptr, "Failed to get Windows build number", "Error", MB_OK);
					return bResult;
				}

				const char* pINIPath = INIUtils::GetINIPath();
				uintptr_t uiSwapChainResetAddr		= 0;
				uintptr_t uiDirectFlipModeMatchAddr	= 0;

				ReadCache(pINIPath, uiSwapChainResetAddr, uiDirectFlipModeMatchAddr);

				bool bIsWindows11 = uiWindowsBuild >= 22000;
				bool bSuccess = true;
				// Remove windowed check in CSwapChain::Reset
				// This enables Flip Model, but doesn't allow the window to get hardware presentation - solved in the next hook
				{
					constexpr const char* W10 = "83 BB ?? ?? ?? ?? ?? 0F 84 ?? ?? ?? ?? 83 BB ?? ?? ?? ?? ?? 0F 84";
					constexpr const char* W11 = "0F 85 ? ? ? ? 8B 4F ? 83 B9";

					if (uiSwapChainResetAddr) {
						const void* pStart = reinterpret_cast<void*>(uintptr_t(hD3D9) + uiSwapChainResetAddr);
						uiSwapChainResetAddr = (uintptr_t)Sig::find(pStart, 0x50, bIsWindows11 ? W11 : W10);
					}
					
					if (!uiSwapChainResetAddr)
						uiSwapChainResetAddr = (uintptr_t)Sig::find(hD3D9, kModInfo.SizeOfImage, bIsWindows11 ? W11 : W10);
					
					if (uiSwapChainResetAddr) {
						if (bIsWindows11)
							PatchMemoryNop(uiSwapChainResetAddr, 6);
						else
							SafeWrite8(uiSwapChainResetAddr + 6, 1);
					}
					else {
						MessageBox(nullptr, "Failed to find CSwapChain::Reset", "Error", MB_OK);
						bSuccess = false;
					}
				}

				// Return given adapter info if window res != screen res (I think) in CEnum::DirectFlipModeMatch
				// This enables DirectFlip
				{
					constexpr const char* W10 = "E8 ? ? ? ? 5F 5E 5B 5D C2 ? ? 8A D3";
					constexpr const char* W11 = "E8 ? ? ? ? 5F 5E 5B C9 C2 ? ? 88 45";
					if (uiDirectFlipModeMatchAddr) {
						const void* pStart = reinterpret_cast<void*>(uintptr_t(hD3D9) + uiDirectFlipModeMatchAddr);
						uiDirectFlipModeMatchAddr = (uintptr_t)Sig::find(pStart, 0x50, bIsWindows11 ? W11 : W10);
					}

					if (!uiDirectFlipModeMatchAddr)
						uiDirectFlipModeMatchAddr = (uintptr_t)Sig::find(hD3D9, kModInfo.SizeOfImage, bIsWindows11 ? W11 : W10);

					if (uiDirectFlipModeMatchAddr) {
						kDirectFlipModeMatchDetour.ReplaceCallEx(uiDirectFlipModeMatchAddr, &CEnum::DirectFlipModeMatchHook);
					}
					else {
						MessageBox(nullptr, "Failed to find CEnum::DirectFlipModeMatch", "Error", MB_OK);
						bSuccess = false;
					}
				}

				bHasSetShim = bSuccess;

				if (bSuccess)
					WriteCache(pINIPath, uiSwapChainResetAddr - uintptr_t(hD3D9), uiDirectFlipModeMatchAddr - uintptr_t(hD3D9));
				else
					WriteCache(pINIPath, 0, 0);
			}

			return bResult;
		}
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
		Setting::bUseFlipModel				= GetPrivateProfileInt("DirectX", "bUseFlipModel", 0, iniPath);
	}

	void InitHooks() {
		if (Setting::bAllowDirectXDebugging) [[unlikely]]
			SafeWriteBuf(0x09F9968, "\xC2\x04\x00\xCC\xCC\xCC", 6);

		if (Setting::bToggleTripleBuffering) [[likely]]
			SafeWrite8(0x1189464, 2);

		if (Setting::bUseDefaultPoolForTextures) [[likely]] {
			SafeWrite32(0xFDF3FC, uint32_t(D3DXCreateTextureFromFileInMemoryHook));
			SafeWrite32(0xFDF400, uint32_t(D3DXCreateCubeTextureFromFileInMemoryHook));
			SafeWrite32(0xFDF404, uint32_t(D3DXCreateVolumeTextureFromFileInMemoryHook));
		}
	}

	void InitEarlyHooks() {
		if (Setting::bUseFlipModel) {
			ReplaceCall(0xE69470, FlipModel::IsD3D9CreateHook); // NiDX9Renderer::CreateDirect3D9
			ReplaceCall(0xE69A69, FlipModel::IsD3D9CreateHook); // NiDX9SystemDesc::Get
		}
	}
}