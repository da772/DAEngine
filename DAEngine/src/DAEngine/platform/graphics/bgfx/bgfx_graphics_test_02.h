#pragma once
#ifdef DA_GRAPHICS_BGFX
#include <bgfx/bgfx.h>
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
		da::core::CStaticMesh* m_smesh;
		bgfx::VertexBufferHandle m_vbh;
		bgfx::IndexBufferHandle m_ibh;
		void* m_material;
		da::core::CWindow* m_window;
		double m_start = 0;
		bgfx::UniformHandle m_uniform;
		bgfx::TextureHandle m_texture;

	};
}

#endif

