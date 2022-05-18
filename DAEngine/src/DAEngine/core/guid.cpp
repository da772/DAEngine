#include "dapch.h"
#include "guid.h"

#include <time.h>
#include <stdlib.h>

namespace da::core {

	unsigned int CGuid::sm_seed = 0;

	CGuid::CGuid()
	{
		memset(&m_uuid, 0, sizeof(uint128_t));
	}

	CGuid::CGuid(const uint128_t guid)
	{
		memcpy(m_uuid, guid, sizeof(uint128_t));
	}

	CGuid::CGuid(const CGuid& guid)
	{
		memcpy(m_uuid, guid.m_uuid, sizeof(uint128_t));
	}

	CGuid::CGuid(const CString& guid)
	{
		if (guid.size() != 36) {
			memset(&m_uuid, 0, sizeof(uint128_t));
			return;
		}

		for (unsigned char i = 0, j = 0; i < 36;) {

			if (guid[i] == '-') {
				i++;
				continue;
			}

			unsigned int n;
			sscanf_s((const char*)&guid[i], "%02x", &n);
			
			memcpy(&m_uuid[j++], &n, sizeof(unsigned int));
			i += 2;
		}
	}

	bool CGuid::isValid() const
	{
		for (unsigned char i = 0; i < 16; i++) {
			if (m_uuid[i] != 0) return true;
		}

		return false;
	}

	const unsigned char* const CGuid::data() const
	{
		return m_uuid;
	}

	CString CGuid::string() const
	{
		char ch[33];

		for (unsigned char i = 0; i < 16; i++) {
			sprintf_s(&ch[i*2], sizeof(ch)-((int)i*2*sizeof(char)), "%02x", m_uuid[i]);
		}

		ch[32] = 0;
		
		CString result = ch;

		result.insert('-', 8);
		result.insert('-', 13);
		result.insert('-', 18);
		result.insert('-', 23);

		return result;

	}

	bool CGuid::operator==(const CGuid& rhs) const
	{
		for (unsigned char i = 0; i < 16; i++) {
			if (m_uuid[i] != rhs.m_uuid[i]) return false;
		}

		return true;
	}

	bool CGuid::operator!=(const CGuid& rhs) const
	{
		for (unsigned char i = 0; i < 16; i++) {
			if (m_uuid[i] != rhs.m_uuid[i]) return true;
		}

		return false;
	}

	CGuid CGuid::Generate()
	{
		if (sm_seed == 0) {
			sm_seed = (unsigned int)time(NULL);
			srand(sm_seed);
		}

		CGuid guid;
		for (unsigned char i = 0; i < 4; i++)
		{
			int rnd = rand();
			memcpy(&guid.m_uuid[i * 4], &rnd, sizeof(int));
		}

		return guid;
	}
}