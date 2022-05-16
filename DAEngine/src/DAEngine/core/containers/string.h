#pragma once
#include "array.h"

namespace da::core::containers
{
	class CString : IEnumerable<char> {
	public:
		CString();
		CString(const CString& s);
		CString(const char& s);
		CString(const char32_t& s);
		CString(const wchar_t& s);
		CString(const char* s);
		CString(const char* s, const size_t& amt);
		CString(const char32_t* s);
		CString(const wchar_t* s);
		CString(const size_t& s);
		~CString();


		TEnumerator<char> begin() const override;


		TEnumerator<char> end() const override;

	public:
		virtual size_t size() const override;
		void clear();
		const char* cstring() const;
		bool isWString() const;
		bool isU32String() const;
		void insert(const CString& s, const size_t& pos);
		void insert(const char& s, const size_t& pos);
		void append(const CString& s);
		void append(const char* s);
		void append(const char& ch);
		void removeAt(const size_t& pos);
		void removeRange(const size_t& start, const size_t& end);
		CString substr(const size_t& s, const size_t& e);
		size_t find(char c);
		size_t findLast(char c);
		bool remove(const CString& s);
		bool remove(char c);
		bool removeLast(char c);
		bool removeLast(const CString& s);
		bool contains(char c);
		bool contains(const char* s);
		bool contains(const CString& c);
		int compare(const char* s);
		int compare(const CString& s);

	public:
		bool operator==(const char* rhs);
		bool operator==(const CString& rhs);
		CString& operator+=(const char* rhs);
		CString& operator+=(const char& rhs);
		CString& operator+=(const CString& rhs);
		char operator [](size_t i) const;
		char& operator [](size_t i);

	private:
		void resize(const size_t& size);

	private:
		char* m_ptr = nullptr;
		size_t m_size = 0;
		size_t m_s = 0;

	};

	CString operator+(const CString& lhs, const CString& rhs);
	CString operator+(const CString& lhs, const char* rhs);
	CString operator+(const char* lhs, const CString& rhs);
}