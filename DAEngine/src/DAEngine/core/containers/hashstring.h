#pragma once
#include "DAEngine/core/containers.h"
#include "DAEngine/core/memory/global_allocator.h"

namespace da::core::containers
{
	template <typename _Allocator = memory::CGlobalAllocator>
	class CBasicHashString
	{
		public:

			static inline constexpr const uint32_t generateHash(const char* str, const size_t& size) {
				uint32_t hash = 0xAAAAAAAA;
				uint32_t i = 0;

				for (i = 0; i < size; ++str, ++i)
				{
					hash ^= ((i & 1) == 0) ? ((hash << 7) ^ (*str) * (hash >> 3)) :
						(~((hash << 11) + ((*str) ^ (hash >> 5))));
				}

				return hash;
			}

			inline CBasicHashString() :
#if defined(DA_DEBUG) || defined(DA_RELEASE)
				m_string(""), 
#endif
				m_hash(0) {

			}
			inline CBasicHashString(const char* str) {
#if defined(DA_DEBUG) || defined(DA_RELEASE)
				m_string = str;
#endif
				m_hash = genHash(str);
			}

			inline CBasicHashString(const char* str, size_t size) {
#if defined(DA_DEBUG) || defined(DA_RELEASE)
				m_string = str;
#endif
				m_hash = generateHash(str, size);
			}

			inline CBasicHashString(const CBasicString<_Allocator>& str) {
#if defined(DA_DEBUG) || defined(DA_RELEASE)
				m_string = str;
#endif
				m_hash = generateHash(str.cstr(), str.size());
			}

			inline CBasicHashString(const uint32_t hash) : m_hash(hash) {

			}

			inline ~CBasicHashString()
			{

			}

			inline const uint32_t hash() const
			{
				return m_hash;
			}

			inline const char* toStr() const
			{
#if defined(DA_DEBUG) || defined(DA_RELEASE)
				return m_string.cstr();
#else
				return nullptr;
#endif
			}

			inline bool operator ==(const CBasicHashString<_Allocator>& rhs) const {
				return rhs.m_hash == m_hash;
			}

			inline bool operator !=(const CBasicHashString<_Allocator>& rhs) const {
				return rhs.m_hash != m_hash;
			}

			inline bool operator ==(const char* rhs) const {
				return genHash(rhs) == m_hash;
			}

			inline bool operator !=(const char* rhs) const {
				return genHash(rhs) != m_hash;
			}

		private:
			/// <summary>
			/// AP Hash Function
			/// https://www.partow.net/programming/hashfunctions/#StringHashing
			/// </summary>
			/// <param name="str"></param>
			/// <returns></returns>
            inline uint32_t genHash(const char* str) const {
                size_t size = 0;
                for (size_t x = 0; x < 512; x++) {
                    if (str[x] == 0) {
                        size = x++;
                        break;
                    }
                }

                return generateHash(str, size);
            }
		private:
#if defined(DA_DEBUG) || defined(DA_RELEASE)
			CBasicString<_Allocator> m_string;
#endif
			uint32_t m_hash;
	};
}
