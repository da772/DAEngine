#pragma once
#include "DAEngine/core/containers.h"

namespace da::core::maths
{
	class CHashString
	{
		public:
			CHashString(const char* str) {
#if DA_DEBUG
				//m_string = str;
#endif
				m_hash = genHash(str);
			}

			CHashString(const uint32_t hash) : m_hash(hash) {

			}

			const uint32_t hash() const
			{
				return m_hash;
			}

			bool operator ==(const CHashString& rhs) const {
				return rhs.m_hash == m_hash;
			}

			bool operator !=(const CHashString& rhs) const {
				return rhs.m_hash != m_hash;
			}

			bool operator ==(const char* rhs) const {
				return genHash(rhs) == m_hash;
			}

			bool operator !=(const char* rhs) const {
				return genHash(rhs) != m_hash;
			}

		private:
			/// <summary>
			/// AP Hash Function
			/// https://www.partow.net/programming/hashfunctions/#StringHashing
			/// </summary>
			/// <param name="str"></param>
			/// <returns></returns>
			inline uint32_t genHash(const char* str) const
			{
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

		private:
#if DA_DEBUG
			
#endif
			uint32_t m_hash;
	};
}

using namespace da::core::maths;