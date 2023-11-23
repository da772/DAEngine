#include "dapch.h"
#include "bgfx_renderer.h"

#ifdef DA_GRAPHICS_BGFX

namespace da::platform::bgfx {

	::bgfx::VertexLayout CBgfxRenderer::PosVertex::layout;


	CBgfxRenderer::CBgfxRenderer() {

	}

	void CBgfxRenderer::initalize()
	{
		PosVertex::init();

		blitSampler = ::bgfx::createUniform("s_texColor", ::bgfx::UniformType::Sampler);
		camPosUniform = ::bgfx::createUniform("u_camPos", ::bgfx::UniformType::Vec4);
		normalMatrixUniform = ::bgfx::createUniform("u_normalMatrix", ::bgfx::UniformType::Mat3);
		exposureVecUniform = ::bgfx::createUniform("u_exposureVec", ::bgfx::UniformType::Vec4);
		tonemappingModeVecUniform = ::bgfx::createUniform("u_tonemappingModeVec", ::bgfx::UniformType::Vec4);

		// triangle used for blitting
		constexpr float BOTTOM = -1.0f, TOP = 3.0f, LEFT = -1.0f, RIGHT = 3.0f;
		const PosVertex vertices[3] = { { LEFT, BOTTOM, 0.0f }, { RIGHT, BOTTOM, 0.0f }, { LEFT, TOP, 0.0f } };
		blitTriangleBuffer = ::bgfx::createVertexBuffer(::bgfx::copy(&vertices, sizeof(vertices)), PosVertex::layout);

		blitProgram = CBgfxGraphicsMaterial("shaders/Cluster/vs_tonemap.sc", "shaders/Cluster/fs_tonemap.sc");
		blitProgram.initialize();


		::bgfx::frame();
	}

	void CBgfxRenderer::update()
	{
		LOG_DEBUG(ELogChannel::Graphics, "Rendering");
	}

	void CBgfxRenderer::shutdown()
	{
		blitProgram.shutdown();
	}

	}

#endif