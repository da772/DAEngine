#include "dapch.h"
#include "hashstring.h"
#include <string.h>

namespace da::core::maths {

	uint32_t CHashString::genHash(const char* str) const {
		
		uint32_t hash = 0xAAAAAAAA;
		uint32_t i = 0;
		const size_t size = strlen(str);

		for (i = 0; i < size; ++str, ++i)
		{
			hash ^= ((i & 1) == 0) ? ((hash << 7) ^ (*str) * (hash >> 3)) :
				(~((hash << 11) + ((*str) ^ (hash >> 5))));
		}

		return hash;

	}

}