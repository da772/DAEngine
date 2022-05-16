#pragma once
#include "DAEngine/core/containers.h"

namespace da::core::maths
{
	class CHashString
	{
		public:
			CHashString(const char* str) {
#if DA_DEBUG
				m_string = str;
#endif
				m_hash = genHash(str);
			}

			CHashString(const CString& str) {
#if DA_DEBUG
				m_string = str;
#endif
				m_hash = genHash(str.cstr());
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
			uint32_t genHash(const char* str) const;
		private:
#if DA_DEBUG
			CString m_string;
#endif
			uint32_t m_hash;
	};
}

using namespace da::core::maths;