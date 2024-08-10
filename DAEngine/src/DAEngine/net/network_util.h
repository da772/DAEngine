#pragma once

#include "dastd.h"

namespace da::net
{
	class CNetworkUtil
	{
	public:
		static std::string IPV4ToString(uint32_t ip);
	};
}
