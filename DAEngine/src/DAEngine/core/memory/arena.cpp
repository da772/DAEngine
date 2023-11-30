#include "dapch.h"
#include "arena.h"

namespace da::memory
{
	CMemoryArena::CMemoryArena(std::size_t size) : arenaSize(size), memory(new std::uint8_t[size]), freeList(nullptr) {
		reset();
	}

	void* CMemoryArena::allocate(std::size_t size, std::size_t alignment)
	{
		std::lock_guard<std::mutex> lock(mutex);
		std::size_t padding = (alignment - (reinterpret_cast<std::uintptr_t>(freeList) % alignment)) % alignment;
		std::size_t alignedSize = size + padding;

		while (freeList != nullptr && freeList->size < alignedSize) {
			freeList = freeList->next;
		}

		if (freeList == nullptr) {
			// No suitable block found, allocate a new one
			Block* newBlock = reinterpret_cast<Block*>(memory);
			newBlock->size = arenaSize - sizeof(Block);
			newBlock->next = nullptr;
			freeList = newBlock;
		}

		// Return the allocated block
		void* result = reinterpret_cast<std::uint8_t*>(freeList) + sizeof(Block) + padding;

		// Update the free list pointer and size
		freeList = reinterpret_cast<Block*>(reinterpret_cast<std::uint8_t*>(result) + size);
		freeList->size = freeList->size - alignedSize;

		return result;
	}

	CMemoryArena::~CMemoryArena()
	{
		delete[] memory;
	}

	void CMemoryArena::release(void* ptr)
	{
		std::lock_guard<std::mutex> lock(mutex);

		// Find the block corresponding to the released pointer
		Block* releasedBlock = reinterpret_cast<Block*>(reinterpret_cast<std::uint8_t*>(ptr) - sizeof(Block));

		// Insert the released block at the beginning of the free list
		releasedBlock->next = freeList;
		freeList = releasedBlock;
	}

	void CMemoryArena::reset()
	{
		std::lock_guard<std::mutex> lock(mutex);

		// Reset the entire memory arena
		freeList = reinterpret_cast<Block*>(memory);
		freeList->size = arenaSize - sizeof(Block);
		freeList->next = nullptr;

		// Adjust the freeList size to exclude the size of the Block structure
		freeList->size -= sizeof(Block);

		// Merge adjacent free blocks after the reset
		mergeFreeBlocks();
	}

	void CMemoryArena::mergeFreeBlocks()
	{
		Block* current = freeList;
		while (current != nullptr && current->next != nullptr) {
			// Check if the current block and the next block are adjacent in memory
			if (reinterpret_cast<std::uint8_t*>(current) + current->size + sizeof(Block) == reinterpret_cast<std::uint8_t*>(current->next)) {
				// Merge the two blocks
				current->size += current->next->size + sizeof(Block);
				current->next = current->next->next;
			}
			else {
				current = current->next;
			}
		}
	}

}