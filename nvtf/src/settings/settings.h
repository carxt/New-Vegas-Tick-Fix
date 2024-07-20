#pragma once
#include <common/settings.hpp>


namespace settings {
	static const char* sectionName = "Main";
	static auto bEnableThreadingTweaks = tickfix::common::settings::SettingContainer(0, sectionName, "bEnableThreadingTweaks");
	void InitSettings();
	void Start();
};