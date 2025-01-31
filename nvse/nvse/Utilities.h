#pragma once

class Script;

//const std::string & GetFalloutDirectory(void);
//std::string GetNVSEConfigOption(const char * section, const char * key);
//bool GetNVSEConfigOption_UInt32(const char * section, const char * key, UInt32 * dataOut);

// this has been tested to work for non-varargs functions
// varargs functions end up with 'this' passed as the last parameter (ie. probably broken)
// do NOT use with classes that have multiple inheritance

// if many member functions are to be declared, use MEMBER_FN_PREFIX to create a type with a known name
// so it doesn't need to be restated throughout the member list

// all of the weirdness with the _GetType function is because you can't declare a static const pointer
// inside the class definition. inlining automatically makes the function call go away since it's a const

#define MEMBER_FN_PREFIX(className)	\
	typedef className _MEMBER_FN_BASE_TYPE

#define DEFINE_MEMBER_FN_LONG(className, functionName, retnType, address, ...)		\
	typedef retnType (className::* _##functionName##_type)(__VA_ARGS__);			\
																					\
	inline _##functionName##_type * _##functionName##_GetPtr(void)					\
	{																				\
		static const UInt32 _address = address;										\
		return (_##functionName##_type *)&_address;									\
	}

#define DEFINE_MEMBER_FN(functionName, retnType, address, ...)	\
	DEFINE_MEMBER_FN_LONG(_MEMBER_FN_BASE_TYPE, functionName, retnType, address, __VA_ARGS__)

#define CALL_MEMBER_FN(obj, fn)	\
	((*(obj)).*(*((obj)->_##fn##_GetPtr())))

// ConsolePrint() limited to 512 chars; use this to print longer strings to console
//void Console_Print_Long(const std::string& str);

// Macro for debug output to console at runtime
#if RUNTIME
#ifdef _DEBUG
#define DEBUG_PRINT(x, ...) { Console_Print((x), __VA_ARGS__); }
#define DEBUG_MESSAGE(x, ...) { PrintDebug((x), __VA_ARGS__); }
#else
#define DEBUG_PRINT(x, ...) { }
#define DEBUG_MESSAGE(x, ...) { }
#endif	//_DEBUG
#else
#define DEBUG_PRINT(x, ...) { }
#define DEBUG_MESSAGE(x, ...) { }
// This is so we don't have to handle size change with EditorData :)
#undef STATIC_ASSERT
#define STATIC_ASSERT(a)
#endif	// RUNTIME

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