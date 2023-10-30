#include <dapch.h>
#include "bgfx_pipeline_shadow.h"

#ifdef DA_GRAPHICS_BGFX

#include "DAEngine/core/containers.h"
#include "DAEngine/platform/graphics/bgfx/bgfx_graphics_material.h"
#include <bx/math.h>

#define SHADOW_RT_RSM 0
#define SHADOW_RT_DEPTH  1
#define SHADOW_MAP_DIM 512

namespace da::platform {


	CBgfxPipelineShadow::CBgfxPipelineShadow()
	{

	}

	const uint32_t CBgfxPipelineShadow::renderId() const
	{
		return 154;
	}

	const da::platform::CBgfxGraphicsMaterial* CBgfxPipelineShadow::getMaterial() const
	{
		return m_shadowProgram;
	}

	bgfx::TextureHandle CBgfxPipelineShadow::getDepthTexture() const
	{
		return bgfx::getTexture(m_shadowBuffer, SHADOW_RT_DEPTH);
	}

	bgfx::FrameBufferHandle CBgfxPipelineShadow::getFrameBufferHandle() const
	{
		return m_shadowBuffer;
	}

	bgfx::TextureHandle CBgfxPipelineShadow::getRSMTexture() const
	{
		return bgfx::getTexture(m_shadowBuffer, SHADOW_RT_RSM);
	}

	void da::platform::CBgfxPipelineShadow::initialize()
	{
		bgfx::setViewName(renderId(),"CBgfxPipelineShadow");

		bgfx::setViewClear(renderId()
			, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
			, 0
			, 1.0f
			, 0
		);

		const uint64_t rsmFlags = 0
			| BGFX_TEXTURE_RT
			| BGFX_SAMPLER_MIN_POINT
			| BGFX_SAMPLER_MAG_POINT
			| BGFX_SAMPLER_MIP_POINT
			| BGFX_SAMPLER_U_CLAMP
			| BGFX_SAMPLER_V_CLAMP
			;


		m_shadowBufferTex[SHADOW_RT_RSM] = bgfx::createTexture2D(
			SHADOW_MAP_DIM
			, SHADOW_MAP_DIM
			, false
			, 1
			, bgfx::TextureFormat::BGRA8
			, rsmFlags
		);

		// Typical shadow map
		m_shadowBufferTex[SHADOW_RT_DEPTH] = bgfx::createTexture2D(
			SHADOW_MAP_DIM
			, SHADOW_MAP_DIM
			, false
			, 1
			, bgfx::TextureFormat::D16
			, BGFX_TEXTURE_RT /* | BGFX_SAMPLER_COMPARE_LEQUAL*/
		);  // Note I'm not setting BGFX_SAMPLER_COMPARE_LEQUAL.  Why?
			// Normally a PCF shadow map such as this requires a compare.  However, this sample also
			// reads from this texture in the lighting pass, and only uses the PCF capabilities in
			// the combine pass, so the flag is disabled by default.

		m_shadowBuffer = bgfx::createFrameBuffer(BX_COUNTOF(m_shadowBufferTex), m_shadowBufferTex, true);

		m_shadowProgram = new CBgfxGraphicsMaterial("shaders/rsm/vs_rsm_shadow.sc", "shaders/rsm/fs_rsm_shadow.sc"); // Drawing shadow map
		m_shadowProgram->initialize();
	}

	void da::platform::CBgfxPipelineShadow::shutdown()
	{
		m_shadowProgram->shutdown();
		delete m_shadowProgram;

		for (size_t i = 0; i < 2; i++) {
			bgfx::destroy(m_shadowBufferTex[i]);
		}

		bgfx::destroy(m_shadowBuffer);
	}
}

#endif