#include "dapch.h"
#include "arena.h"

namespace da
{
	namespace memory
	{
		Arena::Arena(const size_t& size) : m_size(size), m_ptr(0), m_allocated(0)
		{
			m_base = (unsigned char*)malloc(size);
			assert(m_base);
		}

		Arena::~Arena()
		{
			free(m_base);
		}

		void* Arena::Allocate(const size_t& size)
		{
			assert(m_ptr + size >= m_size);

			void* p = &m_base[m_ptr];
			m_ptr += m_size;
			m_allocated += size;

			return p;
		}

	}
}