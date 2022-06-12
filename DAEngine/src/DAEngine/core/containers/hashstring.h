#pragma once
#include "DAEngine/core/containers.h"

namespace da::core::containers
{
	class CHashString
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

			inline CHashString() : 
#if defined(DA_DEBUG) || defined(DA_RELEASE)
				m_string(""), 
#endif
				m_hash(0) {

			}
			inline CHashString(const char* str) {
#if defined(DA_DEBUG) || defined(DA_RELEASE)
				m_string = str;
#endif
				m_hash = genHash(str);
			}

			inline CHashString(const char* str, size_t size) {
#if defined(DA_DEBUG) || defined(DA_RELEASE)
				m_string = str;
#endif
				m_hash = generateHash(str, size);
			}

			inline CHashString(const CString& str) {
#if defined(DA_DEBUG) || defined(DA_RELEASE)
				m_string = str;
#endif
				m_hash = generateHash(str.cstr(), str.size());
			}

			inline CHashString(const uint32_t hash) : m_hash(hash) {

			}

			inline const uint32_t hash() const
			{
				return m_hash;
			}

			inline bool operator ==(const CHashString& rhs) const {
				return rhs.m_hash == m_hash;
			}

			inline bool operator !=(const CHashString& rhs) const {
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
			uint32_t genHash(const char* str) const;
		private:
#if defined(DA_DEBUG) || defined(DA_RELEASE)
			CString m_string;
#endif
			uint32_t m_hash;
	};
}