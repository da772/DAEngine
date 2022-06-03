#include "dapch.h"
#include "hashstring.h"
#include <string.h>

namespace da::core::containers {

	uint32_t CHashString::genHash(const char* str) const {
		
		return generateHash(str);
	}

	constexpr const uint32_t CHashString::generateHash(const char* str )
	{
		uint32_t hash = 0xAAAAAAAA;
		uint32_t i = 0;
		size_t size = 0;
		for (size_t x = 0; x < 256; x++) {
			if (str[x] == 0) {
				size = ++x;
				break;
			}
		}

		for (i = 0; i < size; ++str, ++i)
		{
			hash ^= ((i & 1) == 0) ? ((hash << 7) ^ (*str) * (hash >> 3)) :
				(~((hash << 11) + ((*str) ^ (hash >> 5))));
		}

		return hash;
	}

}