#include "nvse/prefix.h"
#include "nvse/nvse_version.h"
#include "nvse/PluginAPI.h"
#include <criticalsections/criticalsections.hpp>
#include "main.h"


void PrintLog(const char* arg, ...) {

}



#define NVTF_VERSION 10


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
		info->version = NVTF_VERSION;
		info->infoVersion = PluginInfo::kInfoVersion;
		//gLog.Create("NVTF.log");
		// version checks
		if (nvse->nvseVersion < NVSE_VERSION_INTEGER)
		{
			PrintLog("ERROR: NVSE version too old (got %08X expected at least %08X)", nvse->nvseVersion, NVSE_VERSION_INTEGER);
			return false;
		}

		if (!nvse->isEditor)
		{
			if (nvse->runtimeVersion < RUNTIME_VERSION_1_4_0_525)
			{
				PrintLog("ERROR: incorrect runtime version (got %08X need at least %08X)", nvse->runtimeVersion, RUNTIME_VERSION_1_4_0_525);
				return false;
			}

			if (nvse->isNogore)
			{
				PrintLog("ERROR: incorrect runtime edition (got %08X need %08X (standard))", nvse->isNogore, 0);
				return false;
			}
		}
		else return false;

		return true;
	}


	bool NVSEPlugin_Load(const NVSEInterface* nvse)
	{
		PrintLog("Base Address %lx", (UInt32)MyHandle);
		PrintLog("NVTF Version: %u", NVTF_VERSION);
		return true;
	}

};
