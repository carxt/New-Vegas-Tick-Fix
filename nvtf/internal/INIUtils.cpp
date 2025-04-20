#include "INIUtils.hpp"

namespace INIUtils {

	const char cINIPath[] = "Data\\NVSE\\Plugins\\NVTF.ini";

	char cINIDir[MAX_PATH] = {};

	const char* GetINIPath() {
		if (cINIDir[0] != 0)
			return cINIDir;

		uint32_t uiLen = GetModuleFileName(GetModuleHandle(NULL), cINIDir, MAX_PATH);
		if (uiLen == 0)
			return nullptr;
		
		char* pLastSlash = strrchr(cINIDir, '\\');
		if (!pLastSlash)
			return nullptr;

		pLastSlash[0] = '\0'; 
		strcat_s(cINIDir, "\\Data\\NVSE\\Plugins\\NVTF.ini");

		return cINIDir;
	}
}
