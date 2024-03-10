#include "dapch.h"
#include "bgfx_bloom_shader.h"
#include "DAEngine/platform/graphics/bgfx/bgfx_graphics_material.h"
#include <platform/graphics/bgfx/bgfx_util.h>
#include "bgfx_type_renderer.h"
#include "bgfx_samplers.h">

#ifdef DA_REVIEW
#include "daengine/debug/debug_menu_bar.h"
#include <imgui.h>
#endif

namespace da::platform
{

	void CBgfxBloomShader::initialize(size_t width, size_t height)
	{
		m_width = width;
		m_height = height;

		m_pBloomShader = da::graphics::CMaterialFactory::create("shaders/cluster/vs_ssao.sc", "shaders/cluster/fs_bloom.sc");
		m_pDownscaleShader= da::graphics::CMaterialFactory::create("shaders/cluster/vs_ssao.sc", "shaders/cluster/fs_bloom_downsample.sc");
		m_pUpscaleShader = da::graphics::CMaterialFactory::create("shaders/cluster/vs_ssao.sc", "shaders/cluster/fs_bloom_upsample.sc");

		// triangle used for blitting
		constexpr float BOTTOM = -1.0f, TOP = 3.0f, LEFT = -1.0f, RIGHT = 3.0f;
		const PosVertex vertices[3] = { { LEFT, BOTTOM, 0.0f }, { RIGHT, BOTTOM, 0.0f }, { LEFT, TOP, 0.0f } };
		m_blitTriangleBuffer = ::bgfx::createVertexBuffer(::bgfx::copy(&vertices, sizeof(vertices)), PosVertex::layout);
		m_textureUniform = bgfx::createUniform("s_texture", bgfx::UniformType::Sampler);
		m_bloomParamsUniform = bgfx::createUniform("params", bgfx::UniformType::Vec4);
		m_pixelSizeUniform = bgfx::createUniform("u_pixelSize", bgfx::UniformType::Vec4);
		m_upscaleIntensityUniform = bgfx::createUniform("u_intensity", bgfx::UniformType::Vec4);

		for (size_t i = 0; i < m_bloomAmount; i++) {
			m_processedFrameBuffers.push_back(createFrameBuffer(true, width >> i, height >> i));
		}
		m_frameBuffer = createFrameBuffer(true);

#ifdef DA_REVIEW
		da::debug::CDebugMenuBar::register_debug(HASHSTR("Renderer"), HASHSTR("Bloom"), &m_debug, [this] { onDebugRender(); });
#endif
	}

	void CBgfxBloomShader::shutdown()
	{

		da::graphics::CMaterialFactory::remove(m_pBloomShader);
		da::graphics::CMaterialFactory::remove(m_pDownscaleShader);
		da::graphics::CMaterialFactory::remove(m_pUpscaleShader);

		BGFXTRYDESTROY(m_frameBuffer);
		BGFXTRYDESTROY(m_blitTriangleBuffer);
		BGFXTRYDESTROY(m_textureUniform);
		BGFXTRYDESTROY(m_bloomParamsUniform);
		BGFXTRYDESTROY(m_pixelSizeUniform);
		BGFXTRYDESTROY(m_upscaleIntensityUniform);
		for (size_t i = 0; i < m_processedFrameBuffers.size(); i++) {
			BGFXTRYDESTROY(m_processedFrameBuffers[i]);
		}
		

#ifdef DA_REVIEW
		da::debug::CDebugMenuBar::unregister_debug(HASHSTR("Renderer"), HASHSTR("Bloom"));
#endif

	}

