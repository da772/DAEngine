#pragma once

#include "dapch.h"
#ifdef DA_GRAPHICS_BGFX
#include "bgfx_graphics_test_04.h"

namespace da::platform::bgfx {

	void CBgfxGraphicsTest04::Initialize(da::core::CWindow* window)
	{
		m_renderer.initialize();
	}

	void CBgfxGraphicsTest04::Render()
	{
		//m_renderer.update();
	}

	void CBgfxGraphicsTest04::Shutdown()
	{
		m_renderer.shutdown();
	}
}

#endif