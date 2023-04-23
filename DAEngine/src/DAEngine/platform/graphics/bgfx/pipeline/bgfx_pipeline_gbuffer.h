#pragma once

#include "DAEngine/core/core.h"

#ifdef DA_GRAPHICS_BGFX

#include <bgfx/bgfx.h>

namespace da::platform {

	class CBgfxGraphicsMaterial;

	class CBgfxPipelineGBuffer
	{
	public:

		CBgfxPipelineGBuffer();
		void initialize();
		void shutdown();
		constexpr const uint32_t renderId() const;
		const CBgfxGraphicsMaterial* getMaterial() const;
		bgfx::TextureHandle getNormalTexture() const;
		bgfx::TextureHandle getColorTexture() const;
		bgfx::TextureHandle getDepthTexture() const;
		bgfx::FrameBufferHandle getFrameBufferHandle() const;

	private:
		bgfx::TextureHandle m_gbufferTex[3];
		bgfx::FrameBufferHandle m_gbuffer;
		CBgfxGraphicsMaterial* m_gbufferProgram;;


	};

}
#endif
