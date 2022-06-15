#include "dapch.h"
#include "hashstring.h"
#include <string.h>

namespace da::core::containers {

	uint32_t CHashString::genHash(const char* str) const {
		size_t size = 0;
		for (size_t x = 0; x < 512; x++) {
			if (str[x] == 0) {
				size = x++;
				break;
			}
		}

		return CHashString::generateHash(str, size);
	}

}
