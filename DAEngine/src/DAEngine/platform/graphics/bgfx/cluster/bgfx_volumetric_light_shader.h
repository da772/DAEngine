#pragma once

#include <bgfx/bgfx.h>

namespace da::platform
{
	class CBgfxGraphicsMaterial;

	class CBgfxVolumetricLightShader
	{
	public:
		void initialize();
		void render(::bgfx::ViewId id, size_t width, size_t height, ::bgfx::TextureHandle lightOcclusionTexture, glm::vec2 lightPos);
		void onReset(size_t width, size_t height);
		void shutdown();
		::bgfx::FrameBufferHandle getBuffer() const;
	private:
		::bgfx::FrameBufferHandle createFrameBuffer(bool hdr);

#ifdef DA_DEBUG
		void renderDebug();
#endif

	private:
		CBgfxGraphicsMaterial* m_pVolLightShader = nullptr;
		::bgfx::FrameBufferHandle m_frameBuffer = BGFX_INVALID_HANDLE;
		::bgfx::UniformHandle m_lightPos = BGFX_INVALID_HANDLE;
		::bgfx::UniformHandle m_blurParams = BGFX_INVALID_HANDLE;
		::bgfx::UniformHandle m_lightOcclusionTexture = BGFX_INVALID_HANDLE;
		::bgfx::VertexBufferHandle m_blitTriangleBuffer = BGFX_INVALID_HANDLE;

		glm::vec4 m_params1 = {0.f,0.f, 1.f,100.f};
		glm::vec4 m_params2 = { .75f,.5f,1.f,0.01f };

#ifdef DA_DEBUG
		bool m_debug = false;
#endif

	};
}