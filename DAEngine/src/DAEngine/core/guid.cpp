#include "dapch.h"
#include "guid.h"

#include <time.h>
#include <stdlib.h>

namespace da::core {

	unsigned int CGuid::sm_seed = 0;

	CGuid::CGuid()
	{
		memset(&m_uuid, 0, sizeof(uint128_t));
#if defined(DA_DEBUG) || defined(DA_REVIEW)
		genString();
#endif
	}

	CGuid::CGuid(const uint128_t guid)
	{
		memcpy(m_uuid, guid, sizeof(uint128_t));
#if defined(DA_DEBUG) || defined(DA_REVIEW)
		genString();
#endif
	}

	CGuid::CGuid(const CGuid& guid)
	{
		memcpy(m_uuid, guid.m_uuid, sizeof(uint128_t));
#if defined(DA_DEBUG) || defined(DA_REVIEW)
		genString();
#endif
	}

	CGuid::CGuid(const char* guid)
	{
		size_t size = strlen(guid);
		if (size != 36) {
			memset(&m_uuid, 0, sizeof(uint128_t));
#if defined(DA_DEBUG) || defined(DA_REVIEW)
			genString();
#endif
			return;
		}

		std::string s = guid;
		for (unsigned char i = 0, j = 0; i < 36;) {

			if (s[i] == '-') {
				i++;
				continue;
			}

			unsigned int n;
#ifdef DA_PLATFORM_WINDOWS
			sscanf_s((const char*)&s[i], "%02x", &n);
#else
            sscanf((const char*)&s[i], "%02x", &n);
#endif
			
;			memcpy(&m_uuid[j++], &n, sizeof(unsigned int));
			i += 2;
		}

#if defined(DA_DEBUG) || defined(DA_REVIEW)
		memcpy(m_debugName, guid, sizeof(m_debugName));
#endif
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

#if defined(DA_DEBUG) || defined(DA_REVIEW)
	const char* CGuid::c_str() const
	{
		return &m_debugName[0];
	}
#endif

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
#if defined(DA_DEBUG) || defined(DA_REVIEW)
		guid.genString();
#endif

		return guid;
	}

#if defined(DA_DEBUG) || defined(DA_REVIEW)
	void CGuid::genString()
	{
		int index = 0;

		char buffer[4];

		for (unsigned char i = 0; i < 8; i+= 2) {
#ifdef DA_PLATFORM_WINDOWS
			sprintf_s(&buffer[0], sizeof(buffer), "%02x", m_uuid[index++]);
#else
			sprintf(&buffer[0], 4, "%02x", m_uuid[index++]);
#endif

			memcpy(&m_debugName[i], &buffer[0], sizeof(char) * 2);

		}


		m_debugName[8] = '-';
	
		for (unsigned char i = 9; i < 13; i+= 2) {
#ifdef DA_PLATFORM_WINDOWS
			sprintf_s(&buffer[0], 4, "%02x", m_uuid[index++]);
#else
			sprintf(&buffer[0], 4, "%02x", m_uuid[index++]);
#endif
			memcpy(&m_debugName[i], &buffer[0], sizeof(char) * 2);
		}

		m_debugName[13] = '-';


		for (unsigned char i = 14; i < 18; i += 2) {
#ifdef DA_PLATFORM_WINDOWS
			sprintf_s(&buffer[0], 4, "%02x", m_uuid[index++]);
#else
			sprintf(&m_debugName[i], sizeof(m_debugName), "%02x", m_uuid[index++]);
#endif
			memcpy(&m_debugName[i], &buffer[0], sizeof(char) * 2);
		}

		m_debugName[18] = '-';
		
		for (unsigned char i = 19; i < 23; i += 2) {
#ifdef DA_PLATFORM_WINDOWS
			sprintf_s(&buffer[0],4, "%02x", m_uuid[index++]);
#else
			sprintf(&buffer[0], 4, "%02x", m_uuid[index++]);
#endif
			memcpy(&m_debugName[i], &buffer[0], sizeof(char) * 2);
		}

		m_debugName[23] = '-';

		for (unsigned char i = 24; i < 36; i += 2) {
#ifdef DA_PLATFORM_WINDOWS
			sprintf_s(&buffer[0], 4, "%02x", m_uuid[index++]);
#else
			sprintf(&buffer[0], 4, "%02x", m_uuid[index++]);
#endif
			memcpy(&m_debugName[i], &buffer[0], sizeof(char) * 2);
		}

		m_debugName[36] = 0;
	}
#endif

}
