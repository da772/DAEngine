#pragma once

#include "graphics/camera.h"
#include <bgfx/bgfx.h>

namespace da
{
	class CMaterial;
}

namespace da {

#define SHADOW_MAP_SIZE 3

	struct FBgfxShadowMap
	{
		::bgfx::TextureHandle Texture = BGFX_INVALID_HANDLE;
		::bgfx::FrameBufferHandle FrameBuffer = BGFX_INVALID_HANDLE;
		::bgfx::UniformHandle Uniform = BGFX_INVALID_HANDLE;
	};

	struct FBgfxShadowMaps
	{
		FBgfxShadowMap ShadowMaps[SHADOW_MAP_SIZE];
	};

	class CBgfxShadowShader {
	public:
		void initialize();
		void shutdown();


		da::CMaterial* getMaterial() const;
		da::CMaterial* getSKMaterial() const;
		CCamera& getCamera();
		const uint32_t getShadowMapSize() const;
		std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& projView);
		std::pair<glm::mat4, glm::mat4> getLightSpaceProjMatrix(const float nearPlane, const float farPlane, int index, glm::mat4 lightView);

		inline FBgfxShadowMaps getShadowMaps() const { return m_shadowMaps; };
		inline size_t getShadowMapsCount() const { return SHADOW_MAP_SIZE; }

		void createFrameBuffers();
		inline bool useShadowSampler() const { return m_useShadowSampler; }

		inline ::bgfx::UniformHandle getCascadeLevelUniform() const { return m_cascadeLevels; }

		inline glm::vec3& getLightDir() { return m_shadowDir; }

	private:
		::bgfx::UniformHandle m_depthScaleOffset = BGFX_INVALID_HANDLE;
		::bgfx::UniformHandle m_cascadeLevels = BGFX_INVALID_HANDLE;
		FBgfxShadowMaps m_shadowMaps;
		glm::vec3 m_shadowDir = { 0.348f, 0.870f, 0.7f };

		da::CMaterial* m_material;
		da::CMaterial* m_skmaterial;
		CCamera m_camera;
		uint32_t m_shadowMapSize = 4096;
		bool m_useShadowSampler = true;
	};
}