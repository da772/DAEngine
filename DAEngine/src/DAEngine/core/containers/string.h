#pragma once
#include "list.h"

namespace da::core::containers
{
	class CString : public TList<char> {
	public:
		CString();
		CString(const size_t& n);
		CString(const char* str);
		CString(const CString& str);
		CString(const IEnumerable<char>& e);

		void append(const CString& str);
		void append(const char* str);

		void insert(const char* e, const size_t& n);
		void insert(const IEnumerable<char>& e, const size_t& n);
		void insert(char e, const size_t& n);

		CString substr(const size_t& s, const size_t& e) const;

 		virtual size_t size() const override;
		const char* cstr() const;
		
		/*	TODO: OR add to IEnumerator<T>
			- substr ?? 
			- find CString ?? 
			- replace ?? 
		*/

		bool operator ==(const CString& rhs) const;
		bool operator ==(const char* rhs) const;
		bool operator !=(const char* rhs) const;
		bool operator !=(const CString& rhs) const;
		CString& operator=(const CString& rhs);
		CString& operator=(const char* rhs);
		CString& operator=(CString&& rhs);
		CString& operator +=(const CString& rhs);
		CString& operator +=(const char* rhs);

	};

	CString operator+(const CString& lhs, const CString& rhs);
	CString operator+(const CString& lhs, const char* rhs);
	CString operator+(const char* lhs, const CString& rhs);
}