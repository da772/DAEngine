#include "memory_test.h"
#include <DAEngine/core/memory/memory.h>
#include <DAEngine/core/containers.h>
#include <DAEngine/core/memory/global_allocator.h>

CMemoryTest::CMemoryTest()
{

}

bool CMemoryTest::RunTests()
{
	TEST_BEGIN();
	TEST_FUNC(CMemoryLayerTest);
	TEST_END();
}

bool CMemoryTest::CMemoryLayerTest()
{
	da::memory::push_memory_layer((da::memory::EMemoryLayer)1);
	TEST_ASSERT(da::memory::peek_memory_layer() == 1);
	da::memory::push_memory_layer((da::memory::EMemoryLayer)2);
	TEST_ASSERT(da::memory::peek_memory_layer() == 2);
	da::memory::push_memory_layer((da::memory::EMemoryLayer)3);
	TEST_ASSERT(da::memory::peek_memory_layer() == 3);
	da::memory::pop_memory_layer();
	TEST_ASSERT(da::memory::peek_memory_layer() == 2);
	da::memory::pop_memory_layer();
	TEST_ASSERT(da::memory::peek_memory_layer() == 1);
	da::memory::pop_memory_layer();
	TEST_ASSERT(da::memory::peek_memory_layer() == 0);

	da::memory::push_memory_layer((da::memory::EMemoryLayer)1);
	void* ptr = allocate(10);
	TEST_ASSERT(da::memory::get_memory_layers()[1] == 10 + sizeof(size_t));
	ptr = reallocate(ptr, 15);
	TEST_ASSERT(da::memory::get_memory_layers()[1] == 15 + sizeof(size_t));
	ptr = reallocate(ptr, 5);
	TEST_ASSERT(da::memory::get_memory_layers()[1] == 5 + sizeof(size_t));
	deallocate(ptr);
	TEST_ASSERT(da::memory::get_memory_layers()[1] == 0);

	da::memory::push_memory_layer((da::memory::EMemoryLayer)2);
	ptr = allocate(10);
	TEST_ASSERT(da::memory::get_memory_layers()[2] == 10 + sizeof(size_t));
	ptr = reallocate(ptr, 15);
	TEST_ASSERT(da::memory::get_memory_layers()[2] == 15 + sizeof(size_t));
	ptr = reallocate(ptr, 5);
	TEST_ASSERT(da::memory::get_memory_layers()[2] == 5 + sizeof(size_t));
	deallocate(ptr);
	TEST_ASSERT(da::memory::get_memory_layers()[2] == 0);

	da::memory::push_memory_layer((da::memory::EMemoryLayer)3);
	ptr = allocate(10);
	TEST_ASSERT(da::memory::get_memory_layers()[3] == 10 + sizeof(size_t));
	ptr = reallocate(ptr, 15);
	TEST_ASSERT(da::memory::get_memory_layers()[3] == 15 + sizeof(size_t));
	ptr = reallocate(ptr, 5);
	TEST_ASSERT(da::memory::get_memory_layers()[3] == 5 + sizeof(size_t));
	deallocate(ptr);
	TEST_ASSERT(da::memory::get_memory_layers()[3] == 0);

	da::memory::pop_memory_layer();
	da::memory::pop_memory_layer();
	da::memory::pop_memory_layer();

	TEST_ASSERT(da::memory::peek_memory_layer() == 0);

	TList<int, da::memory::CCoreAllocator> lst;

	lst.push(1);
	lst.push(2);
	lst.push(3);
	lst.push(4);
	{
		size_t sz = sizeof(int) * 8 + sizeof(size_t);
		TEST_ASSERT(da::memory::get_memory_layers()[1] == sz);
		TList<int, da::memory::CDebugAllocator> lst2;
		lst2.push(1);
		lst2.push(2);
		lst2.push(3);
		lst2.push(4);
		lst2.push(5);
		lst2.push(6);
		TEST_ASSERT(da::memory::get_memory_layers()[5] == sz);
		lst.clear();
		lst2.push(5);
		TEST_ASSERT(da::memory::get_memory_layers()[5] == sz);
		lst2.clear();
	}
	TEST_ASSERT(da::memory::get_memory_layers()[1] == 0);
	TEST_ASSERT(da::memory::get_memory_layers()[5] == 0);
	


	return true;
}
