// all credits to JazzisParis
#pragma once

#include <intrin.h>

constexpr double
kDblZero = 0,
kDblPI = 3.141592653589793,
kDblPIx2 = 6.283185307179586,
kDblPIx3d2 = 4.71238898038469,
kDblPId2 = 1.5707963267948966,
kDblPId4 = 0.7853981633974483,
kDblPId6 = 0.5235987755982989,
kDblPId12 = 0.26179938779914946,
kDbl2dPI = 0.6366197723675814,
kDbl4dPI = 1.2732395447351628,
kDblTanPId6 = 0.5773502691896257,
kDblTanPId12 = 0.2679491924311227,
kDblPId180 = 0.017453292519943295;

constexpr float
kFltZero = 0.0F,
kFltHalf = 0.5F,
kFltOne = 1.0F,
kFltTwo = 2.0F,
kFltFour = 4.0F,
kFltSix = 6.0F,
kFlt10 = 10.0F,
kFlt100 = 100.0F,
kFlt2048 = 2048.0F,
kFlt4096 = 4096.0F,
kFlt10000 = 10000.0F,
kFlt12288 = 12288.0F,
kFlt40000 = 40000.0F,
kFltMax = FLT_MAX;

class CriticalSection {
	CRITICAL_SECTION	critSection;

public:
	CriticalSection() { InitializeCriticalSection(&critSection); }
	~CriticalSection() { DeleteCriticalSection(&critSection); }

	void Enter() { EnterCriticalSection(&critSection); }
	void Leave() { LeaveCriticalSection(&critSection); }
	bool TryEnter() { return TryEnterCriticalSection(&critSection) != 0; }
};

class LightCS {
	UInt32	owningThread;
	UInt32	enterCount;

public:
	LightCS() : owningThread(0), enterCount(0) {}

	void Enter();
	void EnterSleep();
	void Leave();
};

class DebugLog {
	FILE* theFile;
	UInt32			indent;

public:
	DebugLog() : theFile(NULL), indent(40) {}
	~DebugLog() { if (theFile) fclose(theFile); }

	bool Create(const char* filePath);
	void Message(const char* msgStr);
	void FmtMessage(const char* fmt, va_list args);
	void Indent() { if (indent) indent--; }
	void Outdent() { if (indent < 40) indent++; }
};

extern DebugLog gLog;

void PrintLog(const char* fmt, ...);