#pragma once
#include "core/core.h"
#include "bgfx/bgfx.h"
#include "dastd.h"
#include "daglm.h"

namespace da::graphics
{
	class CMaterial;
}

namespace da::platform
{

	class CBgfxBloomShader
	{
	public:
		void initialize(size_t width, size_t height);
		void shutdown();

		void render(::bgfx::ViewId view, ::bgfx::TextureHandle texture, size_t width, size_t height);
		void renderBlur(::bgfx::ViewId view, size_t width, size_t height);
		void onReset(size_t width, size_t height);
		::bgfx::FrameBufferHandle getBuffer() const;

	private:
		::bgfx::FrameBufferHandle createFrameBuffer(bool hdr, size_t width = 0, size_t height = 0);
#ifdef DA_REVIEW
		void onDebugRender();
#endif

	private:
		::bgfx::VertexBufferHandle m_blitTriangleBuffer = BGFX_INVALID_HANDLE;
		da::graphics::CMaterial* m_pBloomShader;
		da::graphics::CMaterial* m_pDownscaleShader;
		da::graphics::CMaterial* m_pUpscaleShader;
		::bgfx::UniformHandle m_textureUniform = BGFX_INVALID_HANDLE;
		::bgfx::UniformHandle m_bloomParamsUniform = BGFX_INVALID_HANDLE;
		::bgfx::UniformHandle m_pixelSizeUniform = BGFX_INVALID_HANDLE;
		::bgfx::UniformHandle m_upscaleIntensityUniform = BGFX_INVALID_HANDLE;
		::bgfx::FrameBufferHandle m_frameBuffer = BGFX_INVALID_HANDLE;
		std::vector<::bgfx::FrameBufferHandle> m_processedFrameBuffers;

		glm::vec3 m_bloomParams = { .025, 0.7152f, 0.0722f };
		glm::vec4 m_intensity = { .60f,1.f,1.f,1.f };
		uint8_t m_bloomAmount = 5;
		size_t m_width, m_height;

#ifdef DA_REVIEW
		bool m_debug = false;
#endif
	};
}