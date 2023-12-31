#pragma once

#include "DAEngine/core/graphics/camera.h"
#include <bgfx/bgfx.h>

namespace da::platform {
	class CBgfxGraphicsMaterial;

	class CBgfxShadowShader {
	public:
		void initialize();
		void shutdown();


		CBgfxGraphicsMaterial* getMaterial() const;
		core::CCamera& getCamera();
		const uint32_t getShadowMapSize() const;

		inline ::bgfx::TextureHandle getShadowMap() const { return m_shadowMapTexture; };
		inline ::bgfx::FrameBufferHandle getFrameBuffer() const { return m_frameBuffer; };

		void createFrameBuffers();
		inline bool useShadowSampler() const { return m_useShadowSampler; }

	private:
		::bgfx::FrameBufferHandle m_frameBuffer = BGFX_INVALID_HANDLE;
		::bgfx::TextureHandle m_shadowMapTexture = BGFX_INVALID_HANDLE;
		::bgfx::UniformHandle m_depthScaleOffset = BGFX_INVALID_HANDLE;
		CBgfxGraphicsMaterial* m_material;
		core::CCamera m_camera;
		uint32_t m_shadowMapSize = 2048;
		bool m_useShadowSampler = true;
	};
}