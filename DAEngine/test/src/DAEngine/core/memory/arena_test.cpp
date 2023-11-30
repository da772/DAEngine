#include "arena_test.h"
#include "DAEngine/core/memory/arena.h"

using namespace da::memory;

struct ExampleStruct {
	int a;
	double b;
	char c;
};

CMemoryArenaTest::CMemoryArenaTest()
{

}


bool CMemoryArenaTest::RunTests()
{
	TEST_BEGIN();
	TEST_FUNC(testAllocationDeallocation);
	TEST_FUNC(testReset);
	TEST_FUNC(threadedTest);
	TEST_FUNC(speedTest);
	TEST_END();
}

bool CMemoryArenaTest::testAllocationDeallocation()
{
	std::size_t arenaSize = 1024 * 1024;  // 1 MB
	CMemoryArena arena(arenaSize);

	// Test basic allocation and deallocation
	ExampleStruct* structPtr = arena.allocate<ExampleStruct>();
	TEST_ASSERT(structPtr != nullptr);
	arena.release(structPtr);

	// Test multiple allocations and deallocations
	const std::size_t numAllocations = 100;
	ExampleStruct* arrayPtrs[numAllocations];

	for (std::size_t i = 0; i < numAllocations; ++i) {
		arrayPtrs[i] = arena.allocate<ExampleStruct>();
		TEST_ASSERT(arrayPtrs[i] != nullptr);
	}

	for (std::size_t i = 0; i < numAllocations; ++i) {
		arena.release(arrayPtrs[i]);
	}

	std::cout << "Allocation-deallocation tests passed." << std::endl;
	return true;
}

bool CMemoryArenaTest::testReset()
{
	std::size_t arenaSize = 1024 * 1024;  // 1 MB
	CMemoryArena arena(arenaSize);

	// Allocate and release some memory
	ExampleStruct* structPtr1 = arena.allocate<ExampleStruct>();
	ExampleStruct* structPtr2 = arena.allocate<ExampleStruct>();

	// Reset the arena
	arena.reset();

	// Allocate again, ensuring no fragmentation
	ExampleStruct* structPtr3 = arena.allocate<ExampleStruct>();
	ExampleStruct* structPtr4 = arena.allocate<ExampleStruct>();


	// Check for fragmentation (structPtr1 and structPtr2 should not be adjacent)
	TEST_ASSERT(reinterpret_cast<std::uintptr_t>(structPtr1) + sizeof(ExampleStruct) != reinterpret_cast<std::uintptr_t>(structPtr2));

	// Check for alignment
	TEST_ASSERT(reinterpret_cast<std::uintptr_t>(structPtr3) % alignof(ExampleStruct) == 0);
	TEST_ASSERT(reinterpret_cast<std::uintptr_t>(structPtr4) % alignof(ExampleStruct) == 0);

	// Release allocated memory
	arena.release(structPtr1);
	arena.release(structPtr2);
	arena.release(structPtr3);
	arena.release(structPtr4);

	// Alignment test after release
	ExampleStruct* structPtr5 = arena.allocate<ExampleStruct>();
	ExampleStruct* structPtr6 = arena.allocate<ExampleStruct>();

	// Check for alignment after release (structPtr5 and structPtr6 should be aligned)
	TEST_ASSERT(reinterpret_cast<std::uintptr_t>(structPtr5) % alignof(ExampleStruct) == 0);
	TEST_ASSERT(reinterpret_cast<std::uintptr_t>(structPtr6) % alignof(ExampleStruct) == 0);

	// Release additional allocated memory
	arena.release(structPtr5);
	arena.release(structPtr6);
	return true;
}



bool CMemoryArenaTest::speedTest()
{
	std::size_t arenaSize = 1024 * 1024;  // 1 MB
	CMemoryArena arena(arenaSize);

	const std::size_t numAllocations = 10000;
	const std::size_t allocationSize = sizeof(ExampleStruct);

	auto start = std::chrono::high_resolution_clock::now();

	// Perform a large number of allocations and deallocations
	for (std::size_t i = 0; i < numAllocations; ++i) {
		ExampleStruct* structPtr = arena.allocate<ExampleStruct>();
		arena.release(structPtr);
	}

	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	return true;
}



void threadedAllocation(int threadId, int numAllocations, CMemoryArena& memoryArena) {
	for (int i = 0; i < numAllocations; ++i) {
		int* data = memoryArena.allocate<int>();
		*data = threadId * 1000 + i;
		std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Simulate some work
	}
}

void threadedDeallocation(int threadId, int numAllocations, CMemoryArena& memoryArena) {
	for (int i = 0; i < numAllocations; ++i) {
		int* data = memoryArena.allocate<int>();
		// Perform some work with the data
		std::this_thread::sleep_for(std::chrono::milliseconds(10));
		memoryArena.release(data);
	}
}

bool CMemoryArenaTest::threadedTest()
{
	CMemoryArena memoryArena(1024 * 1024);

	const int numThreads = 4;
	const int numAllocationsPerThread = 100;

	std::vector<std::thread> threads;

	// Start allocation threads
	for (int i = 0; i < numThreads; ++i) {
		threads.push_back(std::thread([i, &memoryArena]() {
			threadedAllocation(i, numAllocationsPerThread, memoryArena);
		}));	
	}

	// Start deallocation threads
	for (int i = 0; i < numThreads; ++i) {
		threads.push_back(std::thread([i, &memoryArena]() {
			threadedDeallocation(i, numAllocationsPerThread, memoryArena);
		}));
	}

	// Wait for all threads to finish
	for (auto& thread : threads) {
		thread.join();
	}

	return true;
}

