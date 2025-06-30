#pragma once

template <class T>
class NiPointer {
public:
	__forceinline NiPointer() : m_pObject(nullptr) {};
	__forceinline NiPointer(T* apObject) : m_pObject(apObject) { if (m_pObject) m_pObject->IncRefCount(); }
	__forceinline NiPointer(const NiPointer& arPtr) : m_pObject(arPtr.m_pObject) { if (m_pObject) m_pObject->IncRefCount(); }
	__forceinline ~NiPointer() { if (m_pObject) m_pObject->DecRefCount(); }

	T* m_pObject;

	__forceinline operator T* () const { return m_pObject; }
	__forceinline T& operator*() const { return *m_pObject; }
	__forceinline T* operator->() const { return m_pObject; }

	__forceinline NiPointer<T>& operator =(const NiPointer& ptr) {
		if (m_pObject != ptr.m_pObject) {
			if (m_pObject)
				m_pObject->DecRefCount();
			m_pObject = ptr.m_pObject;
			if (m_pObject)
				m_pObject->IncRefCount();
		}
		return *this;
	}

	__forceinline NiPointer<T>& operator =(T* pObject) {
		if (m_pObject != pObject) {
			if (m_pObject)
				m_pObject->DecRefCount();
			m_pObject = pObject;
			if (m_pObject)
				m_pObject->IncRefCount();
		}
		return *this;
	}

	__forceinline bool operator==(T* apObject) const { return (m_pObject == apObject); }

	__forceinline bool operator==(const NiPointer& ptr) const { return (m_pObject == ptr.m_pObject); }

	__forceinline operator bool() const { return m_pObject != nullptr; }
};

#define NiSmartPointer(className) \
    class className; \
    typedef NiPointer<className> className##Ptr;