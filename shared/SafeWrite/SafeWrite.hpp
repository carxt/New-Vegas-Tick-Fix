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

DECLSPEC_NOINLINE void PatchMemoryNop(ULONG_PTR Address, SIZE_T Size);
void PatchMemoryNopRange(ULONG_PTR StartAddress, ULONG_PTR EndAddress);

template <typename T>
void WriteRelCall(SIZE_T jumpSrc, T jumpTgt) {
	WriteRelCall(jumpSrc, (SIZE_T)jumpTgt);
}

template <typename T>
void WriteRelJump(SIZE_T jumpSrc, T jumpTgt) {
	WriteRelJump(jumpSrc, (SIZE_T)jumpTgt);
}

DECLSPEC_NOINLINE void ReplaceCall(SIZE_T jumpSrc, SIZE_T jumpTgt);

template <typename T>
void ReplaceCall(SIZE_T jumpSrc, T jumpTgt) {
	ReplaceCall(jumpSrc, (SIZE_T)jumpTgt);
}

void ReplaceVirtualFunc(SIZE_T jumpSrc, void* jumpTgt);

// Stores the function-to-call before overwriting it, to allow calling the overwritten function after our hook is over.
// Thanks Demorome and lStewieAl

// Taken from lStewieAl.
// Returns the address of the jump/called function, assuming there is one.
static inline SIZE_T GetRelJumpAddr(SIZE_T jumpSrc) {
	return *(SIZE_T*)(jumpSrc + 1) + jumpSrc + 5;
}

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

template <typename C, typename Ret, typename... Args>
void ReplaceVirtualFuncEx(SIZE_T source, Ret(C::* const target)(Args...) const) {
	union
	{
		Ret(C::* tgt)(Args...) const;
		SIZE_T funcPtr;
	} conversion;
	conversion.tgt = target;

	SafeWrite32(source, conversion.funcPtr);
}

template <typename C, typename Ret, typename... Args>
void ReplaceVirtualFuncEx(SIZE_T source, Ret(C::* const target)(Args...)) {
	union
	{
		Ret(C::* tgt)(Args...);
		SIZE_T funcPtr;
	} conversion;
	conversion.tgt = target;

	SafeWrite32(source, conversion.funcPtr);
}

template <typename C, typename Ret, typename... Args>
void ReplaceVTableEntry(void** apVTable, uint32_t auiPosition, Ret(C::* const target)(Args...) const) {
	union {
		Ret(C::* tgt)(Args...) const;
		SIZE_T funcPtr;
	} conversion;
	conversion.tgt = target;

	apVTable[auiPosition] = (void*)conversion.funcPtr;
}

template <typename C, typename Ret, typename... Args>
void ReplaceVTableEntry(void** apVTable, uint32_t auiPosition, Ret(C::* const target)(Args...)) {
	union {
		Ret(C::* tgt)(Args...);
		SIZE_T funcPtr;
	} conversion;
	conversion.tgt = target;

	apVTable[auiPosition] = (void*)conversion.funcPtr;
}

class CallDetour {
	SIZE_T overwritten_addr = 0;
public:
	DECLSPEC_NOINLINE void WriteRelCall(SIZE_T jumpSrc, void* jumpTgt)
	{
		__assume(jumpSrc != 0);
		__assume(jumpTgt != nullptr);
#ifdef _DEBUG
		if (*reinterpret_cast<uint8_t*>(jumpSrc) != 0xE8) {
			char cTextBuffer[72];
			sprintf_s(cTextBuffer, "Cannot write detour; jumpSrc is not a function call. (0x%08X)", jumpSrc);
			MessageBoxA(nullptr, cTextBuffer, "WriteRelCall", MB_OK | MB_ICONERROR);
			return;
		}
#endif
		overwritten_addr = GetRelJumpAddr(jumpSrc);
		::WriteRelCall(jumpSrc, jumpTgt);
	}

	template <typename T>
	void ReplaceCall(SIZE_T jumpSrc, T jumpTgt) {
		__assume(jumpSrc != 0);
#ifdef _DEBUG
		if (*reinterpret_cast<uint8_t*>(jumpSrc) != 0xE8) {
			char cTextBuffer[72];
			sprintf_s(cTextBuffer, "Cannot write detour; jumpSrc is not a function call. (0x%08X)", jumpSrc);
			MessageBoxA(nullptr, cTextBuffer, "WriteRelCall", MB_OK | MB_ICONERROR);
			return;
		}
#endif
		overwritten_addr = GetRelJumpAddr(jumpSrc);
		::ReplaceCall(jumpSrc, (SIZE_T)jumpTgt);
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

	[[nodiscard]] SIZE_T GetOverwrittenAddr() const { return overwritten_addr; }
};