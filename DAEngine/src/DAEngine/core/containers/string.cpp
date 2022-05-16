#include "dapch.h"
#include "string.h"

namespace da::core::containers
{

	CString::CString(const size_t& s) : m_s(s + 1)
	{
		m_ptr = (char*)malloc(sizeof(char) * m_s);
		ASSERT(m_ptr);
		m_ptr[0] = 0;
	}

	CString::CString(const char* s)
	{
		size_t size = strlen(s);
		m_s = size + 1;
		m_size = size;
		m_ptr = (char*)malloc(sizeof(char) * m_s);
		ASSERT(m_ptr);
		m_ptr[m_size] = 0;
		char* p = m_ptr;
		while (*p != 0 && p)
		{
			*(p++) = (char)(*(s++));
			ASSERT(p);
		}
	}

	CString::CString(const char* s, const size_t& amt)
	{
		size_t size = amt;
		m_s = size + 1;
		m_size = size;
		m_ptr = (char*)malloc(sizeof(char) * m_s);
		ASSERT(m_ptr);
		m_ptr[m_size] = 0;
		char* p = m_ptr;
		while (*p != 0 && p)
		{
			*(p++) = (char)(*(s++));
			ASSERT(p);
		}
	}

	CString::CString(const CString& st)
	{
		const char* s = st.cstring();
		size_t size = st.size();
		m_s = size + 1;
		m_size = size;
		m_ptr = (char*)malloc(sizeof(char) * m_s);
		ASSERT(m_ptr);
		m_ptr[m_size] = 0;
		char* p = m_ptr;
		while (*p != 0 && p)
		{
			*(p++) = (char)(*(s++));
			ASSERT(p);
		}
	}

	CString::CString()
	{
		m_ptr = (char*)malloc(sizeof(char) * 1);
		ASSERT(m_ptr);
		m_s = 1;
		m_ptr[0] = 0;
	}

	CString::CString(const char& s)
	{
		m_ptr = (char*)malloc(sizeof(char) * 2);
		ASSERT(m_ptr);
		m_s = 2;
		m_size = 1;
		m_ptr[0] = s;
		m_ptr[1] = 0;
	}

	CString::~CString()
	{
		ASSERT(m_ptr && m_s > 0);
		free(m_ptr);
	}


	size_t CString::size() const
	{
		return m_size;
	}

	void CString::clear()
	{
		m_size = 0;
		m_s = 0;
		m_ptr[0] = 0;
	}

	const char* CString::cstring() const
	{
		return m_ptr;
	}

	void CString::insert(const CString& str, const size_t& pos)
	{
		ASSERT(pos >= 0 && pos < m_size);
		if (m_size + str.size() >= m_s)
		{
			resize(m_size + str.size() + 1);
		}

		if (pos == m_size - 1)
		{
			memcpy(&m_ptr[m_size], str.cstring(), str.size() * sizeof(char));
			m_size += str.size();
			m_ptr[m_size] = 0;
			return;
		}

		ASSERT(pos + str.size() < m_s);

		memcpy(&m_ptr[pos + str.size()], &m_ptr[pos], (m_size - pos) * sizeof(char));
		m_size += str.size();

		memcpy(&m_ptr[pos], str.cstring(), str.size() * sizeof(char));

		m_ptr[m_size] = 0;
	}

	void CString::insert(const char& s, const size_t& pos)
	{
		insert(CString(s), pos);
	}

	void CString::append(const CString& s)
	{
		ASSERT(s.m_ptr);
		append(s.m_ptr);
	}

	void CString::append(const char* s)
	{
		ASSERT(s);

		size_t size = strlen(s);

		if (m_s - m_size > size)
		{
			memcpy(&m_ptr[m_size + 1], s, size * sizeof(char));
			m_size += size;
			m_ptr[m_size] = 0;
			return;
		}

		if (m_s - m_size <= size)
		{
			m_s += m_s - m_size != size ? size : 1;
			resize(m_s);
			memcpy(&m_ptr[m_size], s, size * sizeof(char));
			m_size += size;
			m_ptr[m_size] = 0;
		}

	}

	void CString::append(const char& ch)
	{
		ASSERT(ch != 0);

		if (m_s - m_size >= 1)
		{
			m_ptr[m_size] = ch;
			m_ptr[++m_size] = 0;
			return;
		}

		resize(++m_size);
		m_ptr[m_size] = 0;
		m_ptr[m_size - 1] = ch;
	}

	void CString::removeAt(const size_t& pos)
	{
		ASSERT(pos < m_size&& pos >= 0);

		if (pos == m_size - 1)
		{
			m_ptr[--m_size] = 0;
			resize(m_size + 1);
			return;
		}

		if (pos == 0)
		{
			m_size--;
			memcpy(m_ptr, &m_ptr[1], m_size * sizeof(char));
			resize(m_size + 1);
			m_ptr[m_size] = 0;
			return;
		}

		memcpy(&m_ptr[pos], &m_ptr[pos + 1], (m_size - pos) * sizeof(char));
		m_size--;
		resize(m_size + 1);
	}

	void CString::removeRange(const size_t& start, const size_t& end)
	{
		ASSERT(start >= 0);
		ASSERT(end <= m_size - 1);
		ASSERT(start <= end);

		if (end == m_size - 1)
		{
			m_size -= (end - start) + 1;
			m_ptr[start] = 0;
			resize(m_size + 1);
			return;
		}

		memcpy(&m_ptr[start], &m_ptr[end + 1], (m_size - (end + 1)) * sizeof(char));

		m_size -= (end - start) + 1;
		m_ptr[m_size + 1] = 0;
		resize(m_size + 1);
	}

