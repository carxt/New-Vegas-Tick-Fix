#pragma once

DECLSPEC_NOINLINE void SafeWrite8(SIZE_T addr, SIZE_T data);
DECLSPEC_NOINLINE void SafeWrite16(SIZE_T addr, SIZE_T data);
DECLSPEC_NOINLINE void SafeWrite32(SIZE_T addr, SIZE_T data);
DECLSPEC_NOINLINE void SafeWriteBuf(SIZE_T addr, const void* data, SIZE_T len);

// 5 bytes
DECLSPEC_NOINLINE void WriteRelJump(SIZE_T jumpSrc, SIZE_T jumpTgt);
DECLSPEC_NOINLINE void WriteRelCall(SIZE_T jumpSrc, SIZE_T jumpTgt);

// 6 bytes
DECLSPEC_NOINLINE void WriteRelJnz(SIZE_T jumpSrc, SIZE_T jumpTgt);
DECLSPEC_NOINLINE void WriteRelJle(SIZE_T jumpSrc, SIZE_T jumpTgt);

template <typename T>
DECLSPEC_NOINLINE void WriteRelCall(SIZE_T jumpSrc, T jumpTgt) {
	WriteRelCall(jumpSrc, (SIZE_T)jumpTgt);
}

template <typename T>
DECLSPEC_NOINLINE void WriteRelJump(SIZE_T jumpSrc, T jumpTgt) {
	WriteRelJump(jumpSrc, (SIZE_T)jumpTgt);
}

DECLSPEC_NOINLINE void ReplaceCall(SIZE_T jumpSrc, SIZE_T jumpTgt);

template <typename T>
void ReplaceCall(SIZE_T jumpSrc, T jumpTgt) {
	ReplaceCall(jumpSrc, (SIZE_T)jumpTgt);
}

void ReplaceVirtualFunc(SIZE_T jumpSrc, void* jumpTgt);

// Specialization for member function pointers
template <typename C, typename Ret, typename... Args>
void WriteRelJumpEx(SIZE_T source, Ret(C::* const target)(Args...) const) {
    union
    {
        Ret(C::* tgt)(Args...) const;
        SIZE_T funcPtr;
    } conversion;
    conversion.tgt = target;

    WriteRelJump(source, conversion.funcPtr);
}

template <typename C, typename Ret, typename... Args>
void WriteRelJumpEx(SIZE_T source, Ret(C::* const target)(Args...)) {
    union
    {
        Ret(C::* tgt)(Args...);
        SIZE_T funcPtr;
    } conversion;
    conversion.tgt = target;

    WriteRelJump(source, conversion.funcPtr);
}

template <typename C, typename Ret, typename... Args>
void ReplaceCallEx(SIZE_T source, Ret(C::* const target)(Args...) const) {
	union
	{
		Ret(C::* tgt)(Args...) const;
		SIZE_T funcPtr;
	} conversion;
	conversion.tgt = target;

	ReplaceCall(source, conversion.funcPtr);
}

template <typename C, typename Ret, typename... Args>
void ReplaceCallEx(SIZE_T source, Ret(C::* const target)(Args...)) {
	union
	{
		Ret(C::* tgt)(Args...);
		SIZE_T funcPtr;
	} conversion;
	conversion.tgt = target;

	ReplaceCall(source, conversion.funcPtr);
}