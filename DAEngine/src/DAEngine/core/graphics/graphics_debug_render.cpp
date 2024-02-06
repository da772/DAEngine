#include "dapch.h"
#include "graphics_debug_render.h"
#if defined(DA_DEBUG) || defined(DA_RELEASE)
namespace da::graphics
{
	da::graphics::CDebugRender* CDebugRender::s_renderer;

	void CDebugRender::setInstance(CDebugRender* renderer)
	{
		s_renderer = renderer;
	}

	da::graphics::CDebugRender* CDebugRender::getInstance()
	{
		return s_renderer;
	}


}
#endif