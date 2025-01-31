// all credits to JazzisParis
#include "internal/utility.h"
#include <time.h>

const UInt32 kMaxMessageLength = 0x4000;
void LightCS::Enter() {
	UInt32 threadID = GetCurrentThreadId();
	if (owningThread == threadID) {
		enterCount++;
		return;
	}
	while (InterlockedCompareExchange(&owningThread, threadID, 0));
	enterCount = 1;
}

#define FAST_SLEEP_COUNT 10000UL

void LightCS::EnterSleep() {
	UInt32 threadID = GetCurrentThreadId();
	if (owningThread == threadID) {
		enterCount++;
		return;
	}
	UInt32 fastIdx = FAST_SLEEP_COUNT;
	while (InterlockedCompareExchange(&owningThread, threadID, 0)) {
		if (fastIdx) {
			fastIdx--;
			Sleep(0);
		}
		else Sleep(1);
	}
	enterCount = 1;
}

void LightCS::Leave() {
	if (!--enterCount)
		owningThread = 0;
}

bool DebugLog::Create(const char* filePath) {
	theFile = _fsopen(filePath, "wb", 0x20);
	return theFile != NULL;
}

const char kIndentLevelStr[] = "\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t\t";

void DebugLog::Message(const char* msgStr) {
	if (!theFile) return;
	if (indent < 40)
		fputs(kIndentLevelStr + indent, theFile);
	fputs(msgStr, theFile);
	fputc('\n', theFile);
	fflush(theFile);
}

void DebugLog::FmtMessage(const char* fmt, va_list args) {
	if (!theFile) return;
	if (indent < 40)
		fputs(kIndentLevelStr + indent, theFile);
	vfprintf(theFile, fmt, args);
	fputc('\n', theFile);
	fflush(theFile);
}

DebugLog gLog;

void PrintLog(const char* fmt, ...) {
	va_list args;
	va_start(args, fmt);
	gLog.FmtMessage(fmt, args);
	va_end(args);
}