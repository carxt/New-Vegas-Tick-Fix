#pragma once
#include <chrono>
extern int g_bAlternateGTCFix;

void FPSStartCounter();
double GetFPSCounterMiliSeconds_WRAP(bool doUpdate= true);
DWORD ReturnCounter_WRAP();