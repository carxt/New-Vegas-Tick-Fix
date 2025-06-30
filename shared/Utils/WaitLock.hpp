#pragma once

#include <atomic>
#include <synchapi.h>

#pragma comment(lib, "Synchronization.lib")

// Lock that uses WaitOnAddress
class WaitLock {
private:
	bool Release() {
		return ucLock.exchange(OPEN, std::memory_order_acq_rel) == LOCKED;
	}

public:
	WaitLock() = default;
	~WaitLock() = default;

	enum LockState : uint8_t {
		OPEN	= 0,
		LOCKED	= 1
	};

	std::atomic<uint8_t> ucLock = OPEN;
	
	bool TryLock() {
		return ucLock.exchange(LOCKED, std::memory_order_acq_rel) == OPEN;
	}

	void Lock() {
		uint8_t ucCmp = LOCKED;
		while (true) {
			if (TryLock())
				break;

			WaitOnAddress(&ucLock, &ucCmp, sizeof(ucLock), INFINITE);
		}
	}

	void Unlock() {
		if (Release())
			WakeByAddressSingle(&ucLock);
	}
};

class WaitLockScope {
public:
	WaitLockScope(WaitLock* apLock) : pLock(apLock)	 { pLock->Lock(); };
	WaitLockScope(WaitLock& arLock) : pLock(&arLock) { pLock->Lock(); };
	~WaitLockScope() { pLock->Unlock(); };

protected:
	WaitLock* pLock = nullptr;
};