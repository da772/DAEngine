#pragma once

#include "DAEngine/core/graphics/camera.h"
#include <bgfx/bgfx.h>

namespace da::platform {
	class CBgfxGraphicsMaterial;

#define SHADOW_MAP_SIZE 4

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


		CBgfxGraphicsMaterial* getMaterial() const;
		core::CCamera& getCamera();
		const uint32_t getShadowMapSize() const;
		std::vector<glm::vec4> getFrustumCornersWorldSpace(const glm::mat4& projView);
		std::pair<glm::mat4,glm::mat4> getLightSpaceProjMatrix(const float nearPlane, const float farPlane, int index, glm::mat4 lightView);

		inline FBgfxShadowMaps getShadowMaps() const { return m_shadowMaps; };
		inline size_t getShadowMapsCount() const { return SHADOW_MAP_SIZE; }

		void createFrameBuffers();
		inline bool useShadowSampler() const { return m_useShadowSampler; }

		inline glm::vec3 getLightDir() const { return  glm::normalize(glm::vec3(20.0f, 50.f, 20.0f));}

	private:
		::bgfx::UniformHandle m_depthScaleOffset = BGFX_INVALID_HANDLE;
		FBgfxShadowMaps m_shadowMaps;

		CBgfxGraphicsMaterial* m_material;
		core::CCamera m_camera;
		uint32_t m_shadowMapSize = 2048;
		bool m_useShadowSampler = true;
	};
}