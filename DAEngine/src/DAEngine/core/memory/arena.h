#pragma once

namespace da::memory
{
	class CMemoryArena
	{
	public:
		CMemoryArena(std::size_t size);
		~CMemoryArena();

		template <typename T>
		inline T* allocate(std::size_t count = 1)
		{
			std::size_t size = count * sizeof(T);
			std::size_t alignment = alignof(T);
			return (T*)allocate(size, alignment);
		}
		void* allocate(std::size_t size, std::size_t alignment);
		void release(void* ptr);
		void reset();

	private:
		struct Block {
			std::size_t size;
			Block* next;
		};

		std::size_t arenaSize;
		std::uint8_t* memory;
		Block* freeList;
		std::mutex mutex;


		void mergeFreeBlocks();
	};
}