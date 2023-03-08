#pragma once
#include "list.h"

namespace da::core::containers
{
	template<typename _Allocator = memory::CGlobalAllocator>
	class CBasicString : public TList<char, _Allocator, 0> {
	public:
		CBasicString(const char* str)
		{
			const size_t len = strlen(str);
			TList<char, _Allocator, 0>::resize(len + 1);
			memcpy((void*)TList<char, _Allocator, 0>::m_ptr, (void*)str, len);
			TList<char, _Allocator, 0>::m_ptr[len] = 0;
		}

		CBasicString(const CBasicString<_Allocator>& str)
		{
			TList<char, _Allocator, 0>::resize(str.size()+1);
			memcpy((void*)TList<char, _Allocator, 0>::m_ptr, (void*)str.begin().get(), str.size());
			TList<char, _Allocator, 0>::m_ptr[str.size()] = 0;
		}

		CBasicString(const size_t& n) : TList<char, _Allocator, 0>(n)
		{
			TList<char, _Allocator, 0>::m_ptr[0] = 0;
		}

		CBasicString() : TList<char, _Allocator, 0>()
		{
		}

		CBasicString(const IEnumerable<char>& e) : TList<char, _Allocator, 0>(e)
		{
			TList<char, _Allocator, 0>::resize(e.size() + 1);
			memcpy((void*)TList<char, _Allocator, 0>::m_ptr, (void*)e.begin().get(), e.size());
			TList<char, _Allocator, 0>::m_ptr[e.size()] = 0;
		}

		void append(const CBasicString<_Allocator>& str)
		{
			const size_t lastSize = TList<char, _Allocator, 0>::m_size;
			TList<char, _Allocator, 0>::resize(TList<char, _Allocator, 0>::m_size + str.size());
			for (size_t i = 0; i < str.size(); i++) {
				TList<char, _Allocator, 0>::m_ptr[lastSize - 1 + i] = str[i];
			}
			TList<char, _Allocator, 0>::m_ptr[TList<char, _Allocator, 0>::m_size - 1] = 0;
		}

		void append(const char* str)
		{
			const size_t len = strlen(str);
			const size_t lastSize = TList<char, _Allocator, 0>::m_size;
			TList<char, _Allocator, 0>::resize(TList<char, _Allocator, 0>::m_size + len);
			for (size_t i = 0; i < len; i++) {
				TList<char, _Allocator, 0>::m_ptr[lastSize - 1 + i] = str[i];
			}
			TList<char, _Allocator, 0>::m_ptr[TList<char, _Allocator, 0>::m_size - 1] = 0;
		}

		void insert(const char* e, const size_t& n) {

			if (n == TList<char, _Allocator, 0>::m_size - 1) {
				append(e);
				return;
			}

			const size_t len = strlen(e);
			ASSERT(len);

			//ASSERT(n < TList<char, _Allocator, 0>::m_size);
			TList<char, _Allocator, 0>::resize(TList<char, _Allocator, 0>::m_size + len);

			memorycopy(&TList<char, _Allocator, 0>::m_ptr[n + len], &TList<char, _Allocator, 0>::m_ptr[n], sizeof(char) * (size() - n - 1));

			for (size_t i = 0; i < len; i++) {
				TList<char, _Allocator, 0>::m_ptr[n + i] = e[i];
			}
		}

		void insert(const IEnumerable<char>& e, const size_t& n)
		{
			TList<char, _Allocator, 0>::insert(e, n);
		}

		void insert(char e, const size_t& n)
		{
			TList<char, _Allocator, 0>::insert(e, n);
		}

		size_t size() const
		{
			return TList<char, _Allocator, 0>::m_size > 0 ? TList<char, _Allocator, 0>::m_size - 1 : 0;
		}

		const char* cstr() const
		{
			return TList<char, _Allocator, 0>::m_ptr;
		}

		bool operator==(const CBasicString<_Allocator>& rhs) const
		{
			if (rhs.size() != size()) return false;

			for (size_t i = 0; i < size(); i++) {
				if (TList<char, _Allocator, 0>::m_ptr[i] != rhs[i]) return false;
			}

			return true;
		}

		bool operator==(const char* rhs) const
		{
			const size_t len = strlen(rhs);

			if (len != size()) return false;

			for (size_t i = 0; i < len; i++) {
				if (TList<char, _Allocator, 0>::m_ptr[i] != rhs[i]) return false;
			}

			return true;
		}

