#pragma once

#include "BSTimer.hpp"

struct TimeGlobal : public BSTimer {

	static TimeGlobal* GetSingleton();
};
