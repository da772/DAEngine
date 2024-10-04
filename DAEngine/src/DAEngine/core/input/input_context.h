#pragma once

#include "core.h"
#include "stl/hashstring.h"

namespace da
{
	class CInputContext
	{
	public:
		CInputContext(CHashString ctx, uint32_t priority);
		~CInputContext();

	private:
		CHashString m_ctx;
		uint32_t m_priority;
	};
}
