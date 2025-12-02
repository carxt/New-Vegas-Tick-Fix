#pragma once

class BSCriticalSection  {
public:
	CRITICAL_SECTION	kCriticalSection;

	BSCriticalSection() {
		InitializeCriticalSection(&kCriticalSection);
	}

	~BSCriticalSection() {
		DeleteCriticalSection(&kCriticalSection);
	}

	void Lock(const char* apName = nullptr) {
		EnterCriticalSection(&kCriticalSection);
	}

	void Unlock() {
		LeaveCriticalSection(&kCriticalSection);
	}

	bool TryLock() {
		return TryEnterCriticalSection(&kCriticalSection) != 0;
	}
};

ASSERT_SIZE(BSCriticalSection, 0x18);

struct BSCriticalSectionScope {
	BSCriticalSectionScope(BSCriticalSection& aCriticalSection) : rCriticalSection(aCriticalSection) {
		rCriticalSection.Lock();
	}

	BSCriticalSectionScope(BSCriticalSection* apCriticalSection) : rCriticalSection(*apCriticalSection) {
		rCriticalSection.Lock();
	}

	~BSCriticalSectionScope() {
		rCriticalSection.Unlock();
	}

	BSCriticalSection& rCriticalSection;
};