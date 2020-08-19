  
#include "nvse/PluginAPI.h"
#include "nvse/CommandTable.h"
#include "nvse/GameAPI.h"
#include "nvse/ParamInfos.h"
#include "nvse/GameObjects.h"
#include "main.h"
#include <string>

IDebugLog gLog("NVTF.log");
HANDLE MyHandle;

extern "C" {
	BOOL WINAPI DllMain(
		HANDLE  hDllHandle,
		DWORD   dwReason,
		LPVOID  lpreserved
	)
	{
		switch (dwReason) {
		case DLL_PROCESS_ATTACH:
			MyHandle = hDllHandle;
			break;
		}
		return TRUE;
	}

	bool NVSEPlugin_Query(const NVSEInterface* nvse, PluginInfo* info)
	{
		info->name = "NVTF";
		info->version = 7;
		info->infoVersion = PluginInfo::kInfoVersion;

		// version checks
		if (nvse->nvseVersion < NVSE_VERSION_INTEGER)
		{
			_ERROR("NVSE version too old (got %08X expected at least %08X)", nvse->nvseVersion, NVSE_VERSION_INTEGER);
			return false;
		}

		if (!nvse->isEditor)
		{
			if (nvse->runtimeVersion < RUNTIME_VERSION_1_4_0_525)
			{
				_ERROR("incorrect runtime version (got %08X need at least %08X)", nvse->runtimeVersion, RUNTIME_VERSION_1_4_0_525);
				return false;
			}

			if (nvse->isNogore)
			{
				_ERROR("incorrect runtime edition (got %08X need %08X (standard))", nvse->isNogore, 0);
				return false;
			}
		}
		else return false;

		return true;
	}

	bool NVSEPlugin_Load(const NVSEInterface* nvse)
	{
		_MESSAGE("Base Address %lx", (UInt32)MyHandle);
		_MESSAGE("NVTF Version: %u", 6);
		char iniDir[MAX_PATH];

		GetModuleFileName(GetModuleHandle(NULL), iniDir, MAX_PATH);
		strcpy((char*)(strrchr(iniDir, '\\') + 1), "Data\\NVSE\\Plugins\\NVTF.ini");
		_MESSAGE("%s", iniDir);
		g_bGTCFix = GetPrivateProfileInt("Main", "bGTCFix", 0, iniDir);
		g_bAllowBrightnessChangeWindowed = GetPrivateProfileInt("Main", "bAllowBrightnessChangeWindowed", 0, iniDir);
		g_bFastExit = GetPrivateProfileInt("Main", "bFastExit", 1, iniDir);
		g_bInlineStuff = GetPrivateProfileInt("Main", "bInlineCommonFunctions", 0, iniDir);
		g_bSpinCriticalSections = GetPrivateProfileInt("Main", "bSpinCriticalSections", 0, iniDir);
		g_bEnableExperimentalHooks = GetPrivateProfileInt("Main", "bEnableExperimentalHooks", 0, iniDir);
		g_bModifyDirectXBehavior = GetPrivateProfileInt("Main", "bModifyDirectXBehavior", 0, iniDir);
		g_bRedoHashtables = GetPrivateProfileInt("Main", "bRedoHashtables", 0, iniDir);
		if (g_bGTCFix <= 0 && g_bFastExit <= 0 && g_bInlineStuff <= 0 && g_bSpinCriticalSections <= 0 && g_bEnableExperimentalHooks <= 0 && g_bModifyDirectXBehavior <= 0 && g_bAllowBrightnessChangeWindowed <= 0 && g_bRedoHashtables <= 0) return false;
		char floatbuffer[0x40];
		g_iSpinCount = GetPrivateProfileInt("CS", "iSpinCount", -1, iniDir);
		g_bFPSFix = GetPrivateProfileInt("GTC", "bFPSFix", 0, iniDir);
		g_iMaxFPS = GetPrivateProfileInt("FPSFix", "iMaxFPSTolerance", 59, iniDir);
		g_iMinFPS = GetPrivateProfileInt("FPSFix", "iMinFPSTolerance", 20, iniDir);
		g_bfMaxTime = GetPrivateProfileInt("FPSFix", "bfMaxTime", 1, iniDir);
		GetPrivateProfileString("FPSFix", "fDialogFixMult", "2.0000", floatbuffer, 0x3F, iniDir);
		g_bResizeHashtables = GetPrivateProfileInt("Hashtables", "bResizeHashtables", 1, iniDir);
		g_bReplaceHashingAlgorithm = GetPrivateProfileInt("Hashtables", "bReplaceHashingAlgorithm", 1, iniDir);
		g_iDialogFixMult = atof(floatbuffer);
		g_bRemoveRCSafeGuard = GetPrivateProfileInt("Experimental", "bRemoveRCSafeGuard", 0, iniDir);
		g_bRemove0x80SafeGuard = GetPrivateProfileInt("Experimental", "bRemove0x80SafeGuard", 0, iniDir);
		g_bSpiderHandsFix = GetPrivateProfileInt("Experimental", "bSpiderHandsFix", 0, iniDir);
		g_bToggleTripleBuffering = GetPrivateProfileInt("DirectX", "bToggleTripleBuffering", 0, iniDir);
		g_bForceD3D9Ex = GetPrivateProfileInt("DirectX", "bUseD3D9Ex", 0, iniDir);
		g_bD3D9ManageResources = GetPrivateProfileInt("DirectX", "bD3D9ManageResources", 0, iniDir);
		g_iNumBackBuffers = GetPrivateProfileInt("DirectX", "iNumBackBuffers", 2, iniDir);
		g_bAlternateGTCFix = GetPrivateProfileInt("GTC", "bAlternateGTCFix", 0, iniDir);
		g_bUseFlipExSwapMode = GetPrivateProfileInt("D3D9Ex", "bUseFlipExSwapMode", 0, iniDir);
		g_bUseDynamicResources = GetPrivateProfileInt("D3D9Ex", "bUseDynamicResources", 0, iniDir);
		g_bHeavyInlines = GetPrivateProfileInt("Inlines", "bHeavyInlines", 1, iniDir);
		g_bLightInlines = GetPrivateProfileInt("Inlines", "bLightInlines", 0, iniDir);

		DoPatches();
		return true;
	}

};
