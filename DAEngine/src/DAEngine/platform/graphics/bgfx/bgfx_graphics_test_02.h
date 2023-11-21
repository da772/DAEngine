#pragma once
#ifdef DA_GRAPHICS_BGFX
#include <bgfx/bgfx.h>
#include "bgfx_texture2d.h"
#include "bgfx_uniform.h"

namespace da::core { class CWindow; class CStaticMesh; }

namespace da::platform
{
	class CBgfxGraphicsTest02
	{
	public:
		void Initialize(da::core::CWindow* window);
		void Render();
		void Shutdown();

	private:
		class CBgfxStaticMesh* m_mesh;
		void* m_material;
		da::core::CWindow* m_window;
		double m_start = 0;
		da::platform::bgfx::CBgfxUniform<uint16_t> m_uniform;
		da::platform::bgfx::CBgfxTexture2D m_texture;

	};
}

#endif

