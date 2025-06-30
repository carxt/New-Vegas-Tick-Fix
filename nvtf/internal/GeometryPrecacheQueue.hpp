#pragma once

#include "Game/Gamebryo/NiRefObject.hpp"
#include "Game/Gamebryo/NiDX9Renderer.hpp"
#include <shared/Utils/WaitLock.hpp>
#include <queue>

class NiD3DShaderDeclaration;
class NiNode;

class GeometryPrecacheQueue : public NiDX9Renderer {
public:
	static void InitHooks();

	bool PrecacheGeometry_MT(NiRefObject* apGeometry, uint32_t auiBonesPerPartition, uint32_t auiBonesPerVertex, NiD3DShaderDeclaration* apShaderDeclaration);
	void PerformPrecache_MT();

private:
	struct QueuedObject {
		NiPointer<NiRefObject>	spGeometry;
		uint32_t				uiBonesPerPartition = 0;
		uint32_t				uiBonesPerVertex	= 0;
		NiD3DShaderDeclaration* pShaderDeclaration	= nullptr;
	};

	static HANDLE hTaskEvent;
	static HANDLE hPauseEvent;

	static WaitLock					 kQueueLock;
	static std::queue<QueuedObject>  kQueue;
	static HANDLE 					 hThread;

	void StartProcessing();
	void StopProcessing();

	static DWORD __stdcall ThreadProc(LPVOID lpThreadParameter);
	static bool InitializeThread();
};