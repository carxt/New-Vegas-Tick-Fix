#pragma once

#include <utility>

class Script;

#define SIZEOF_ARRAY(arrayName, elementType) (sizeof(arrayName) / sizeof(elementType))

// thread-safe template versions of ThisStdCall()

template <typename T_Ret = void, typename ...Args>
__forceinline T_Ret ThisStdCall(UInt32 _addr, void* _this, Args ...args) {
	class T {};
	union {
		UInt32  addr;
		T_Ret(T::* func)(Args...);
	} u = { _addr };
	return ((T*)_this->*u.func)(std::forward<Args>(args)...);
}
template <typename T_Ret = void, typename ...Args>
__forceinline T_Ret StdCall(UInt32 _addr, Args ...args) {
	return ((T_Ret(__stdcall*)(Args...))_addr)(std::forward<Args>(args)...);
}

template <typename T_Ret = void, typename ...Args>
__forceinline T_Ret CdeclCall(UInt32 _addr, Args ...args) {
	return ((T_Ret(__cdecl*)(Args...))_addr)(std::forward<Args>(args)...);
}
//	Templates for UInt8 return.

__forceinline UInt8 ThisStdCall_B(UInt32 _f, void* _t) {
	class T {}; union { UInt32 x; UInt8(T::* m)(); } u = { _f };
	return ((T*)_t->*u.m)();
}

template <typename T1>
__forceinline UInt8 ThisStdCall_B(UInt32 _f, void* _t, T1 a1) {
	class T {}; union { UInt32 x; UInt8(T::* m)(T1); } u = { _f };
	return ((T*)_t->*u.m)(a1);
}

template <typename T1, typename T2>
__forceinline UInt8 ThisStdCall_B(UInt32 _f, void* _t, T1 a1, T2 a2) {
	class T {}; union { UInt32 x; UInt8(T::* m)(T1, T2); } u = { _f };
	return ((T*)_t->*u.m)(a1, a2);
}

template <typename T1, typename T2, typename T3>
__forceinline UInt8 ThisStdCall_B(UInt32 _f, void* _t, T1 a1, T2 a2, T3 a3) {
	class T {}; union { UInt32 x; UInt8(T::* m)(T1, T2, T3); } u = { _f };
	return ((T*)_t->*u.m)(a1, a2, a3);
}

//	Templates for float return.

__forceinline float ThisStdCall_F(UInt32 _f, void* _t) {
	class T {}; union { UInt32 x; float (T::* m)(); } u = { _f };
	return ((T*)_t->*u.m)();
}

template <typename T1>
__forceinline float ThisStdCall_F(UInt32 _f, void* _t, T1 a1) {
	class T {}; union { UInt32 x; float (T::* m)(T1); } u = { _f };
	return ((T*)_t->*u.m)(a1);
}