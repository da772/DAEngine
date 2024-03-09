#include "dapch.h"

#include "bgfx_ssao_shader.h"
#include "bgfx_type_renderer.h"
#include <platform/graphics/bgfx/bgfx_util.h>
#include "bgfx_samplers.h"
#include <glm/gtc/type_ptr.hpp>

#ifdef DA_DEBUG
#include "DAEngine/debug/debug_menu_bar.h"
#include <imgui.h>
#endif

namespace da::platform
{
	void CBgfxSSAOShader::initialize()
	{
		m_pSsaoProgram = da::graphics::CMaterialFactory::create("shaders/cluster/vs_ssao.sc", "shaders/cluster/fs_ssao.sc");
		m_pSsaoBlurProgram = da::graphics::CMaterialFactory::create("shaders/cluster/vs_ssao.sc", "shaders/cluster/fs_blur.sc");

		// triangle used for blitting
		constexpr float BOTTOM = -1.0f, TOP = 3.0f, LEFT = -1.0f, RIGHT = 3.0f;
		const PosVertex vertices[3] = { { LEFT, BOTTOM, 0.0f }, { RIGHT, BOTTOM, 0.0f }, { LEFT, TOP, 0.0f } };
		m_blitTriangleBuffer = ::bgfx::createVertexBuffer(::bgfx::copy(&vertices, sizeof(vertices)), PosVertex::layout);

		m_ssaoBuffer = createFrameBuffer(true);

		m_uniformHandle = bgfx::createUniform("s_texture", bgfx::UniformType::Sampler);
		m_uniformParamHandle = bgfx::createUniform("u_param1", bgfx::UniformType::Vec4);
		m_ssaoUniformHandle = bgfx::createUniform("s_ssao", bgfx::UniformType::Sampler);

#ifdef DA_DEBUG
		da::debug::CDebugMenuBar::register_debug(HASHSTR("Renderer"), HASHSTR("SSAO"), &m_debug, [this] { renderDebug(); });
#endif
	}


	void CBgfxSSAOShader::renderSSAO(size_t width, size_t height, ::bgfx::ViewId view, ::bgfx::FrameBufferHandle depthBuffer )
	{
		::bgfx::setViewName(view, "SSAO");
		::bgfx::setViewClear(view, BGFX_CLEAR_NONE);
		::bgfx::setViewRect(view, 0, 0, width, height);
		::bgfx::setViewFrameBuffer(view, m_ssaoBuffer);
		::bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_CULL_CW);
		::bgfx::TextureHandle frameBufferTexture = ::bgfx::getTexture(depthBuffer, 0);
		::bgfx::setTexture(da::platform::CBgfxSamplers::DEFERRED_DEPTH, m_uniformHandle, frameBufferTexture);
		::bgfx::setVertexBuffer(0, m_blitTriangleBuffer);
		::bgfx::setUniform(m_uniformParamHandle, glm::value_ptr(m_param1));
		::bgfx::submit(view, { m_pSsaoProgram->getHandle() });
	}

	void CBgfxSSAOShader::renderBlur(size_t width, size_t height, ::bgfx::ViewId view)
	{
		::bgfx::setViewName(view, "SSAO Blur");
		::bgfx::setViewClear(view, BGFX_CLEAR_NONE);
		::bgfx::setViewRect(view, 0, 0, width, height);
		::bgfx::setViewFrameBuffer(view, m_ssaoProcessedBuffer);
		::bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_CULL_CW);
		::bgfx::TextureHandle frameBufferTexture = ::bgfx::getTexture(m_ssaoBuffer, 0);
		::bgfx::setTexture(da::platform::CBgfxSamplers::DEFERRED_DEPTH, m_uniformHandle, frameBufferTexture);
		::bgfx::setVertexBuffer(0, m_blitTriangleBuffer);
		::bgfx::setUniform(m_uniformParamHandle, glm::value_ptr(m_param1));
		::bgfx::submit(view, { m_pSsaoBlurProgram->getHandle() });
	}

	void CBgfxSSAOShader::shutdown()
	{
		da::graphics::CMaterialFactory::remove(m_pSsaoProgram);
		da::graphics::CMaterialFactory::remove(m_pSsaoBlurProgram);

		BGFXTRYDESTROY(m_ssaoBuffer);
		BGFXTRYDESTROY(m_ssaoProcessedBuffer);
		BGFXTRYDESTROY(m_blitTriangleBuffer);
		BGFXTRYDESTROY(m_uniformHandle);
		BGFXTRYDESTROY(m_uniformParamHandle);
		BGFXTRYDESTROY(m_ssaoUniformHandle);
#ifdef DA_DEBUG
		da::debug::CDebugMenuBar::unregister_debug(HASHSTR("Renderer"), HASHSTR("SSAO"));
#endif
	}

	void CBgfxSSAOShader::reset(size_t width, size_t height)
	{
		if (!::bgfx::isValid(m_ssaoBuffer))
		{
			m_ssaoBuffer = createFrameBuffer(true);
		}

		if (!::bgfx::isValid(m_ssaoProcessedBuffer))
		{
			m_ssaoProcessedBuffer = createFrameBuffer(true);
		}
	}

	::bgfx::FrameBufferHandle  CBgfxSSAOShader::createFrameBuffer(bool hdr)
	{
		bgfx::TextureHandle textures[2];
		uint8_t attachments = 0;

		const uint64_t samplerFlags = BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIP_POINT |
			BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_TEXTURE_RT;

		bgfx::TextureFormat::Enum format =
			hdr ? bgfx::TextureFormat::RGBA16F : bgfx::TextureFormat::BGRA8; // BGRA is often faster (internal GPU format)
		assert(bgfx::isTextureValid(0, false, 1, format, samplerFlags));
		textures[attachments++] =
			bgfx::createTexture2D(bgfx::BackbufferRatio::Equal, false, 1, format, samplerFlags);


		bgfx::FrameBufferHandle fb = bgfx::createFrameBuffer(attachments, textures, true);

		return fb;
	}

	void CBgfxSSAOShader::bindSSAO()
	{
		::bgfx::TextureHandle frameBufferTexture = ::bgfx::getTexture(m_ssaoProcessedBuffer, 0);
		::bgfx::setTexture(CBgfxSamplers::SAMPLER_SSAO, m_ssaoUniformHandle, frameBufferTexture);
	}


#ifdef DA_DEBUG

	void CBgfxSSAOShader::renderDebug()
	{
		if (ImGui::Begin("SSAO Debug", &m_debug)) {
			ImGui::InputFloat("Param1 - Total Str:", &m_param1[0]);
			ImGui::InputFloat("Param1 - Base:", &m_param1[1]);
			ImGui::InputFloat("Param1 - Area:", &m_param1[2]);
			ImGui::InputFloat("Param1 - Radius:", &m_param1[3]);

			if (ImGui::CollapsingHeader("Framebuffer")) {
				::bgfx::TextureHandle handle = ::bgfx::getTexture(m_ssaoProcessedBuffer);

				if (handle.idx == UINT16_MAX) {
					return;
				}
				ImGui::Image((ImTextureID)handle.idx, { 480, 240 });
			}
		}

		ImGui::End();
	}
#endif
}