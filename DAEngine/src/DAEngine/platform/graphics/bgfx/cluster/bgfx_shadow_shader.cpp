#include "dapch.h"
#include "bgfx_shadow_shader.h"

#include "daengine/platform/graphics/bgfx/bgfx_graphics_material.h"
#include "../bgfx_util.h"

namespace da::platform {

	void CBgfxShadowShader::initialize()
	{
		if (!useShadowSampler()) {
			m_material = new CBgfxGraphicsMaterial("shaders/cluster/vs_shadow.sc", "shaders/cluster/fs_shadow.sc");
			m_material->initialize();

		
			m_depthScaleOffset = bgfx::createUniform("u_depthScaleOffset", bgfx::UniformType::Vec4);

			const bgfx::Caps* caps = bgfx::getCaps();

			float depthScaleOffset[4] = { 1.0f, 0.0f, 0.0f, 0.0f };
			if (caps->homogeneousDepth)
			{
				depthScaleOffset[0] = 0.5f;
				depthScaleOffset[1] = 0.5f;
			}
			bgfx::setUniform(m_depthScaleOffset, depthScaleOffset);
			return;
		}

		m_material = new CBgfxGraphicsMaterial("shaders/cluster/vs_shadow_sampler.sc", "shaders/cluster/fs_shadow_sampler.sc");
		m_material->initialize();
	}

	void CBgfxShadowShader::shutdown()
	{
		m_material->shutdown();
		delete m_material;
		m_material = nullptr;

		

		if (bgfx::isValid(m_frameBuffer)) {
			BGFXDESTROY(m_frameBuffer);
		}
		if (bgfx::isValid(m_depthScaleOffset)) {
			BGFXDESTROY(m_depthScaleOffset);
		}
	}

	da::platform::CBgfxGraphicsMaterial* CBgfxShadowShader::getMaterial() const
	{
		return m_material;
	}

	da::core::CCamera& CBgfxShadowShader::getCamera()
	{
		return m_camera;
	}

	const uint32_t CBgfxShadowShader::getShadowMapSize() const
	{
		return m_shadowMapSize;
	}

	void CBgfxShadowShader::createFrameBuffers()
	{
		if (bgfx::isValid(m_shadowMapTexture)) { 
			bgfx::destroy(m_shadowMapTexture);
		}

		if (bgfx::isValid(m_frameBuffer)) {
			bgfx::destroy(m_frameBuffer);
		}

		uint8_t size = 0;
		if (m_useShadowSampler) {
			bgfx::TextureHandle fbtextures[] =
			{
				bgfx::createTexture2D(
					  m_shadowMapSize
					, m_shadowMapSize
					, false
					, 1
					, bgfx::TextureFormat::D16
					, BGFX_TEXTURE_RT | BGFX_SAMPLER_COMPARE_LEQUAL
					),
			};

			m_shadowMapTexture = fbtextures[0];
			size = 1;
			m_frameBuffer = bgfx::createFrameBuffer(size, fbtextures, true);
		}
		else {

			bgfx::TextureHandle fbtextures[]
			{
				bgfx::createTexture2D(
					  m_shadowMapSize
					, m_shadowMapSize
					, false
					, 1
					, bgfx::TextureFormat::BGRA8
					, BGFX_TEXTURE_RT
					),
				bgfx::createTexture2D(
					  m_shadowMapSize
					, m_shadowMapSize
					, false
					, 1
					, bgfx::TextureFormat::D16
					, BGFX_TEXTURE_RT_WRITE_ONLY
					),
			};

			m_shadowMapTexture = fbtextures[0];
			size = 2;
			m_frameBuffer = bgfx::createFrameBuffer(size, fbtextures, true);
		}
		const char name[] = "ShadowDepthBuffer";
		bgfx::setName(m_shadowMapTexture, name, sizeof(name));
		
	}

}