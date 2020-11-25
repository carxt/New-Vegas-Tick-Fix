#pragma once

#include "FPSTimer.h"
double FPSTimerFrequency = 0;
double lastCount = 0;

signed int GetTickCountBias = 0;

void FPSStartCounter()
{
	auto Now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch()));
	long duration = Now.count();
	GetTickCountBias = duration - GetTickCount();
	lastCount = duration;
}

double GetFPSCounterMiliSeconds()
{

	auto Now = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()).time_since_epoch()));
	double currentCount = double(Now.count()) / 1000000;
	double toReturn = currentCount - lastCount;
	lastCount = currentCount;
	return toReturn;

}


DWORD ReturnCounter()
{
	auto Now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch()));
	return unsigned long (Now.count() - GetTickCountBias);
}
