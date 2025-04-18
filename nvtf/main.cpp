#include <nvse_version.h>
#include <PluginAPI.h>
#include <internal/INIUtils.hpp>
#include <internal/D3DHooks.hpp>
#include <internal/HashTables.hpp>
#include <internal/ThreadingTweaks.hpp>
#include <internal/FastExit.hpp>
#include <internal/TickFix.hpp>

constexpr uint32_t		PLUGIN_VERSION = 10;
constexpr const char*	PLUGIN_NAME = "NVTF";

namespace Main {

	bool bINIRead = false;

	namespace Setting {
		bool bGTCFix				= false;
		bool bFastExit				= false;
		bool bEnableThreadingTweaks	= false;
		bool bModifyDirectXBehavior	= true;
		bool bRedoHashtables		= false;
	}

	bool ReadINI(const char* iniPath) {
		Setting::bGTCFix					= GetPrivateProfileInt("Main", "bGTCFix", 0, iniPath);
		Setting::bFastExit					= GetPrivateProfileInt("Main", "bFastExit", 1, iniPath);
		Setting::bEnableThreadingTweaks		= GetPrivateProfileInt("Main", "bEnableThreadingTweaks", 0, iniPath);
		Setting::bModifyDirectXBehavior		= GetPrivateProfileInt("Main", "bModifyDirectXBehavior", 0, iniPath);
		Setting::bRedoHashtables			= GetPrivateProfileInt("Main", "bRedoHashtables", 0, iniPath);

		if (!Setting::bGTCFix && !Setting::bFastExit && !Setting::bEnableThreadingTweaks && !Setting::bModifyDirectXBehavior && !Setting::bRedoHashtables) [[unlikely]]
			return false;

		TickFix::ReadINI(iniPath);
		HashTables::ReadINI(iniPath);
		ThreadingTweaks::ReadINI(iniPath);
		D3DHooks::ReadINI(iniPath);
		return true;
	}

	void InitHooks() {
		if (Setting::bEnableThreadingTweaks) [[likely]]
			ThreadingTweaks::InitHooks();

		if (Setting::bFastExit) [[likely]]
			FastExit::InitHooks();

		if (Setting::bRedoHashtables) [[likely]]
			HashTables::InitHooks();

		if (Setting::bModifyDirectXBehavior) [[likely]]
			D3DHooks::InitHooks();

		if (Setting::bGTCFix) [[likely]]
			TickFix::InitHooks();
	}

	void InitEarlyHooks() {
		if (Setting::bModifyDirectXBehavior) [[likely]]
			D3DHooks::InitEarlyHooks();
	}
}



EXTERN_DLL_EXPORT bool NVSEPlugin_Query(const NVSEInterface* nvse, PluginInfo* info) {
	info->name			= PLUGIN_NAME;
	info->version		= PLUGIN_VERSION;
	info->infoVersion	= PluginInfo::kInfoVersion;

	// version checks
	if (nvse->nvseVersion < NVSE_VERSION_INTEGER) [[unlikely]] {
		char text[72];
		sprintf_s(text, "NVSE version too old (got %08X expected at least %08X)", nvse->nvseVersion, NVSE_VERSION_INTEGER);
		MessageBoxA(NULL, text, PLUGIN_NAME, MB_OK);
		return false;
	}

	return !nvse->isEditor;
}

EXTERN_DLL_EXPORT bool NVSEPlugin_Preload() {
	Main::bINIRead = Main::ReadINI(INIUtils::GetINIPath());

	if (Main::bINIRead)
		Main::InitEarlyHooks();

	return true;
}

EXTERN_DLL_EXPORT bool NVSEPlugin_Load(const NVSEInterface* nvse) {
	if (nvse->isEditor) [[unlikely]]
		return true;

	if (Main::bINIRead) [[likely]]
		Main::InitHooks();

	return true;
}

BOOL WINAPI DllMain(
	HANDLE  hDllHandle,
	DWORD   dwReason,
	LPVOID  lpreserved
)
{
	return TRUE;
}