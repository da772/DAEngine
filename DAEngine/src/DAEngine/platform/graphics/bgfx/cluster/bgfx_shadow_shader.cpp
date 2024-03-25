#include "dapch.h"
#include "bgfx_shadow_shader.h"

#include "daengine/platform/graphics/bgfx/bgfx_graphics_material.h"
#include "../bgfx_util.h"
#include <bx/bx.h>
#include <bx/math.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_relational.hpp>

namespace da::platform {

	void CBgfxShadowShader::initialize()
	{
		if (!useShadowSampler()) {
			m_material = da::graphics::CMaterialFactory::create("shaders/cluster/vs_shadow.sc", "shaders/cluster/fs_shadow.sc");
			m_skmaterial = da::graphics::CMaterialFactory::create("shaders/cluster/vs_sk_shadow.sc", "shaders/cluster/fs_shadow.sc");
		
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

		m_material =   da::graphics::CMaterialFactory::create("shaders/cluster/vs_shadow_sampler.sc", "shaders/cluster/fs_shadow_sampler.sc");
		m_skmaterial = da::graphics::CMaterialFactory::create("shaders/cluster/vs_sk_shadow_sampler.sc", "shaders/cluster/fs_shadow_sampler.sc");

		m_cascadeLevels = bgfx::createUniform("u_cascadePlaneDistances", bgfx::UniformType::Vec4, SHADOW_MAP_SIZE);

		for (size_t i = 0; i < SHADOW_MAP_SIZE; i++) {
			std::string name = std::string("s_shadowMap") + std::to_string(i);
			m_shadowMaps.ShadowMaps[i].Uniform = ::bgfx::createUniform(name.c_str(), ::bgfx::UniformType::Sampler, SHADOW_MAP_SIZE);
		}
	}

	void CBgfxShadowShader::shutdown()
	{

		da::graphics::CMaterialFactory::remove(m_material);
		da::graphics::CMaterialFactory::remove(m_skmaterial);

		BGFXDESTROY(m_cascadeLevels);


		for (size_t i = 0; i < SHADOW_MAP_SIZE; i++) {
			BGFXDESTROY(m_shadowMaps.ShadowMaps[i].Uniform);
			BGFXDESTROY(m_shadowMaps.ShadowMaps[i].FrameBuffer);
			BGFXDESTROY(m_shadowMaps.ShadowMaps[i].Texture);
		}


		if (bgfx::isValid(m_depthScaleOffset)) {
			BGFXDESTROY(m_depthScaleOffset);
		}
	}

	da::graphics::CMaterial* CBgfxShadowShader::getMaterial() const
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
		for (size_t i = 0; i < SHADOW_MAP_SIZE; i++) {
			BGFXTRYDESTROY(m_shadowMaps.ShadowMaps[i].FrameBuffer);
			BGFXTRYDESTROY(m_shadowMaps.ShadowMaps[i].Texture);
		}

		uint8_t size = 0;
		for (size_t i = 0; i < SHADOW_MAP_SIZE; i++) {
			if (m_useShadowSampler) {

				bgfx::TextureHandle fbtextures[] =
				{
					bgfx::createTexture2D(
						  m_shadowMapSize
						, m_shadowMapSize
						, false
						, 1
						, bgfx::TextureFormat::D32F
						, BGFX_TEXTURE_RT | BGFX_SAMPLER_COMPARE_LEQUAL
						),
				};

				m_shadowMaps.ShadowMaps[i].Texture = fbtextures[0];
				size = 1;
				m_shadowMaps.ShadowMaps[i].FrameBuffer = bgfx::createFrameBuffer(size, fbtextures, true);
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
						, bgfx::TextureFormat::D32F
						, BGFX_TEXTURE_RT_WRITE_ONLY
						),
				};

				m_shadowMaps.ShadowMaps[i].Texture = fbtextures[0];
				size = 2;
				m_shadowMaps.ShadowMaps[i].FrameBuffer = bgfx::createFrameBuffer(size, fbtextures, true);
			}
			std::string name = std::string("ShadowDepthBuffer-") + std::to_string(i);
			bgfx::setName(m_shadowMaps.ShadowMaps[i].Texture, name.c_str(), name.size());
		}
		
	}

	std::vector<glm::vec4> CBgfxShadowShader::getFrustumCornersWorldSpace(const glm::mat4& projView)
	{
		const glm::mat4 inv = glm::inverse(projView);

		std::vector<glm::vec4> frustumCorners;
		for (unsigned int x = 0; x < 2; ++x)
		{
			for (unsigned int y = 0; y < 2; ++y)
			{
				for (unsigned int z = 0; z < 2; ++z)
				{
					const glm::vec4 pt = inv * glm::vec4(2.0f * x - 1.0f, 2.0f * y - 1.0f, 2.0f * z - 1.0f, 1.0f);
					frustumCorners.push_back(pt / pt.w);
				}
			}
		}

		return frustumCorners;
	}

	std::pair<glm::mat4, glm::mat4> CBgfxShadowShader::getLightSpaceProjMatrix(const float nearPlane, const float farPlane, int i, glm::mat4 lightView1)
	{	
		// view matrix
		da::core::CCamera* cam = da::core::CCamera::getCamera();

		glm::mat4 camView;

		bx::Vec3 pos(cam->position().x, cam->position().y, cam->position().z);
		glm::vec3 _at = cam->position() + cam->forward();
		bx::Vec3 at(_at.x, _at.y, _at.z);
		bx::Vec3 up(cam->up().x, cam->up().y, cam->up().z);
		bx::mtxLookAt(glm::value_ptr(camView), pos, at, up, bx::Handedness::Right);
		// projection matrix
		
		glm::mat4 proj;

		bx::mtxProj(glm::value_ptr(proj),
			da::core::CCamera::getCamera()->fov,
			1280.f / 720.f,
			nearPlane,
			farPlane,
			bgfx::getCaps()->homogeneousDepth,
			bx::Handedness::Right);

		const auto corners = getFrustumCornersWorldSpace(proj * camView);

		glm::vec3 center = glm::vec3(0, 0, 0);
		for (const auto& v : corners)
		{
			center += glm::vec3(v);
		}
		center /= corners.size();

		const auto lightView = glm::lookAt(center + getLightDir(), center, glm::vec3(0.0f, 0.0f, 1.0f));

		float minX = std::numeric_limits<float>::max();
		float maxX = std::numeric_limits<float>::lowest();
		float minY = std::numeric_limits<float>::max();
		float maxY = std::numeric_limits<float>::lowest();
		float minZ = std::numeric_limits<float>::max();
		float maxZ = std::numeric_limits<float>::lowest();
		for (const auto& v : corners)
		{
			const auto trf = lightView * v;
			minX = std::min(minX, trf.x);
			maxX = std::max(maxX, trf.x);
			minY = std::min(minY, trf.y);
			maxY = std::max(maxY, trf.y);
			minZ = std::min(minZ, trf.z);
			maxZ = std::max(maxZ, trf.z);
		}

		// Tune this parameter according to the scene
		constexpr float zMult = 1.f;
		if (minZ < 0)
		{
			minZ *= zMult;
		}
		else
		{
			minZ /= zMult;
		}
		if (maxZ < 0)
		{
			maxZ /= zMult;
		}
		else
		{
			maxZ *= zMult;
		}


		glm::mat4 lightProj;
		bx::mtxOrtho(glm::value_ptr(lightProj), minX, maxX, minY, maxY, minZ, maxZ, 0, ::bgfx::getCaps()->homogeneousDepth, bx::Handedness::Right);

		return { lightProj, lightView };
	}

	da::graphics::CMaterial* CBgfxShadowShader::getSKMaterial() const
	{
		return m_skmaterial;
	}

}