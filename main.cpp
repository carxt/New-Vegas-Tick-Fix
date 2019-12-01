#include "nvse/PluginAPI.h"
#include "nvse/CommandTable.h"
#include "nvse/GameAPI.h"
#include "nvse/ParamInfos.h"
#include "nvse/GameObjects.h"
#include <string>
#include "main.h"


IDebugLog gLog("NVTF.log");


extern "C" {

	bool NVSEPlugin_Query(const NVSEInterface* nvse, PluginInfo* info)
	{
		info->name = "NVTF";
		info->version = 2;
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

bool NVSEPlugin_Load(const NVSEInterface * nvse)
{
	_MESSAGE("load");


	char iniDir[MAX_PATH];
	GetModuleFileName(GetModuleHandle(NULL), iniDir, MAX_PATH);
	strcpy((char*)(strrchr(iniDir, '\\') + 1), "Data\\NVSE\\Plugins\\NVTF.ini");
	_MESSAGE("%s", iniDir);
	 g_bGTCFix = GetPrivateProfileInt("Main", "bGTCFix", 0, iniDir);
	 g_bFastExit = GetPrivateProfileInt("Main", "bFastExit", 1, iniDir);
	 g_bInlineStuff = GetPrivateProfileInt("Main", "bInlineCommonFunctions", 0, iniDir);
	 g_bFPSFix = GetPrivateProfileInt("GTC", "bFPSFix", 0, iniDir);

	if (g_bGTCFix <= 0 && g_bFastExit <= 0 && g_bInlineStuff <= 0 && g_bFPSFix == 0) return false;
	
	DoPatches();


	return true;
}

};
