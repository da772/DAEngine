#pragma once
#include "core.h"
#include "daglm.h"
#include <platform/graphics/bgfx/bgfx_graphics_material.h>
#include <bgfx/bgfx.h>

namespace da::platform
{
	class CBgfxSSAOShader
	{
	public:
		void initialize();
		void shutdown();
		void renderSSAO(size_t width, size_t height, ::bgfx::ViewId view, ::bgfx::FrameBufferHandle depthBuffer);
		void renderBlur(size_t width, size_t height, ::bgfx::ViewId view);
		void reset(size_t width, size_t height);
		inline ::bgfx::FrameBufferHandle getBuffer() const { return m_ssaoProcessedBuffer; };

		void bindSSAO();

	private:
		::bgfx::FrameBufferHandle createFrameBuffer(bool hdr);

	private:
		da::graphics::CMaterial* m_pSsaoProgram = nullptr;
		da::graphics::CMaterial* m_pSsaoBlurProgram = nullptr;
		::bgfx::FrameBufferHandle m_ssaoBuffer = BGFX_INVALID_HANDLE;
		::bgfx::FrameBufferHandle m_ssaoProcessedBuffer = BGFX_INVALID_HANDLE;
		::bgfx::VertexBufferHandle m_blitTriangleBuffer = BGFX_INVALID_HANDLE;
		::bgfx::UniformHandle m_uniformHandle = BGFX_INVALID_HANDLE;
		::bgfx::UniformHandle m_uniformParamHandle = BGFX_INVALID_HANDLE;
		::bgfx::UniformHandle m_ssaoUniformHandle = BGFX_INVALID_HANDLE;

		glm::vec4 m_param1 = {0.3f, 0.15f, 0.015f, 0.025 };

#ifdef DA_REVIEW
		void renderDebug();
		bool m_debug = false;
#endif
	};
}