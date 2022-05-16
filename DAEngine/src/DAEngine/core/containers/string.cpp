#include "dapch.h"
#include "string.h"

namespace da::core::containers
{
	

	CString::CString(const char* str)
	{
		const size_t len = strlen(str);
		resize(len+1);

		for (size_t i = 0; i < len; i++) {
			m_ptr[i] = str[i];
		}
		m_ptr[m_size - 1] = 0;
	}

	CString::CString(const CString& str)
	{
		const size_t len = str.size();
		resize(len + 1);

		for (size_t i = 0; i < len; i++) {
			m_ptr[i] = str[i];
		}
		m_ptr[m_size - 1] = 0;
	}

	CString::CString(const size_t& n)
	{
		resize(n);
		m_ptr[0] = 0;
	}

	CString::CString() : TList<char>()
	{

	}

	void CString::append(const CString& str)
	{
		const size_t lastSize = m_size;
		resize(m_size+str.size());
		for (size_t i = 0; i < str.size(); i++) {
			m_ptr[lastSize - 1 + i] = str[i];
		}
		m_ptr[m_size - 1] = 0;
	}

	void CString::append(const char* str)
	{
		const size_t len = strlen(str);
		const size_t lastSize = m_size;
		resize(m_size + len);
		for (size_t i = 0; i < len; i++) {
			m_ptr[lastSize - 1 + i] = str[i];
		}
		m_ptr[m_size - 1] = 0;
	}

	void CString::insert(const char* e, const size_t& n) {
		const size_t len = strlen(e);
		assert(len);
		assert(n < m_size);
		resize(m_size + len);

		memcpy(&m_ptr[n + len], &m_ptr[n], sizeof(char) * (size() - n - 1));

		for (size_t i = 0; i < len; i++) {
			m_ptr[n + i] = e[i];
		}
	}

	void CString::insert(const IEnumerable<char>& e, const size_t& n)
	{
		TList<char>::insert(e, n);
	}

	void CString::insert(char e, const size_t& n)
	{
		TList<char>::insert(e, n);
	}

	size_t CString::size() const
	{
		return m_size > 0 ? m_size - 1 : 0;
	}

	const char* CString::cstr() const
	{
		return m_ptr;
	}

	bool CString::operator==(const CString& rhs) const
	{
		if (rhs.size() != size()) return false;

		for (size_t i = 0; i < size(); i++) {
			if (m_ptr[i] != rhs[i]) return false;
		}

		return true;
	}

	bool CString::operator==(const char* rhs) const
	{
		const size_t len = strlen(rhs);

		if (len != size()) return false;

		for (size_t i = 0; i < len; i++) {
			if (m_ptr[i] != rhs[i]) return false;
		}

		return true;
	}

	bool CString::operator!=(const CString& rhs) const
	{
		if (rhs.size() != size()) return true;

		for (size_t i = 0; i < size(); i++) {
			if (m_ptr[i] != rhs[i]) return true;
		}

		return false;
	}

	bool CString::operator!=(const char* rhs) const
	{
		const size_t len = strlen(rhs);

		if (len != size()) return true;

		for (size_t i = 0; i < len; i++) {
			if (m_ptr[i] != rhs[i]) return true;
		}

		return false;
	}

	da::core::containers::CString& CString::operator=(const CString& rhs)
	{
		resize(rhs.size()+1);

		for (size_t i = 0; i < rhs.size(); i++) {
			m_ptr[i] = rhs[i];
		}

		return *this;
	}

	da::core::containers::CString& CString::operator=(CString&& rhs)
	{
		resize(rhs.size() + 1);

		for (size_t i = 0; i < rhs.size(); i++) {
			m_ptr[i] = rhs[i];
		}

		rhs.clear();

		return *this;
	}

	da::core::containers::CString& CString::operator=(const char* rhs)
	{
		const size_t len = strlen(rhs);
		resize(len+1);

		for (size_t i = 0; i < len; i++) {
			m_ptr[i] = rhs[i];
		}

		m_ptr[m_size - 1] = 0;

		return *this;
	}

	da::core::containers::CString& CString::operator+=(const CString& rhs)
	{
		append(rhs);
		return *this;
	}

	da::core::containers::CString& CString::operator+=(const char* rhs)
	{
		append(rhs);
		return *this;
	}

	CString CString::substr(const size_t& s, const size_t& e) const
	{
		assert(e);
		assert(s + e <= size());
		CString str(e + 1);
		str.m_size = e + 1;

		for (size_t i = s, j = 0; i <= e+s; i++, j++) {
			str.m_ptr[j] = m_ptr[i];
		}

		str.m_ptr[e+1] = 0;

		return str;
	}

	CString operator+(const CString& lhs, const CString& rhs)
	{
		CString rslt = CString(lhs);
		rslt.append(rhs);
		return rslt;

	}

	CString operator+(const CString& lhs, const char* rhs)
	{
		CString rslt = CString(lhs);
		rslt.append(rhs);
		return rslt;
	}

	CString operator+(const char* lhs, const CString& rhs)
	{
		CString rslt = CString(lhs);
		rslt.append(rhs);
		return rslt;
	}

}