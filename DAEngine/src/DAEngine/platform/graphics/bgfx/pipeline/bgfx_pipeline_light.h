#pragma once

#include "DAEngine/core/core.h"

#ifdef DA_GRAPHICS_BGFX

#include <bgfx/bgfx.h>

namespace da::platform {

	class CBgfxGraphicsMaterial;

	class CBgfxPipelineLight
	{
	public:

		CBgfxPipelineLight();
		void initialize();
		void shutdown();
		const uint32_t renderId() const;
		const CBgfxGraphicsMaterial* getMaterial() const;
		bgfx::TextureHandle getTexture() const;
		bgfx::FrameBufferHandle getFrameBufferHandle() const;

	private:
		bgfx::TextureHandle m_lightBufferTex;
		bgfx::FrameBufferHandle m_lightBuffer;
		CBgfxGraphicsMaterial* m_lightProgram;;


	};

}
#endif
