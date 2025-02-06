
#include "nvse/prefix.h"
#include "nvse/PluginAPI.h"
#include "nvse/Utilities.h"
#include <math.h>
#include "main.h"

#define NVTF_VERSION 10
extern "C" {
	BOOL WINAPI DllMain(
		HANDLE  hDllHandle,
		DWORD   dwReason,
		LPVOID  lpreserved
	)
	{
		return TRUE;
	}

	bool NVSEPlugin_Query(const NVSEInterface* nvse, PluginInfo* info)
	{
		info->name = "NVTF";
		info->version = NVTF_VERSION;
		info->infoVersion = PluginInfo::kInfoVersion;

		// version checks
		if (nvse->nvseVersion < NVSE_VERSION_INTEGER)
		{
			char text[72];
			sprintf_s(text, "NVSE version too old (got %08X expected at least %08X)", nvse->nvseVersion, NVSE_VERSION_INTEGER);
			MessageBoxA(NULL, text, "NVTF", MB_OK);
			return false;
		}

		return true;
	}

	bool NVSEPlugin_Load(const NVSEInterface* nvse)
	{
		char iniDir[MAX_PATH];
		GetModuleFileNameA(GetModuleHandle(NULL), iniDir, MAX_PATH);
		strcpy((char*)(strrchr(iniDir, '\\') + 1), "Data\\NVSE\\Plugins\\NVTF.ini");
		g_bGTCFix = GetPrivateProfileInt("Main", "bGTCFix", 0, iniDir);
		g_bFastExit = GetPrivateProfileInt("Main", "bFastExit", 1, iniDir);
		g_bEnableThreadingTweaks = GetPrivateProfileInt("Main", "bEnableThreadingTweaks", 0, iniDir);
		g_bModifyDirectXBehavior = GetPrivateProfileInt("Main", "bModifyDirectXBehavior", 0, iniDir);
		g_bRedoHashtables = GetPrivateProfileInt("Main", "bRedoHashtables", 0, iniDir);
		
		if (g_bGTCFix <= 0 && g_bFastExit <= 0 && g_bEnableThreadingTweaks <= 0 && g_bModifyDirectXBehavior <= 0 && g_bRedoHashtables <= 0) 
			return false;

		g_bFPSFix = GetPrivateProfileInt("GTC", "bFPSFix", 0, iniDir);

		g_iMaxFPS = GetPrivateProfileInt("FPSFix", "iMaxFPSTolerance", 59, iniDir);
		g_iMinFPS = GetPrivateProfileInt("FPSFix", "iMinFPSTolerance", 20, iniDir);
		g_bfMaxTime = GetPrivateProfileInt("FPSFix", "bfMaxTime", 1, iniDir);
		g_bResizeHashtables = GetPrivateProfileInt("Hashtables", "bResizeHashtables", 1, iniDir);

		g_iReplaceTextureCreationLocks = GetPrivateProfileInt("ThreadingTweaks", "iReplaceTextureCreationLocks", 1, iniDir);
		g_iReplaceGeometryPrecacheLocks = GetPrivateProfileInt("ThreadingTweaks", "iReplaceGeometryPrecacheLocks", 1, iniDir);

		// Legacy support
		{
			int iTweakRCSafeGuard = GetPrivateProfileInt("ThreadingTweaks", "iTweakRCSafeGuard", 0xFFFFFFFF, iniDir);
			int iTweakMiscRendererSafeGuard = GetPrivateProfileInt("ThreadingTweaks", "iTweakMiscRendererSafeGuard", 0xFFFFFFFF, iniDir);
			if (iTweakRCSafeGuard != 0xFFFFFFFF)
				g_iReplaceTextureCreationLocks = iTweakRCSafeGuard;

			if (iTweakMiscRendererSafeGuard != 0xFFFFFFFF)
				g_iReplaceGeometryPrecacheLocks = iTweakMiscRendererSafeGuard;
		}

		g_bTweakMiscCriticalSections= GetPrivateProfileInt("ThreadingTweaks", "bTweakMiscCriticalSections", 0, iniDir);
		g_bReplaceDeadlockCSWithWaitAndSleep = GetPrivateProfileInt("ThreadingTweaks", "bReplaceDeadlockCSWithWaitAndSleep", 0, iniDir);

		g_bSpiderHandsFix = GetPrivateProfileInt("FPSFix", "bSpiderHandsFix", 0, iniDir);

		g_bToggleTripleBuffering = GetPrivateProfileInt("DirectX", "bToggleTripleBuffering", 0, iniDir);
		g_bUseDefaultPoolForTextures = GetPrivateProfileInt("DirectX", "bUseDefaultPoolForTextures", 0, iniDir);
		g_bAllowDirectXDebugging = GetPrivateProfileInt("DirectX", "bAllowDirectXDebugging", 0, iniDir);

		g_bAlternateGTCFix = GetPrivateProfileInt("GTC", "bAlternateGTCFix", 0, iniDir);
		g_bRemoveGTCLimits = GetPrivateProfileInt("GTC", "bRemoveGTCLimits", 0, iniDir);

		char floatbuffer[0x40];
		GetPrivateProfileString("FPSFix", "fMaxTimeLowerBoundary", "0.016", floatbuffer, 0x3F, iniDir);
		g_fMaxTimeLowerBoundary = atof(floatbuffer);

		DoPatches();
		return true;
	}

};
