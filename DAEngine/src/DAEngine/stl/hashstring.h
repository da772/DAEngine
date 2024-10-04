#pragma once
#include "core.h"
#include <stdint.h>
#include <string>
#include <algorithm>

#ifdef DA_REVIEW
#define HASHSTR(x) CHashString(da::CBasicHashString::generateHash(x), x)
#else
#define HASHSTR(x) CHashString(da::CBasicHashString::generateHash(x))	
#endif

#define HASH(x) CHashString::generateHash(x)
#define HASHSZ(x,y) CHashString::generateHash(x,y)
#define HASHCOMBINE(x, y) CHashString::hashCombine(x,y)

namespace da
{
	class CBasicHashString
	{
		public:

			static inline constexpr const uint64_t generateHash(const char* data, const size_t& len) {
				uint64_t h = 0xCBF29CE484222325;  // Initial hash value (from FNV-1a algorithm)
				const uint64_t prime = 0x100000001B3;  // Prime multiplier

				for (uint32_t i = 0; i < len; ++i) {
					h ^= static_cast<uint64_t>(data[i]);  // XOR the input byte into the hash
					h *= prime;  // Multiply by prime (FNV-1a step)
				}

				// Optionally, we can add further mixing steps for better hash quality
				h ^= h >> 33;
				h *= 0xff51afd7ed558ccd;
				h ^= h >> 33;
				h *= 0xc4ceb9fe1a85ec53;
				h ^= h >> 33;

				return h;
			}

			static inline constexpr const uint64_t generateHash(const char* str)
			{
				size_t size = std::char_traits<char>::length(str);
				return generateHash(str, size);
			}

			static inline constexpr const uint64_t hashCombine(uint64_t lhs, uint64_t rhs)
			{
				return rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
			}

			inline constexpr CBasicHashString() :
#ifdef DA_REVIEW
				m_string(""), 
#endif
				m_hash(0) {

			}
			inline constexpr CBasicHashString(const char* str) {
#ifdef DA_REVIEW
				m_string = std::string(str);
#endif
				m_hash = generateHash(str);
			}

			inline constexpr CBasicHashString(uint64_t hash, const char* str) {
#ifdef DA_REVIEW
				m_string = std::string(str);
#endif
				m_hash = hash;
			}

			inline constexpr CBasicHashString(const char* str, size_t size) {
#ifdef DA_REVIEW
				m_string = std::string(str);
#endif
				m_hash = generateHash(str, size);
			}

			inline constexpr CBasicHashString(uint64_t lhs, uint64_t rhs) {
				m_hash ^= rhs + 0x9e3779b9 + (lhs << 6) + (lhs >> 2);
#ifdef DA_REVIEW
				m_string = std::to_string(m_hash);
#endif
			}

			inline constexpr CBasicHashString(const uint64_t hash) : m_hash(hash) {

			}

			inline ~CBasicHashString()
			{

			}

			inline const uint64_t hash() const
			{
				return m_hash;
			}

			inline constexpr const char* c_str() const
			{
#ifdef DA_REVIEW
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
            inline constexpr uint64_t genHash(const char* str) const {
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
#ifdef DA_REVIEW
			std::string m_string;
#endif
			uint64_t m_hash;
	};
}


namespace std
{
	template<> struct hash<da::CBasicHashString>
	{
		std::size_t operator()(da::CBasicHashString const& hashStr) const noexcept
		{
			return hashStr.hash();
		}
	};

	template<> struct less<da::CBasicHashString>
	{
		bool operator() (const da::CBasicHashString& lhs, const da::CBasicHashString& rhs) const
		{
#ifdef DA_REVIEW

			return strcmp(lhs.c_str(), rhs.c_str()) < 0;
#else
			return lhs.hash() < rhs.hash();
#endif
		}
	};
}

typedef da::CBasicHashString CHashString;