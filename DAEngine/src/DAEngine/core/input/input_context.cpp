#include "input_context.h"
#include "input.h"

namespace da
{
	CInputContext::CInputContext(CHashString ctx, uint32_t priority) : m_ctx(ctx), m_priority(priority)
	{
		CInput::pushInputContext(m_ctx, m_priority);
	}

	CInputContext::~CInputContext()
	{
		CInput::popInputContext(m_ctx);
	}

}