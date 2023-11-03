#pragma once
#include "DAEngine/core/containers.h"
#include <stdint.h>
#include <string>

namespace da::core::containers
{
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

			inline CBasicHashString(const uint32_t hash) : m_hash(hash) {

			}

			inline ~CBasicHashString()
			{

			}

			inline const uint32_t hash() const
			{
				return m_hash;
			}
#if defined(DA_DEBUG) || defined(DA_RELEASE)
			inline const char* c_str() const
			{

				return m_string.c_str();

			}
#endif

			inline bool operator ==(const CBasicHashString& rhs) const {
				return rhs.m_hash == m_hash;
			}

			inline bool operator !=(const CBasicHashString& rhs) const {
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
			std::string m_string;
#endif
			uint32_t m_hash;
	};
}
