#include "Interface.hpp"

bool Interface::IsLoadingNewGame() {
    return *(bool*)0x11D8907;
}
