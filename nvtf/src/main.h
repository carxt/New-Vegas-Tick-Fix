#pragma once
#include "nvse/PluginAPI.h"
#define NVTF_VERSION 10
static HANDLE MyHandle;

extern "C" {
	bool NVSEPlugin_Query(const NVSEInterface* nvse, PluginInfo* info);
	bool NVSEPlugin_Load(const NVSEInterface* nvse);
};
