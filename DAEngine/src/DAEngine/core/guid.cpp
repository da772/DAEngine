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

	CGuid::CGuid(const std::string& guid)
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
#ifdef DA_PLATFORM_WINDOWS
			sscanf_s((const char*)&guid[i], "%02x", &n);
#else
            sscanf((const char*)&guid[i], "%02x", &n);
#endif
			
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

	std::string CGuid::string() const
	{
		char ch[33];

		for (unsigned char i = 0; i < 16; i++) {
#ifdef DA_PLATFORM_WINDOWS
			sprintf_s(&ch[i*2], sizeof(ch)-((int)i*2*sizeof(char)), "%02x", m_uuid[i]);
#else
            snprintf(&ch[i*2], sizeof(ch)-((int)i*2*sizeof(char)), "%02x", m_uuid[i]);
#endif
		}

		ch[32] = 0;
		
		std::string result = ch;

		result.insert(result.begin()+8, '-');
		result.insert(result.begin() + 13, '-');
		result.insert(result.begin() + 18, '-');
		result.insert(result.begin() + 23, '-');

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
