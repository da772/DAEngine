#pragma once

#include <bgfx/bgfx.h>

#define BGFXDESTROY(x) ASSERT(::bgfx::isValid(x)) ::bgfx::destroy(x); x = BGFX_INVALID_HANDLE;
#define BGFXTRYDESTROY(x) if (::bgfx::isValid(x)) { ::bgfx::destroy(x); x = BGFX_INVALID_HANDLE ;}

namespace da::platform {
	class CBgfxUtil {
	public:

		struct PosVertex
		{
			float x;
			float y;
			float z;

			static void init()
			{
				s_initialized = true;
				layout.begin().add(::bgfx::Attrib::Position, 3, ::bgfx::AttribType::Float).end();
			}

			
			static ::bgfx::VertexLayout layout;
			static bool s_initialized;
		};

		static inline const PosVertex& getScreenVertices() {
			if (!s_posVertex.s_initialized) s_posVertex.init();
			return s_posVertex;
		}

		static inline ::bgfx::FrameBufferHandle createFrameBuffer(bool hdr)
		{
			::bgfx::TextureHandle textures[2];
			uint8_t attachments = 0;

			const uint64_t samplerFlags = BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIP_POINT |
				BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_TEXTURE_RT;

			::bgfx::TextureFormat::Enum format =
				hdr ? ::bgfx::TextureFormat::RGBA16F : ::bgfx::TextureFormat::BGRA8; // BGRA is often faster (internal GPU format)
			assert(::bgfx::isTextureValid(0, false, 1, format, samplerFlags));
			textures[attachments++] =
				::bgfx::createTexture2D(::bgfx::BackbufferRatio::Equal, false, 1, format, samplerFlags);


			::bgfx::FrameBufferHandle fb = ::bgfx::createFrameBuffer(attachments, textures, true);

			return fb;
		}

	private:
		static PosVertex s_posVertex;
	};

	__declspec(selectany) bool CBgfxUtil::PosVertex::s_initialized;
	__declspec(selectany) ::bgfx::VertexLayout CBgfxUtil::PosVertex::layout;
	__declspec(selectany) da::platform::CBgfxUtil::PosVertex CBgfxUtil::s_posVertex;

}