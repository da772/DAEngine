#include <dapch.h>
#include "bgfx_pipeline_gbuffer.h"

#ifdef DA_GRAPHICS_BGFX

#include "DAEngine/core/containers.h"
#include "DAEngine/platform/graphics/bgfx/bgfx_graphics_material.h"
#include <bx/math.h>

#define GBUFFER_RT_NORMAL 0
#define GBUFFER_RT_COLOR  1
#define GBUFFER_RT_DEPTH  2

namespace da::platform {


	CBgfxPipelineGBuffer::CBgfxPipelineGBuffer()
	{

	}

	const uint32_t CBgfxPipelineGBuffer::renderId() const
	{
		return 155;
	}

	const da::platform::CBgfxGraphicsMaterial* CBgfxPipelineGBuffer::getMaterial() const
	{
		return m_gbufferProgram;
	}

	bgfx::TextureHandle CBgfxPipelineGBuffer::getNormalTexture() const
	{
		return bgfx::getTexture(m_gbuffer, GBUFFER_RT_NORMAL);
	}

	bgfx::TextureHandle CBgfxPipelineGBuffer::getColorTexture() const
	{
		return bgfx::getTexture(m_gbuffer, GBUFFER_RT_COLOR);
	}

	bgfx::TextureHandle CBgfxPipelineGBuffer::getDepthTexture() const
	{
		return bgfx::getTexture(m_gbuffer, GBUFFER_RT_DEPTH);
	}

	bgfx::FrameBufferHandle CBgfxPipelineGBuffer::getFrameBufferHandle() const
	{
		return m_gbuffer;
	}

	void da::platform::CBgfxPipelineGBuffer::initialize()
	{
		bgfx::setViewName(renderId(),"CBgfxPipelineGBuffer");

		bgfx::setViewClear(renderId()
			, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
			, 0
			, 1.0f
			, 0
		);

		const uint64_t tsFlags = 0
			| BGFX_TEXTURE_RT
			| BGFX_SAMPLER_MIN_POINT
			| BGFX_SAMPLER_MAG_POINT
			| BGFX_SAMPLER_MIP_POINT
			| BGFX_SAMPLER_U_CLAMP
			| BGFX_SAMPLER_V_CLAMP
			;

		m_gbufferTex[GBUFFER_RT_NORMAL] = bgfx::createTexture2D(bgfx::BackbufferRatio::Equal, false, 1, bgfx::TextureFormat::BGRA8, tsFlags);
		m_gbufferTex[GBUFFER_RT_COLOR] = bgfx::createTexture2D(bgfx::BackbufferRatio::Equal, false, 1, bgfx::TextureFormat::BGRA8, tsFlags);
		m_gbufferTex[GBUFFER_RT_DEPTH] = bgfx::createTexture2D(bgfx::BackbufferRatio::Equal, false, 1, bgfx::TextureFormat::D32F, tsFlags);
		m_gbuffer = bgfx::createFrameBuffer(BX_COUNTOF(m_gbufferTex), m_gbufferTex, true);

		m_gbufferProgram = new CBgfxGraphicsMaterial("shaders/rsm/vs_rsm_gbuffer.sc", "shaders/rsm/fs_rsm_gbuffer.sc");  // Gbuffer
		m_gbufferProgram->initialize();
	}

	void da::platform::CBgfxPipelineGBuffer::shutdown()
	{
		m_gbufferProgram->shutdown();
		delete m_gbufferProgram;

		for (size_t i = 0; i < 3; i++) {
			bgfx::destroy(m_gbufferTex[i]);
		}

		bgfx::destroy(m_gbuffer);
	}
}

#endif