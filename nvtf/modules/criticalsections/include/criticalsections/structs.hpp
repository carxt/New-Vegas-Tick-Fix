namespace tickfix {
	namespace criticalsections {
		namespace structs {
			struct MemoryHeap
			{
				void* vtable; // AbstractHeap
				UInt32 unk04[0x835];
				CRITICAL_SECTION criticalSection;
			};
			struct Heaps
			{
				MemoryHeap* defaultHeap;
				MemoryHeap* staticHeap;
				MemoryHeap* fileHeap;
			};


			struct HeapManager
			{
				UInt16 pad00;
				UInt16 unk02;
				UInt32 unk04; // 04
				Heaps* memHeaps; // 08
				static HeapManager* GetSingleton() { return (HeapManager*)0x11F6238; }
			};


			struct BGSLightCriticalSection {
				uintptr_t OwningThreadId = 0;
				uintptr_t LockCount = 0;
			};
		};
	};
};