	void CBgfxBloomShader::render(::bgfx::ViewId view, ::bgfx::TextureHandle texture, size_t width, size_t height)
	{
		::bgfx::setViewName(view, "Bloom");
		::bgfx::setViewClear(view, BGFX_CLEAR_NONE);
		::bgfx::setViewRect(view, 0, 0, width, height);
		::bgfx::setViewFrameBuffer(view, m_frameBuffer);
		::bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_CULL_CW);
		::bgfx::setTexture(da::platform::CBgfxSamplers::DEFERRED_DEPTH, m_textureUniform, texture);
		::bgfx::setUniform(m_bloomParamsUniform, glm::value_ptr(m_bloomParams));
		::bgfx::setVertexBuffer(0, m_blitTriangleBuffer);
		::bgfx::submit(view, { m_pBloomShader->getHandle() });
	}


	void CBgfxBloomShader::renderBlur(::bgfx::ViewId view, size_t width, size_t height)
	{
		// downsample
		for (size_t i = 0; i < m_bloomAmount; i++) {
			::bgfx::setViewName(view+i, "Bloom Post Process");
			::bgfx::setViewClear(view + i, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);
			::bgfx::setViewRect(view + i, 0, 0, width >> i, height >> i);

			const uint16_t shift = i;
			const float pixelSize[4] =
			{
				1.0f / (float)(width >> shift),
				1.0f / (float)(height >> shift),
				0.0f,
				0.0f,
			};

			::bgfx::setUniform(m_pixelSizeUniform, pixelSize);
			::bgfx::setViewFrameBuffer(view + i, m_processedFrameBuffers[i]);
			::bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_CULL_CW);
			::bgfx::TextureHandle texture;

			if (i == 0) {
				texture = ::bgfx::getTexture(m_frameBuffer, 0);
			}
			else {
				texture = ::bgfx::getTexture(m_processedFrameBuffers[i-1], 0);
			}
			
			
			::bgfx::setTexture(da::platform::CBgfxSamplers::DEFERRED_DEPTH, m_textureUniform, texture);
			::bgfx::setVertexBuffer(0, m_blitTriangleBuffer);
			::bgfx::submit(view + i, { m_pDownscaleShader->getHandle() });
		}

		// upScale
		for (size_t i = m_bloomAmount -1; i > 0; i--) {
			::bgfx::ViewId mView = view + (m_bloomAmount*2) - i-1;

			::bgfx::setViewName(mView, "Bloom Post Process");
			::bgfx::setViewClear(mView, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH);
			

			const uint16_t shift = i - 1;
			const float pixelSize[4] =
			{
				1.0f / (float)(width >> shift),
				1.0f / (float)(height >> shift),
				0.0f,
				0.0f,
			};

			::bgfx::setViewRect(mView, 0, 0, width >> (i-1), height >> (i-1));

			::bgfx::setUniform(m_pixelSizeUniform, pixelSize);
			::bgfx::setUniform(m_upscaleIntensityUniform, glm::value_ptr(m_intensity));
			::bgfx::setViewFrameBuffer(mView, m_processedFrameBuffers[i-1]);
			::bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A | BGFX_STATE_CULL_CW | BGFX_STATE_BLEND_ADD);
			::bgfx::TextureHandle texture;

			if (i == 0) {
				texture = ::bgfx::getTexture(m_frameBuffer, 0);
			}
			else {
				texture = ::bgfx::getTexture(m_processedFrameBuffers[i], 0);
			}

			::bgfx::setTexture(da::platform::CBgfxSamplers::DEFERRED_DEPTH, m_textureUniform, texture);
			::bgfx::setVertexBuffer(0, m_blitTriangleBuffer);
			::bgfx::submit(mView, { m_pUpscaleShader->getHandle() });
		}
	}



	::bgfx::FrameBufferHandle CBgfxBloomShader::getBuffer() const
	{
		return m_processedFrameBuffers[0];
	}

	bgfx::FrameBufferHandle  CBgfxBloomShader::createFrameBuffer(bool hdr, size_t width, size_t height)
	{
		bgfx::TextureHandle textures[1];
		uint8_t attachments = 0;

		const uint64_t samplerFlags = BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_TEXTURE_RT;

		bgfx::TextureFormat::Enum format =
			!hdr ? bgfx::TextureFormat::RGBA16F : bgfx::TextureFormat::BGRA8; // BGRA is often faster (internal GPU format)

		if (width == 0 || height == 0)
		{
			textures[attachments++] =
				bgfx::createTexture2D(bgfx::BackbufferRatio::Equal, false, 1, format, samplerFlags);
		}
		else
		{
			return ::bgfx::createFrameBuffer(width, height, format, samplerFlags);
		}
		

		bgfx::FrameBufferHandle handle = bgfx::createFrameBuffer(attachments, textures, true);

		return handle;
	}

	void CBgfxBloomShader::onReset(size_t width, size_t height)
	{

		m_width = width;
		m_height = height;

		if (!::bgfx::isValid(m_frameBuffer))
			m_frameBuffer = createFrameBuffer(true);

		for (size_t i = 0; i < m_processedFrameBuffers.size(); i++) {

			BGFXTRYDESTROY(m_processedFrameBuffers[i]);	
		}

		m_processedFrameBuffers = {};
		for (size_t i = 0; i < m_bloomAmount; i++) {
			m_processedFrameBuffers.push_back(createFrameBuffer(true, width >> i, height >> i));
		}

	
	}

#ifdef DA_REVIEW
	void CBgfxBloomShader::onDebugRender()
	{
		if (ImGui::Begin("Bloom", &m_debug)) {
			ImGui::Text("Bloom Threshold");
			ImGui::SameLine();
			ImGui::SetNextItemWidth(125);
			ImGui::InputFloat("##threhold1", &m_bloomParams.x, .01f);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(125);
			ImGui::InputFloat("##threhold2", &m_bloomParams.y, .01f);
			ImGui::SameLine();
			ImGui::SetNextItemWidth(125);
			ImGui::InputFloat("##threhold3", &m_bloomParams.z, .01f);

			ImGui::Text("Bloom Intensity");
			ImGui::SameLine();
			ImGui::InputFloat4("##intensity", glm::value_ptr(m_intensity));

			ImGui::Text("Bloom Amount");
			ImGui::SameLine();
			int amt = m_bloomAmount;
			if (ImGui::InputInt("##passes", (int*)&amt, 1)) {
				if (amt <= 255 && amt > 0 && m_width >> amt > 0 && m_height >> amt > 0) {
					m_bloomAmount = amt;
					for (size_t i = 0; i < m_processedFrameBuffers.size(); i++) {
						BGFXTRYDESTROY(m_processedFrameBuffers[i]);
					}

					onReset(m_width, m_height);
				}
				
			}

			if (ImGui::CollapsingHeader("Bloom image")) {
				ImGui::Image((ImTextureID)::bgfx::getTexture(getBuffer(), 0).idx, { 480,240 });
			}
		}

		ImGui::End();
	}
#endif

}