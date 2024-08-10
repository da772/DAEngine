#pragma once
#include "core.h"
#include <stdint.h>
#include <string>
#include <algorithm>

#if defined(DA_DEBUG) || defined(DA_RELEASE)
#define HASHSTR(x) CHashString(x, da::core::containers::CBasicHashString::generateHash(x))
#else
#define HASHSTR(x) CHashString(da::core::containers::CBasicHashString::generateHash(x))	
#endif

#define HASH(x) CHashString::generateHash(x)

namespace da::core::containers
{
	class CBasicHashString
	{
		public:

			static inline constexpr const uint32_t generateHash(const char* str, const size_t& size) {
				uint32_t hash = 0xAAAAAAAA;
				uint32_t i = 0;
				if (!size) return 0;
				for (i = 0; i < size; ++str, ++i)
				{
					hash ^= ((i & 1) == 0) ? ((hash << 7) ^ (*str) * (hash >> 3)) :
						(~((hash << 11) + ((*str) ^ (hash >> 5))));
				}

				return hash;
			}

			static inline constexpr const uint32_t generateHash(const char* str)
			{
				size_t size = std::char_traits<char>::length(str);
				return generateHash(str, size);
			}

			inline constexpr CBasicHashString() :
#if defined(DA_DEBUG) || defined(DA_RELEASE)
				m_string(""), 
#endif
				m_hash(0) {

			}
			inline constexpr CBasicHashString(const char* str) {
#if defined(DA_DEBUG) || defined(DA_RELEASE)
				m_string = str;
#endif
				m_hash = generateHash(str);
			}

			inline constexpr CBasicHashString(const char* str, uint32_t hash) {
#if defined(DA_DEBUG) || defined(DA_RELEASE)
				m_string = str;
#endif
				m_hash = hash;
			}

			inline constexpr CBasicHashString(const char* str, size_t size) {
#if defined(DA_DEBUG) || defined(DA_RELEASE)
				m_string = str;
#endif
				m_hash = generateHash(str, size);
			}

			inline constexpr CBasicHashString(uint32_t lhs, uint32_t rhs) {
				m_hash ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
#ifdef DA_REVIEW
				m_string = std::to_string(m_hash);
#endif
			}

			inline constexpr CBasicHashString(const uint32_t hash) : m_hash(hash) {

			}

			inline ~CBasicHashString()
			{

			}

			inline const uint32_t hash() const
			{
				return m_hash;
			}

			inline constexpr const char* c_str() const
			{
#if defined(DA_DEBUG) || defined(DA_RELEASE)
				return m_string.c_str();
#else
				return std::to_string(m_hash).c_str();
#endif

			}

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
            inline constexpr uint32_t genHash(const char* str) const {
				if (str == nullptr) return 0;
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


namespace std
{
	template<> struct hash<da::core::containers::CBasicHashString>
	{
		std::size_t operator()(da::core::containers::CBasicHashString const& hashStr) const noexcept
		{
			return hashStr.hash();
		}
	};

	template<> struct less<da::core::containers::CBasicHashString>
	{
		bool operator() (const da::core::containers::CBasicHashString& lhs, const da::core::containers::CBasicHashString& rhs) const
		{
#ifdef DA_REVIEW

			return strcmp(lhs.c_str(), rhs.c_str()) < 0;
#else
			return lhs.hash() < rhs.hash();
#endif
		}
	};
}

typedef da::core::containers::CBasicHashString CHashString;