	CString CString::substr(const size_t& s, const size_t& e)
	{
		return CString(&m_ptr[s], e - s);
	}

	size_t CString::find(char c)
	{
		for (size_t i = 0; i < m_size; i++)
		{
			if (m_ptr[i] == c)
			{
				return i;
			}
		}

		return -1;
	}

	size_t CString::findLast(char c)
	{
		for (int i = (int)m_size - 1; i >= 0; i--)
		{
			if (m_ptr[i] == c)
			{
				return i;
			}
		}

		return -1;
	}

	bool CString::remove(char c)
	{
		size_t p = find(c);
		if (p != -1)
		{
			removeAt(p);
			return true;
		}

		return false;
	}

	bool CString::remove(const CString& s)
	{
		size_t size = s.size();

		if (size > m_size)
		{
			return false;
		}

		for (size_t i = 0; i < m_size; i++)
		{
			if (i + size > m_size)
			{
				return false;
			}

			for (size_t j = 0; j < size; j++)
			{
				if (m_ptr[i + j] != s[j])
				{
					break;
				}

				if (j == size - 1)
				{
					removeRange(i, i + size - 1);
					return true;
				}
			}

		}

		return false;
	}

	bool CString::removeLast(const CString& s)
	{
		size_t size = s.size();

		if (size > m_size)
		{
			return false;
		}

		for (int i = (int)m_size - 1; i >= 0; i--)
		{
			if (i - size < 0)
			{
				return false;
			}

			for (size_t j = 0; j < i; j++)
			{
				if (m_ptr[i - j] != s[size - 1 - j])
				{
					break;
				}

				if (j == size - 1)
				{
					removeRange((size_t)i - j, i);
					return true;
				}
			}

		}

		return false;
	}


	bool CString::removeLast(char c)
	{
		size_t p = findLast(c);
		if (p != -1)
		{
			removeAt(p);
			return true;
		}

		return false;
	}

	bool CString::contains(char c)
	{
		return find(c) != -1;
	}

	bool CString::contains(const char* s)
	{
		size_t size = strlen(s);

		if (size > m_size)
		{
			return false;
		}

		for (size_t i = 0; i < m_size; i++)
		{
			if (i + size > m_size)
			{
				return false;
			}

			for (size_t j = 0; j < size; j++)
			{
				if (m_ptr[i + j] != s[j])
				{
					break;
				}
			}

		}

		return false;
	}

	bool CString::contains(const CString& s)
	{
		size_t size = s.m_size;

		if (size > m_size)
		{
			return false;
		}

		for (size_t i = 0; i < m_size; i++)
		{
			if (i + size > m_size)
			{
				return false;
			}

			for (size_t j = 0; j < size; j++)
			{
				if (m_ptr[i + j] != s[j])
				{
					break;
				}

				if (j == size - 1)
				{
					return true;
				}
			}

		}

		return false;
	}

	int CString::compare(const char* s)
	{
		size_t size = strlen(s);

		if (m_size != size)
		{
			return m_size > size ? 1 : -1;
		}

		size_t s1 = 0, s2 = 0;

		for (size_t i = 0; i < size; i++)
		{
			s1 += m_ptr[i];
			s2 += s[i];
		}

		return s1 == s2 ? 0 : s1 > s2 ? 1 : -1;
	}

	int CString::compare(const CString& s)
	{
		return compare(s.cstring());
	}

	bool CString::operator==(const CString& rhs)
	{
		if (rhs.size() != m_size)
		{
			return false;
		}

		for (size_t i = 0; i < m_size; i++)
		{
			if (rhs[i] != m_ptr[i])
			{
				return false;
			}
		}

		return true;
	}

	bool CString::operator==(const char* rhs)
	{
		size_t size = strlen(rhs);

		if (size != m_size)
		{
			return false;
		}

		for (size_t i = 0; i < size; i++)
		{
			if (m_ptr[i] != rhs[i])
			{
				return false;
			}
		}

		return true;
	}

	char& CString::operator[](size_t i)
	{
		ASSERT(i < m_size);
		return m_ptr[i];
	}

	char CString::operator[](size_t i) const
	{
		ASSERT(i < m_size);
		return m_ptr[i];
	}

	CString& CString::operator+=(const char& rhs)
	{
		ASSERT(rhs != 0);
		append(rhs);
		return *this;
	}

	CString& CString::operator+=(const CString& rhs)
	{
		ASSERT(rhs.m_ptr);
		append(rhs.m_ptr);
		return *this;
	}

	CString& CString::operator+=(const char* rhs)
	{
		ASSERT(rhs);
		append(rhs);
		return *this;
	}

	void CString::resize(const size_t& size)
	{
		m_s = size;
		char* p = (char*)realloc(m_ptr, m_s * sizeof(char));
		ASSERT(p);
		m_ptr = p;
	}

	da::core::containers::TEnumerator<char> CString::begin() const
	{
		return m_ptr;
	}

	da::core::containers::TEnumerator<char> CString::end() const
	{
		return &m_ptr[m_size-1];
	}

	CString operator+(const CString& lhs, const CString& rhs)
	{
		CString r(lhs);
		r.append(rhs);
		return r;
	}

	CString operator+(const CString& lhs, const char* rhs)
	{
		CString r(lhs);
		r.append(rhs);
		return r;
	}

	CString operator+(const char* lhs, const CString& rhs)
	{
		CString r(lhs);
		r.append(rhs);
		return r;
	}

}