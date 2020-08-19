#pragma once

#include "FPSTimer.h"
double FPSTimerFrequency = 0;
ULONGLONG CurrentStat = 0;
void FPSStartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li)) _MESSAGE("UNEXPECTED ERROR");
	FPSTimerFrequency = double(li.QuadPart) / 1000.0;
	QueryPerformanceCounter(&li);
	CurrentStat = li.QuadPart;

}

double GetFPSCounterMiliSeconds()
{
	if (!g_bAlternateGTCFix) 
	{
		LARGE_INTEGER li;
		QueryPerformanceCounter(&li);
		double toReturn = (double(li.QuadPart - CurrentStat)) / FPSTimerFrequency;
		CurrentStat = li.QuadPart;
		return toReturn;
	}
	ULONGLONG tGtCurrent = timeGetTime();
	double toReturn = double(tGtCurrent - CurrentStat);
	CurrentStat = tGtCurrent;
	return toReturn;
}


DWORD ReturnCounter()
{
	LARGE_INTEGER tgtInteger;
	QueryPerformanceCounter(&tgtInteger);
	return DWORD((tgtInteger.QuadPart / FPSTimerFrequency));

}


