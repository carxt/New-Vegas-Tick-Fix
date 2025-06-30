#pragma once

#include "NiSmartPointer.hpp"

NiSmartPointer(NiRefObject);

class NiRefObject {
public:
	NiRefObject();

	virtual			~NiRefObject(); // 00 | Destructor
	virtual void	DeleteThis();   // 01 | Deletes the object

	uint32_t m_uiRefCount;

	// GAME - 0x40F6E0
	void IncRefCount() {
		InterlockedIncrement(&m_uiRefCount);
	}

	// GAME - 0x401970
	void DecRefCount() {
		if (!InterlockedDecrement(&m_uiRefCount))
			DeleteThis();
	}
};

ASSERT_SIZE(NiRefObject, 0x8);