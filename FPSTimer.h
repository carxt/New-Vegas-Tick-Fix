#pragma once

double FPSTimerFrequency = 0.0;
LONGLONG CurrentStat = 0;



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

	LARGE_INTEGER li;	
	QueryPerformanceCounter(&li);
	double toReturn = (double(li.QuadPart - CurrentStat)) / FPSTimerFrequency;
	CurrentStat = li.QuadPart;
	return toReturn;

}


DWORD ReturnCounter()
{
	LARGE_INTEGER tgtInteger;
	QueryPerformanceCounter(&tgtInteger);
	return DWORD((tgtInteger.QuadPart / FPSTimerFrequency));

}


