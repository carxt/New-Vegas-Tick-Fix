#include "NiDX9Renderer.hpp"

// GAME - 0xE69410
bool NiDX9Renderer::IsD3D9Create() {
	return CdeclCall<bool>(0xE69410);
};