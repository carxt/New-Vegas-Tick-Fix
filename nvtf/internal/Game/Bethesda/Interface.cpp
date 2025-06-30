#include "Interface.hpp"

bool Interface::IsLoadingNewGame() {
    return *reinterpret_cast<bool*>(0x11D8907);
}
