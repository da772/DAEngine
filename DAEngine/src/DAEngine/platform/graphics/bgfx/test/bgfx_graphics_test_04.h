#pragma once

#ifdef DA_GRAPHICS_BGFX
#include "../bgfx_renderer.h"
#include <bgfx/bgfx.h>
namespace da::core { class CWindow; class CStaticMesh; class CCamera; }

namespace da::platform::bgfx
{
	class CBgfxGraphicsMaterial;
	class CBgfxPipelineGBuffer;
	class CBgfxPipelineShadow;
	class CBgfxPipelineLight;

	class CBgfxGraphicsTest04
	{
	public:
		void Initialize(da::core::CWindow* window);
		void Render();
		void Shutdown();

	private:
		CBgfxRenderer m_renderer;

	};
}

#endif

