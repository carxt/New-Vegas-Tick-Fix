#include "NiDX9Renderer.hpp"

NiDX9Renderer* NiDX9Renderer::GetSingleton() {
	return *reinterpret_cast<NiDX9Renderer**>(0x11C73B4);
}

// GAME - 0xE69410
bool NiDX9Renderer::IsD3D9Create() {
	return CdeclCall<bool>(0xE69410);
}

// GAME - 0xE73F60
bool NiDX9Renderer::PrecacheGeometryEx(NiRefObject* apGeometry, uint32_t uiBonesPerPartition, uint32_t uiBonesPerVertex, NiD3DShaderDeclaration* apShaderDeclaration) {
	return ThisCall<bool>(0xE73F60, this, apGeometry, uiBonesPerPartition, uiBonesPerVertex, apShaderDeclaration);
}