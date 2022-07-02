#pragma once

#include "FPSTimer.h"
namespace QPC {
	double FPSTimerFrequency = 0;
	double lastCount = 0;
	signed int GetTickCountBias = 0;
	DWORD ReturnCounter()
	{
		auto Now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch()));
		return unsigned long(Now.count() + GetTickCountBias);
	}
	double GetFPSCounterMiliSeconds()
	{

		auto Now = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()).time_since_epoch()));
		double currentCount = double(Now.count()) / 1000000;
		double toReturn = currentCount - lastCount;
		lastCount = currentCount;
		return toReturn;

	}
}
namespace tGt 
{
	double lastCount = 0;
	signed int GetTickCountBias = 0;
	DWORD ReturnCounter()
	{
		return unsigned long(timeGetTime() + GetTickCountBias);
	}
	double GetFPSCounterMiliSeconds()
	{

		double currentCount = double(timeGetTime()) ;
		double toReturn = currentCount - lastCount;
		lastCount = currentCount;
		return toReturn;

	}

}

void FPSStartCounter()
{
	auto Now = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now()).time_since_epoch()));
	long duration = Now.count();


	QPC::GetTickCountBias = duration - long(GetTickCount()); //this would proooooooobably fail if your system was started 53 days ago, but at that point idc
	QPC::lastCount = duration;
	//this one is for timeGetTime
	tGt::lastCount = timeGetTime();
	tGt::GetTickCountBias = long(tGt::lastCount) - long(GetTickCount());

}






double GetFPSCounterMiliSeconds_WRAP(bool doUpdate)
{
	if (doUpdate)
	{
		if (!g_bAlternateGTCFix) return QPC::GetFPSCounterMiliSeconds();
		else return tGt::GetFPSCounterMiliSeconds();
	}
	else {
		if (!g_bAlternateGTCFix)
		{
			auto currentCount = double((std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now()).time_since_epoch()))).count()) / 1000000;
			return double(currentCount - QPC::lastCount);
		}
		else {

			return double(double(timeGetTime()) - tGt::lastCount);
		};
	}

}




DWORD ReturnCounter_WRAP()
{
	if (!g_bAlternateGTCFix) return QPC::ReturnCounter();
	else return tGt::ReturnCounter();
}