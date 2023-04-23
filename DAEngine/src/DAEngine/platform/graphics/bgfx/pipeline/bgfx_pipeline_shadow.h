#pragma once

#include "DAEngine/core/core.h"

#ifdef DA_GRAPHICS_BGFX

#include <bgfx/bgfx.h>

namespace da::platform {

	class CBgfxGraphicsMaterial;

	class CBgfxPipelineShadow
	{
	public:

		CBgfxPipelineShadow();
		void initialize();
		void shutdown();
		constexpr const uint32_t renderId() const;
		const CBgfxGraphicsMaterial* getMaterial() const;
		bgfx::TextureHandle getRSMTexture() const;
		bgfx::TextureHandle getDepthTexture() const;
		bgfx::FrameBufferHandle getFrameBufferHandle() const;

	private:
		bgfx::TextureHandle m_shadowBufferTex[2];
		bgfx::FrameBufferHandle m_shadowBuffer;
		CBgfxGraphicsMaterial* m_shadowProgram;;


	};

}
#endif
