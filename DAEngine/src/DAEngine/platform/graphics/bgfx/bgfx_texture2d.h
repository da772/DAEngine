#pragma once

#ifdef DA_GRAPHICS_BGFX
#include "daengine/core/graphics/graphics_texture2d.h"
#include "asset/asset.h"

namespace da::platform::bgfx {
	class CBgfxTexture2D : public da::core::CGraphicsTexture2D {
	public:
		CBgfxTexture2D();
		CBgfxTexture2D(const std::string& s);
		CBgfxTexture2D(const std::string& name, uint32_t width, uint32_t height);

		inline const uint16_t getHandle() const { return m_handle; }
		void destroy();

	private:
		da::CAsset m_fileAsset;
		uint16_t m_handle = UINT16_MAX;

	};
}
#endif