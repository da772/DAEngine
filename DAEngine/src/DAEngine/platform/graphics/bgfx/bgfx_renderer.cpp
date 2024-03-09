#include "dapch.h"
#include "bgfx_renderer.h"
#include <core/graphics/camera.h>
#include "DAEngine/core/graphics/graphics_material.h"

#ifdef DA_GRAPHICS_BGFX

#include <bx/bx.h>
#include <bx/macros.h>
#include <bx/string.h>
#include <bx/math.h>
#include <glm/common.hpp>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtc/color_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include "bgfx_util.h"

namespace da::platform::bgfx {

	::bgfx::VertexLayout CBgfxRenderer::PosVertex::layout;


	CBgfxRenderer::CBgfxRenderer() {

	}

	void CBgfxRenderer::initialize()
	{
		PosVertex::init();

		m_blitSampler = ::bgfx::createUniform("s_texColor", ::bgfx::UniformType::Sampler);
		m_skySampler = ::bgfx::createUniform("s_skyColor", ::bgfx::UniformType::Sampler);
		m_camPosUniform = ::bgfx::createUniform("u_camPos", ::bgfx::UniformType::Vec4);
		m_normalMatrixUniform = ::bgfx::createUniform("u_normalMatrix", ::bgfx::UniformType::Mat3);
		m_exposureVecUniform = ::bgfx::createUniform("u_exposureVec", ::bgfx::UniformType::Vec4);
		m_tonemappingModeVecUniform = ::bgfx::createUniform("u_tonemappingModeVec", ::bgfx::UniformType::Vec4);

		// triangle used for blitting
		constexpr float BOTTOM = -1.0f, TOP = 3.0f, LEFT = -1.0f, RIGHT = 3.0f;
		const PosVertex vertices[3] = { { LEFT, BOTTOM, 0.0f }, { RIGHT, BOTTOM, 0.0f }, { LEFT, TOP, 0.0f } };
		m_blitTriangleBuffer = ::bgfx::createVertexBuffer(::bgfx::copy(&vertices, sizeof(vertices)), PosVertex::layout);

		m_blitProgram = da::graphics::CMaterialFactory::create("shaders/Cluster/vs_tonemap.sc", "shaders/Cluster/fs_tonemap.sc");

		m_pbr.initialize();
		m_pbr.generateAlbedoLUT();

		onInitialize();

		::bgfx::frame();
	}

	void CBgfxRenderer::update(float dt)
	{

		glm::vec4 camPos = glm::vec4(da::core::CCamera::getCamera()->position(), 1.0f);
		::bgfx::setUniform(m_camPosUniform, glm::value_ptr(camPos));

		glm::vec3 linear = m_pbr.m_whiteFurnaceEnabled
			? glm::vec3(CBgfxPBRShader::WHITE_FURNACE_RADIANCE)
			: glm::convertSRGBToLinear(glm::vec3(1.f,0.f,0.f)); // tonemapping expects linear colors
		glm::u8vec3 result = glm::u8vec3(glm::round(glm::clamp(linear, 0.0f, 1.0f) * 255.0f));
		m_clearColor = (result[0] << 24) | (result[1] << 16) | (result[2] << 8) | 255;


		onUpdate(dt);
		

		::bgfx::setViewName(MAX_VIEW + 1, "imgui");
	}

	void CBgfxRenderer::lateUpdate(float dt)
	{
		blitToScreen(MAX_VIEW);
	}


	void CBgfxRenderer::shutdown()
	{
		onShutdown();

		da::graphics::CMaterialFactory::remove(m_blitProgram);
		m_pbr.shutdown();

		BGFXDESTROY(m_blitSampler);
		BGFXDESTROY(m_skySampler);
		BGFXDESTROY(m_camPosUniform);
		BGFXDESTROY(m_normalMatrixUniform);
		BGFXDESTROY(m_exposureVecUniform);
		BGFXDESTROY(m_tonemappingModeVecUniform);
		BGFXDESTROY(m_blitTriangleBuffer);
		BGFXDESTROY(m_frameBuffer);
		BGFXDESTROY(m_skyFrameBuffer);



		::bgfx::discard();
	}

	void CBgfxRenderer::setViewProjection(::bgfx::ViewId view)
	{
		da::core::CCamera& cam = *da::core::CCamera::getCamera();

		// view matrix
		m_viewMat = cam.matrix();
		// projection matrix
		bx::mtxProj(glm::value_ptr(m_projMat),
			cam.fov,
			float(m_width) / m_height,
			cam.zNear,
			cam.zFar,
			::bgfx::getCaps()->homogeneousDepth,
			bx::Handedness::Left);
		::bgfx::setViewTransform(view, glm::value_ptr(m_viewMat), glm::value_ptr(m_projMat));
	}

