#pragma once

#include <bgfx/bgfx.h>

namespace da {
	namespace graphics {
		class CMaterial;
	}
}

namespace da::platform {
	class CBgfxSsr {
	public:
		void initialize(uint16_t width, uint16_t height);
		void renderSSR(bgfx::ViewId view, ::bgfx::FrameBufferHandle color, ::bgfx::FrameBufferHandle depth, ::bgfx::FrameBufferHandle positions);
		void shutdown();

		void reset(uint16_t width, uint16_t height);

		inline ::bgfx::FrameBufferHandle getFrameBuffer() const { return m_frameBuffer; }

	private:
		::bgfx::UniformHandle m_Color = BGFX_INVALID_HANDLE, m_depth = BGFX_INVALID_HANDLE, m_positions = BGFX_INVALID_HANDLE;
		::bgfx::FrameBufferHandle m_frameBuffer = BGFX_INVALID_HANDLE;
		::bgfx::VertexBufferHandle m_vertexBuffer = BGFX_INVALID_HANDLE;
		da::graphics::CMaterial* m_material;
		uint16_t m_width, m_height;

	};
}