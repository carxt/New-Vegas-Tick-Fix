#include "BGSSaveLoadGame.hpp"

BGSSaveLoadGame* BGSSaveLoadGame::GetSingleton() {
    return *(BGSSaveLoadGame**)0x11DDF38;
}

// GAME - 0x42CE10
bool BGSSaveLoadGame::IsLoading() const {
    return (uiGlobalFlags & 2) != 0;
}