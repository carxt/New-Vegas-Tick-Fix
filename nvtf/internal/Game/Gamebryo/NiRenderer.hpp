#pragma once

#include "NiCriticalSection.hpp"

class NiShader;
class NiDynamicEffectState;
class NiRenderedCubeMap;
class BSShaderAccumulator;
class NiDX9Renderer;
class NiPropertyState;
class NiRenderTargetGroup;
class NiFrustum;

class NiRenderer {
public:
	NiRenderer();
	virtual ~NiRenderer();

	char							pad[60]; // We don't need everything
	char							unk040[62];
	NiCriticalSection				m_kRendererLock;
	char							unk0A0[94];
	NiCriticalSection				m_kPrecacheCriticalSection;
	char							unk120[95];
	NiCriticalSection				m_kSourceDataCriticalSection;
	char							unk1AC[92];
	uint32_t						m_eSavedFrameState;
	uint32_t						m_eFrameState;
	uint32_t						m_uiFrameID;
	bool							m_bRenderTargetGroupActive;
	bool							m_bBatchRendering;
	int								unk20C[29];

	void LockRenderer();
	bool TryLockRenderer();
	void UnlockRenderer();
};

ASSERT_SIZE(NiRenderer, 0x280)