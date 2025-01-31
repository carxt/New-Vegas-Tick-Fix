#pragma once

extern int g_bAllowDirectXDebugging;
extern int g_bToggleTripleBuffering;
extern int g_bUseDefaultPoolForTextures;

namespace D3DHooks {
	void Init();
}