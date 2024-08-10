#pragma once

#include "core/core.h"

namespace da::core
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
