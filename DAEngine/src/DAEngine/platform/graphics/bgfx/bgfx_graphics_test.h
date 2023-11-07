#pragma once

#ifdef DA_GRAPHICS_BGFX
#include <bgfx/bgfx.h>

namespace da::core { class CWindow; }

namespace da::platform {

	class CBgfxGraphicsTest {

	public:
		void Initialize(class da::core::CWindow* window);
		void Render();
		void Shutdown();

	private:
		da::core::CWindow* m_window;
		void* m_test = nullptr;
		bool m_show = true;
		int m_testIndex = 3;
	};

}
#endif