#pragma once

#include "global_allocator.h"

namespace da::memory {

	class CMemoryScope {
	public:
		inline CMemoryScope(EMemoryLayer layer) : m_layer(layer) {
			da::memory::push_memory_layer(layer);
		}

		inline ~CMemoryScope() {
			da::memory::pop_memory_layer();
		}

	private:
		EMemoryLayer m_layer;


	};

}