		bool operator!=(const CBasicString<_Allocator>& rhs) const
		{
			if (rhs.size() != size()) return true;

			for (size_t i = 0; i < size(); i++) {
				if (TList<char, _Allocator, 0>::m_ptr[i] != rhs[i]) return true;
			}

			return false;
		}

		bool operator!=(const char* rhs) const
		{
			const size_t len = strlen(rhs);

			if (len != size()) return true;

			for (size_t i = 0; i < len; i++) {
				if (TList<char, _Allocator, 0>::m_ptr[i] != rhs[i]) return true;
			}

			return false;
		}

		da::core::containers::CBasicString<_Allocator>& operator=(const CBasicString<_Allocator>& rhs)
		{
			TList<char, _Allocator, 0>::resize(rhs.size() + 1);

			for (size_t i = 0; i < rhs.size(); i++) {
				TList<char, _Allocator, 0>::m_ptr[i] = rhs[i];
			}

			return *this;
		}

		// Move operator
		da::core::containers::CBasicString<_Allocator>& operator=(CBasicString<_Allocator>&& rhs)
		{
			TList<char, _Allocator, 0>::m_ptr = rhs.TList<char, _Allocator, 0>::m_ptr;
			TList<char, _Allocator, 0>::m_heapSize = rhs.TList<char, _Allocator, 0>::m_heapSize;
			TList<char, _Allocator, 0>::m_size = rhs.TList<char, _Allocator, 0>::m_size;
			rhs.TList<char, _Allocator, 0>::m_ptr = nullptr;
			rhs.TList<char, _Allocator, 0>::m_heapSize = 0;
			rhs.TList<char, _Allocator, 0>::m_size = 0;
			return *this;
		}

		da::core::containers::CBasicString<_Allocator>& operator=(const char* rhs)
		{
			const size_t len = strlen(rhs);
			TList<char, _Allocator, 0>::resize(len + 1);

			for (size_t i = 0; i < len; i++) {
				TList<char, _Allocator, 0>::m_ptr[i] = rhs[i];
			}

			TList<char, _Allocator, 0>::m_ptr[TList<char, _Allocator, 0>::m_size - 1] = 0;

			return *this;
		}

		da::core::containers::CBasicString<_Allocator>& operator+=(const CBasicString<_Allocator>& rhs)
		{
			append(rhs);
			return *this;
		}

		da::core::containers::CBasicString<_Allocator>& operator+=(const char* rhs)
		{
			append(rhs);
			return *this;
		}

		CBasicString<_Allocator> substr(const size_t& s, const size_t& e) const
		{
			ASSERT(e);
			ASSERT(s + e <= size());
			CBasicString str(e + 1);
			str.TList<char, _Allocator, 0>::m_size = e + 1;

			for (size_t i = s, j = 0; i <= e + s; i++, j++) {
				str.TList<char, _Allocator, 0>::m_ptr[j] = TList<char, _Allocator, 0>::m_ptr[i];
			}

			str.TList<char, _Allocator, 0>::m_ptr[e + 1] = 0;

			return str;
		}

		void remove(char e)
		{
			for (size_t i = 0; i < TList<char, _Allocator, 0>::m_size;) {
				if (TList<char, _Allocator, 0>::m_ptr[i] == e) {
					TList<char, _Allocator, 0>::remove(i);
					continue;
				}
				i++;
			}
		}

		~CBasicString()
		{

		}
	};

	template<typename _Allocator = memory::CGlobalAllocator>
	CBasicString<_Allocator> operator+(const CBasicString<_Allocator>& lhs, const CBasicString<_Allocator>& rhs)
	{
		CBasicString rslt = CBasicString(lhs);
		rslt.append(rhs);
		return rslt;
	}
	template<typename _Allocator = memory::CGlobalAllocator>
	CBasicString<_Allocator> operator+(const CBasicString<_Allocator>& lhs, const char* rhs)
	{
		CBasicString rslt = CBasicString(lhs);
		rslt.append(rhs);
		return rslt;
	}
	template<typename _Allocator = memory::CGlobalAllocator>
	CBasicString<_Allocator> operator+(const char* lhs, const CBasicString<_Allocator>& rhs)
	{
		CBasicString rslt = CBasicString(lhs);
		rslt.append(rhs);
		return rslt;
	}
}
