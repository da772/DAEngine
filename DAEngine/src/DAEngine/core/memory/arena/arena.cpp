#include "dapch.h"
#include "arena.h"

namespace da::core::memory
{
	CArena::CArena(const size_t& size) : m_size(size), m_ptr(0), m_allocated(0)
	{
		m_base = (unsigned char*)malloc(size);
		assert(m_base);
	}

	CArena::~CArena()
	{
		free(m_base);
	}

	void* CArena::Allocate(const size_t& size)
	{
		if (m_ptr + size >= m_size)
		{
			assert(false);
			return nullptr;
		}

		void* p = &m_base[m_ptr];
		m_ptr += size;
		m_allocated += size;

		return p;
	}

	void CArena::Free()
	{
		free(m_base);
		m_ptr = 0;
		m_allocated = 0;
		m_size = 0;
	}
	
}