#include "GeometryPrecacheQueue.hpp"
#include "Game/Bethesda/TESMain.hpp"
#include "Game/Gamebryo/NiDX9Renderer.hpp"

WaitLock				                            GeometryPrecacheQueue::kQueueLock;
std::queue<GeometryPrecacheQueue::QueuedObject>     GeometryPrecacheQueue::kQueue;
std::vector<NiPointer<NiRefObject>>					GeometryPrecacheQueue::kActiveObjects;
HANDLE 												GeometryPrecacheQueue::hThread;
HANDLE												GeometryPrecacheQueue::hTaskEvent;
HANDLE												GeometryPrecacheQueue::hPauseEvent;

void GeometryPrecacheQueue::InitHooks() {
	if (!InitializeThread())
		return;

    ReplaceVirtualFuncEx(0x10EE5A8, &PrecacheGeometry_MT);
	WriteRelJumpEx(0xE74120, &PerformPrecache_MT);

	WriteRelJumpEx(0x4A0370, &StopProcessing);
	WriteRelJumpEx(0x4A03C0, &StartProcessing);
}

bool GeometryPrecacheQueue::PrecacheGeometry_MT(NiRefObject* apGeometry, uint32_t auiBonesPerPartition, uint32_t auiBonesPerVertex, NiD3DShaderDeclaration* apShaderDeclaration) {
	if (GetCurrentThreadId() == TESMain::GetSingleton()->uiMainThreadID) [[unlikely]] {
		return PrecacheGeometryEx(apGeometry, auiBonesPerPartition, auiBonesPerVertex, apShaderDeclaration);
	}

	kQueueLock.Lock();
	kQueue.push({ apGeometry, auiBonesPerPartition, auiBonesPerVertex, apShaderDeclaration });
	kQueueLock.Unlock();

	SetEvent(hTaskEvent);
	return true;
}

__declspec(naked) void __fastcall PerformPrecache_ST(NiDX9Renderer* apThis) {
	__asm {
		sub esp, 0x24
		push ebx
		push ebp
		push 0xE74125
		retn
	}
}

void GeometryPrecacheQueue::PerformPrecache_MT() {
	if (GetCurrentThreadId() != TESMain::GetSingleton()->uiMainThreadID) [[unlikely]] {
		SetEvent(hTaskEvent);
	}
	else [[likely]] {
		bool bLocked = TryLockRenderer();

		if (bLocked) {
			// We got in!
			PerformPrecache_ST(this);
			UnlockRenderer();
		}
	}
}

// Runs after the frame has been rendered - this is where we can do our precaching
void GeometryPrecacheQueue::StartProcessing() {
	UnlockRenderer();
	SetEvent(hPauseEvent);
}

// Runs before the frame is rendered - we cannot do precaching while the frame is being rendered
void GeometryPrecacheQueue::StopProcessing() {
	ResetEvent(hPauseEvent);
	LockRenderer();
}

DWORD __stdcall GeometryPrecacheQueue::ThreadProc(LPVOID lpThreadParameter) {
	while (true) {
		WaitForSingleObject(hPauseEvent, INFINITE);
		NiDX9Renderer* pRenderer = NiDX9Renderer::GetSingleton();
		if (pRenderer) {
			const uint32_t uiPrecacheCount = pRenderer->uiPrePackObjectCount;
			bool bIsEmpty = kQueue.empty();
			if (uiPrecacheCount > 5 || !bIsEmpty)
				SetEvent(hTaskEvent);

			WaitForSingleObject(hTaskEvent, INFINITE);

			bIsEmpty = kQueue.empty();
			if (!bIsEmpty)
				kActiveObjects.reserve(kQueue.size());
			
			while (!bIsEmpty && WaitForSingleObject(hPauseEvent, INFINITE) == WAIT_OBJECT_0) {
				kQueueLock.Lock();
				auto& rData = kQueue.front();
				kActiveObjects.push_back(rData.spGeometry);
				pRenderer->PrecacheGeometryEx(rData.spGeometry, rData.uiBonesPerPartition, rData.uiBonesPerVertex, rData.pShaderDeclaration);
				kQueue.pop();
				bIsEmpty = kQueue.empty();
				kQueueLock.Unlock();
			}

			if (uiPrecacheCount && WaitForSingleObject(hPauseEvent, INFINITE) == WAIT_OBJECT_0)
				PerformPrecache_ST(pRenderer);

			kActiveObjects.clear();
			kActiveObjects.shrink_to_fit();
		}
	}

	return 0;
}

bool GeometryPrecacheQueue::InitializeThread() {
	hTaskEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	hPauseEvent = CreateEvent(nullptr, TRUE, TRUE, nullptr); 

	hThread = CreateThread(nullptr, 0x10000, ThreadProc, nullptr, 0, nullptr);
	if (hThread) {
		wchar_t cThreadName[32];
		_snwprintf_s(cThreadName, sizeof(cThreadName), L"[NVTF] Geometry Precache Queue");
		SetThreadDescription(hThread, cThreadName);
		return true;
	}
	else {
		if (hTaskEvent) {
			CloseHandle(hTaskEvent);
			hTaskEvent = nullptr;
		}

		if (hPauseEvent) {
			CloseHandle(hPauseEvent);
			hPauseEvent = nullptr;
		}
		return false;
	}
}