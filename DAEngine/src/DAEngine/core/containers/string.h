#pragma once
#include "list.h"

namespace da::core::containers
{
	class CString : TList<char> {
	public:

	};

	CString operator+(const CString& lhs, const CString& rhs);
	CString operator+(const CString& lhs, const char* rhs);
	CString operator+(const char* lhs, const CString& rhs);
}