#pragma once
#include "DAEngine/core/containers.h"

namespace da::core::containers
{
#define HASH(x) CHashString::generateHash(#x)

	class CHashString
	{
		public:
			inline CHashString() : m_string(""), m_hash(0) {

			}
			inline CHashString(const char* str) {
#if defined(DA_DEBUG) || defined(DA_RELEASE)
				m_string = str;
#endif
				m_hash = genHash(str);
			}

			inline CHashString(const CString& str) {
#if defined(DA_DEBUG) || defined(DA_RELEASE)
				m_string = str;
#endif
				m_hash = genHash(str.cstr());
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

			static constexpr const uint32_t generateHash(const char*);

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