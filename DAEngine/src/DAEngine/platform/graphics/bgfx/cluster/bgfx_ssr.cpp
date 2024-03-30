#include "dapch.h"

#include "bgfx_ssr.h"

#include "core/graphics/graphics_material.h"
#include "platform/graphics/bgfx/bgfx_util.h"
#include "bgfx_samplers.h"

namespace da::platform {

	void CBgfxSsr::initialize(uint16_t width, uint16_t height)
	{
		
		m_Color = bgfx::createUniform("u_texture1", bgfx::UniformType::Sampler);
		m_depth = bgfx::createUniform("u_texture2", bgfx::UniformType::Sampler);
		m_positions = bgfx::createUniform("u_texture3", bgfx::UniformType::Sampler);
		m_material = da::graphics::CMaterialFactory::create("shaders/ssr/vs_ssr.sc", "shaders/ssr/fs_ssr.sc");
		m_width = width;
		m_height = height;
		reset(width, height);

		constexpr float BOTTOM = -1.0f, TOP = 3.0f, LEFT = -1.0f, RIGHT = 3.0f;
		const CBgfxUtil::PosVertex vertices[3] = { { LEFT, BOTTOM, 0.0f }, { RIGHT, BOTTOM, 0.0f }, { LEFT, TOP, 0.0f } };
		m_vertexBuffer  = ::bgfx::createVertexBuffer(::bgfx::copy(&vertices, sizeof(vertices)), CBgfxUtil::getScreenVertices().layout);
	}

	void CBgfxSsr::renderSSR(bgfx::ViewId view, ::bgfx::FrameBufferHandle color, ::bgfx::FrameBufferHandle depth, ::bgfx::FrameBufferHandle positions)
	{
		::bgfx::setViewName(view, "SSR");
		::bgfx::setViewRect(view, 0, 0, m_width, m_height);
		::bgfx::setViewFrameBuffer(view, m_frameBuffer);
		::bgfx::setViewClear(view, BGFX_CLEAR_COLOR, 0x00000000, 1.0f, 0);
		::bgfx::touch(view);

		::bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_CULL_CW);
		::bgfx::TextureHandle colorTexture = ::bgfx::getTexture(color, 0);
		::bgfx::setTexture(da::platform::CBgfxSamplers::SAMPLER_COLOR, m_Color, colorTexture);
		::bgfx::TextureHandle depthTexture = ::bgfx::getTexture(depth, 0);
		::bgfx::setTexture(da::platform::CBgfxSamplers::SAMPLER_DEPTH, m_depth, depthTexture);
		::bgfx::TextureHandle posTexture = ::bgfx::getTexture(positions, 2);
		::bgfx::setTexture(da::platform::CBgfxSamplers::SAMPLER_POSITIONS, m_positions, posTexture);
		::bgfx::setVertexBuffer(0, m_vertexBuffer);
		::bgfx::submit(view, { m_material->getHandle() });
	}

	void CBgfxSsr::shutdown()
	{
		da::graphics::CMaterialFactory::remove(m_material);
		BGFXTRYDESTROY(m_Color);
		BGFXTRYDESTROY(m_depth);
		BGFXTRYDESTROY(m_frameBuffer);
		BGFXTRYDESTROY(m_vertexBuffer);
	}

	void CBgfxSsr::reset(uint16_t width, uint16_t height)
	{
		m_width = width;
		m_height = height;
		BGFXTRYDESTROY(m_frameBuffer);
		m_frameBuffer = CBgfxUtil::createFrameBuffer(true);
	}



}