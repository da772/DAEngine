#include <dapch.h>

#include "DAEngine/platform/graphics/bgfx/pipeline/bgfx_pipeline_light.h"

#ifdef DA_GRAPHICS_BGFX

#include "DAEngine/platform/graphics/bgfx/bgfx_graphics_material.h"
#include <bx/math.h>

namespace da::platform
{

	CBgfxPipelineLight::CBgfxPipelineLight()
	{

	}

	void CBgfxPipelineLight::initialize()
	{
		bgfx::setViewName(renderId(), "CBgfxPipelineLight");

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


		m_lightBufferTex = bgfx::createTexture2D(bgfx::BackbufferRatio::Equal, false, 1, bgfx::TextureFormat::BGRA8, tsFlags);
		bgfx::TextureHandle lightBufferRTs[] = {
			m_lightBufferTex
		};
		m_lightBuffer = bgfx::createFrameBuffer(BX_COUNTOF(lightBufferRTs), lightBufferRTs, true);

		m_lightProgram = new CBgfxGraphicsMaterial("shaders/rsm/vs_rsm_lbuffer.sc", "shaders/rsm/fs_rsm_lbuffer.sc");  // Light buffer
		m_lightProgram->initialize();
	}

	void CBgfxPipelineLight::shutdown()
	{
		if (m_lightProgram)
		{
			m_lightProgram->shutdown();
			delete m_lightProgram;
		}

		bgfx::destroy(m_lightBufferTex);
		bgfx::destroy(m_lightBuffer);
	}

	constexpr const uint32_t CBgfxPipelineLight::renderId() const
	{
		return 153;
	}

	const da::platform::CBgfxGraphicsMaterial* CBgfxPipelineLight::getMaterial() const
	{
		return m_lightProgram;
	}

	bgfx::TextureHandle CBgfxPipelineLight::getTexture() const
	{
		return m_lightBufferTex;
	}

	bgfx::FrameBufferHandle CBgfxPipelineLight::getFrameBufferHandle() const
	{
		return m_lightBuffer;
	}

}

#endif