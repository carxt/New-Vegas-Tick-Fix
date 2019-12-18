#pragma once







double FPSTimerFrequency = 0.0;
__int64 CurrentStat = 0;

void FPSStartCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li)) _MESSAGE("UNEXPECTED ERROR");
	FPSTimerFrequency = double(li.QuadPart) / 1000;
	QueryPerformanceCounter(&li);
	CurrentStat = li.QuadPart;

}
double __forceinline GetFPSCounterMiliSeconds()
{

	LARGE_INTEGER li;	
	QueryPerformanceCounter(&li);
	double toReturn = double(li.QuadPart - CurrentStat) / FPSTimerFrequency;
	CurrentStat = li.QuadPart;
	return toReturn;

}













//Legacy stuff
/*
void TimeGlobalHook() {
	if (!initTimeHook) {
		//	timeBeginPeriod(1); //whatever
		initTimeHook = true;
		LastTime = timeGetTime();
		DefaultMaxTime = (*fMaxTime) * 1000;
	}
	UINT32 getTime = timeGetTime();
	double Delta = ((double)(getTime - LastTime));
	*g_FPSGlobal = Delta;
	LastTime = getTime;
	*fMaxTime = ((Delta > 0 && Delta < DefaultMaxTime) ? Delta / 1000 : DefaultMaxTime / 1000);
}
*/