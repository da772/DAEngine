#include "dapch.h"

#include "bgfx_volumetric_light_shader.h"
#include "DAEngine/platform/graphics/bgfx/bgfx_graphics_material.h"
#include <platform/graphics/bgfx/bgfx_util.h>
#include "bgfx_type_renderer.h"

#ifdef DA_DEBUG
#include "DAEngine/debug/debug_menu_bar.h"
#include <imgui.h>
#endif

namespace da::platform
{
	void CBgfxVolumetricLightShader::initialize()
	{
		m_pVolLightShader = da::graphics::CMaterialFactory::create("shaders/cluster/vs_ssao.sc", "shaders/cluster/fs_vol_lighting.sc");

		m_frameBuffer = createFrameBuffer(true);

		m_blurParams = ::bgfx::createUniform("s_params", ::bgfx::UniformType::Vec4);
		m_lightPos = ::bgfx::createUniform("s_ssLightPos", ::bgfx::UniformType::Vec4);
		m_lightOcclusionTexture = ::bgfx::createUniform("s_texture", ::bgfx::UniformType::Sampler);

		constexpr float BOTTOM = -1.0f, TOP = 3.0f, LEFT = -1.0f, RIGHT = 3.0f;
		const PosVertex vertices[3] = { { LEFT, BOTTOM, 0.0f }, { RIGHT, BOTTOM, 0.0f }, { LEFT, TOP, 0.0f } };
		m_blitTriangleBuffer = ::bgfx::createVertexBuffer(::bgfx::copy(&vertices, sizeof(vertices)), PosVertex::layout);

#ifdef DA_DEBUG
		da::debug::CDebugMenuBar::register_debug(HASHSTR("Renderer"), HASHSTR("Volumetric Lighting"), &m_debug, [this] {renderDebug(); });
#endif

	}

	void CBgfxVolumetricLightShader::render(::bgfx::ViewId view, size_t width, size_t height,::bgfx::TextureHandle lightOcclusionTexture, glm::vec2 lightPos)
	{
		m_params1.x = lightPos.x;
		m_params1.y = 1.0-lightPos.y;
		

		::bgfx::setViewName(view, "Volumetric Lighting");
		::bgfx::setViewClear(view, BGFX_CLEAR_NONE);
		::bgfx::setViewRect(view, 0, 0, width, height);
		::bgfx::setViewFrameBuffer(view, m_frameBuffer);
		::bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_CULL_CW);

		::bgfx::setUniform(m_lightPos, glm::value_ptr(m_params1));
		::bgfx::setUniform(m_blurParams, glm::value_ptr(m_params2));
		::bgfx::setTexture(0, m_lightOcclusionTexture, lightOcclusionTexture);

		::bgfx::setVertexBuffer(0, m_blitTriangleBuffer);
		
		::bgfx::submit(view, { m_pVolLightShader->getHandle() });
	}

	void CBgfxVolumetricLightShader::shutdown()
	{
		da::graphics::CMaterialFactory::remove(m_pVolLightShader);

		BGFXTRYDESTROY(m_frameBuffer);
		BGFXTRYDESTROY(m_blurParams);
		BGFXTRYDESTROY(m_lightPos);
		BGFXTRYDESTROY(m_lightOcclusionTexture);
		BGFXTRYDESTROY(m_blitTriangleBuffer);



#ifdef DA_DEBUG
		da::debug::CDebugMenuBar::unregister_debug(HASHSTR("Renderer"), HASHSTR("Volumetric Lighting"));
#endif
	}

	void CBgfxVolumetricLightShader::onReset(size_t width, size_t height)
	{
		if (!::bgfx::isValid(m_frameBuffer)) {
			m_frameBuffer = createFrameBuffer(true);
		}
	}

	::bgfx::FrameBufferHandle CBgfxVolumetricLightShader::createFrameBuffer(bool hdr)
	{
		bgfx::TextureHandle textures[1];
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

	::bgfx::FrameBufferHandle CBgfxVolumetricLightShader::getBuffer() const
	{
		return m_frameBuffer;
	}

#ifdef DA_DEBUG
	void CBgfxVolumetricLightShader::renderDebug()
	{
		if (ImGui::Begin("Volumetric Lighting", &m_debug))
		{
			ImGui::Text("Enable Blur");
			ImGui::SameLine();
			bool blur = m_params1.z > 0.0;
			if (ImGui::Checkbox("##enabled", &blur)) {
				m_params1.z = blur ? 1.0 : 0.0;
			}
			ImGui::Text("Light Screenspace Pos: %f, %f", m_params1.x, m_params1.y);
			ImGui::Separator();
			ImGui::Text("Coefficents");
			ImGui::Separator();

			ImGui::Text("Num samples: ");
			ImGui::SameLine();
			int samples = (int)m_params1.w;
			if (ImGui::InputInt("##samples", &samples)) {
				if (samples > 1000) samples = 1000;
				if (samples < 1) samples = 1;

				m_params1.w = (float)samples;
			}

			ImGui::Text("Density: ");
			ImGui::SameLine();
			ImGui::InputFloat("##density", &m_params2.x);

			ImGui::Text("Exposure: ");
			ImGui::SameLine();
			ImGui::InputFloat("##exposure", &m_params2.y);

			ImGui::Text("Decay: ");
			ImGui::SameLine();
			ImGui::InputFloat("##decay", &m_params2.z);

			ImGui::Text("Weight: ");
			ImGui::SameLine();
			ImGui::InputFloat("##Weight", &m_params2.w);

			if (ImGui::CollapsingHeader("Framebuffer")) {

				::bgfx::TextureHandle t = ::bgfx::getTexture(m_frameBuffer, 0);
				ImGui::Image((ImTextureID)t.idx, { 480, 240 });
			}


		}

		ImGui::End();
	}
#endif

}