#include "NiRenderer.hpp"

// GAME - 0x4A0370
void NiRenderer::LockRenderer() {
	m_kRendererLock.Lock();
}

bool NiRenderer::TryLockRenderer() {
	return m_kRendererLock.TryLock();
}

// GAME - 0x4A03C0
void NiRenderer::UnlockRenderer() {
	m_kRendererLock.Unlock();
}