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
		// Resource handles
		bgfx::VertexBufferHandle m_vbh;
		bgfx::IndexBufferHandle m_ibh;
		void* m_material;

		bool m_r = true;
		bool m_g = true;
		bool m_b = true;
		bool m_a = true;
		int64_t m_timeOffset;

		da::core::CWindow* m_window;
	};

}
#endif