#include "TESMain.hpp"

TESMain* TESMain::GetSingleton() {
	return *reinterpret_cast<TESMain**>(0x11DEA0C);
}

bool TESMain::IsInPauseFade() {
    return *reinterpret_cast<bool*>(0x11DEA2D);
}