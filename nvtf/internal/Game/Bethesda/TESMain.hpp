#pragma once

class TESMain {
public:
	bool					bOneMore;
	bool					bQuitGame;
	bool					bResetGame;
	bool					bGameActive;
	bool					bOnIdle;
	bool					unk05;
	bool					bIsFlyCam;
	bool					bFreezeTime;
	HWND					hWnd;
	HINSTANCE				hInstance;
	uint32_t				uiMainThreadID;
	HANDLE					hMainThread;
	// There's more but we don't need it for now

	static TESMain* GetSingleton();

	static bool IsInPauseFade();
};