	void CBgfxRenderer::setNormalMatrix(const glm::mat4& modelMat)
	{
		// usually the normal matrix is based on the model view matrix
		// but shading is done in world space (not eye space) so it's just the model matrix
		//glm::mat4 modelViewMat = viewMat * modelMat;

		// if we don't do non-uniform scaling, the normal matrix is the same as the model-view matrix
		// (only the magnitude of the normal is changed, but we normalize either way)
		//glm::mat3 normalMat = glm::mat3(modelMat);

		// use adjugate instead of inverse
		// see https://github.com/graphitemaster/normals_revisited#the-details-of-transforming-normals
		// cofactor is the transpose of the adjugate
		glm::mat3 normalMat = glm::transpose(glm::adjugate(glm::mat3(modelMat)));
		::bgfx::setUniform(m_normalMatrixUniform, glm::value_ptr(normalMat));
	}

	void CBgfxRenderer::blitToScreen(::bgfx::ViewId view /*= MAX_VIEW*/)
	{
		da::core::CCamera& camera = *da::core::CCamera::getCamera();

		::bgfx::setViewName(view, "Tonemapping");
		::bgfx::setViewClear(view, BGFX_CLEAR_NONE);
		::bgfx::setViewRect(view, 0, 0, m_width, m_height);
		::bgfx::setViewFrameBuffer(view, BGFX_INVALID_HANDLE);
		::bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_CULL_CW);
		::bgfx::TextureHandle frameBufferTexture = ::bgfx::getTexture(m_frameBuffer, 0);
		::bgfx::setTexture(0, m_blitSampler, frameBufferTexture);
		::bgfx::TextureHandle skyFrameBufferTexture = ::bgfx::getTexture(m_skyFrameBuffer, 0);
		::bgfx::setTexture(0, m_skySampler, skyFrameBufferTexture);
		float exposureVec[4] = { camera.exposure };
		::bgfx::setUniform(m_exposureVecUniform, exposureVec);
		float tonemappingModeVec[4] = { (float)tonemappingMode };
		::bgfx::setUniform(m_tonemappingModeVecUniform, tonemappingModeVec);
		::bgfx::setVertexBuffer(0, m_blitTriangleBuffer);
		::bgfx::submit(view, { m_blitProgram->getHandle() });
	}

	::bgfx::TextureFormat::Enum CBgfxRenderer::findDepthFormat(uint64_t textureFlags, bool stencil /*= false*/)
	{
		const ::bgfx::TextureFormat::Enum depthFormats[] = { ::bgfx::TextureFormat::D16,::bgfx::TextureFormat::D32 };

		const ::bgfx::TextureFormat::Enum depthStencilFormats[] = { ::bgfx::TextureFormat::D24S8 };

		const ::bgfx::TextureFormat::Enum* formats = stencil ? depthStencilFormats : depthFormats;
		size_t count = stencil ? BX_COUNTOF(depthStencilFormats) : BX_COUNTOF(depthFormats);

		::bgfx::TextureFormat::Enum depthFormat = ::bgfx::TextureFormat::Count;
		for (size_t i = 0; i < count; i++)
		{
			if (::bgfx::isTextureValid(0, false, 1, formats[i], textureFlags))
			{
				depthFormat = formats[i];
				break;
			}
		}

		assert(depthFormat != ::bgfx::TextureFormat::Enum::Count);

		return depthFormat;
	}

	::bgfx::FrameBufferHandle CBgfxRenderer::createFrameBuffer(bool hdr /*= true*/, bool depth /*= true*/)
	{
		::bgfx::TextureHandle textures[2];
		uint8_t attachments = 0;

		const uint64_t samplerFlags = BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIP_POINT |
			BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP;

		::bgfx::TextureFormat::Enum format =
			hdr ? ::bgfx::TextureFormat::RGBA16F : ::bgfx::TextureFormat::BGRA8; // BGRA is often faster (internal GPU format)
		assert(::bgfx::isTextureValid(0, false, 1, format, BGFX_TEXTURE_RT | samplerFlags));
		textures[attachments++] =
			::bgfx::createTexture2D(::bgfx::BackbufferRatio::Equal, false, 1, format, BGFX_TEXTURE_RT | samplerFlags);

		if (depth)
		{
			::bgfx::TextureFormat::Enum depthFormat = findDepthFormat(BGFX_TEXTURE_RT_WRITE_ONLY | samplerFlags);
			assert(depthFormat != ::bgfx::TextureFormat::Enum::Count);
			textures[attachments++] = ::bgfx::createTexture2D(
				::bgfx::BackbufferRatio::Equal, false, 1, depthFormat, BGFX_TEXTURE_RT_WRITE_ONLY | samplerFlags);
		}

		::bgfx::FrameBufferHandle fb = ::bgfx::createFrameBuffer(attachments, textures, true);

		return fb;
	}

	void CBgfxRenderer::reset(uint16_t width, uint16_t height)
	{
		if (!::bgfx::isValid(m_frameBuffer))
		{
			m_frameBuffer = createFrameBuffer(true, true);
			::bgfx::setName(m_frameBuffer, "Render framebuffer (pre-postprocessing)");
		}

		if (!::bgfx::isValid(m_skyFrameBuffer))
		{
			m_skyFrameBuffer = createFrameBuffer(true, true);
			::bgfx::setName(m_skyFrameBuffer, "Sky framebuffer");
		}
		this->m_width = width;
		this->m_height = height;

	}

}

#endif