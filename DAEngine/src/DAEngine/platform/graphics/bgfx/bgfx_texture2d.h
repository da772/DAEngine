#pragma once
#include "core.h"

#ifdef DA_GRAPHICS_BGFX
#include "graphics/graphics_texture2d.h"
#include "core/asset/asset.h"

namespace da
{
	class CTexture2DFactory;
}

namespace da {
	class CBgfxTexture2D : public da::CGraphicsTexture2D {
	public:
		inline const uint16_t getHandle() const { return m_handle; }
		inline virtual void* getTextureNative() const { return (void*)&m_handle; }
		void destroy();

		friend class da::CFactory<CGraphicsTexture2D>;

	protected:
		CBgfxTexture2D();
		CBgfxTexture2D(const std::string& s);
		CBgfxTexture2D(const std::string& name, uint32_t width, uint32_t height);
		CBgfxTexture2D(const std::string& name, uint32_t width, uint32_t height, char* data);

		~CBgfxTexture2D() override;

	private:
		da::CAsset m_fileAsset;
		uint16_t m_handle = UINT16_MAX;
		std::mutex* m_mutex;

		friend class da::CTexture2DFactory;

	};
}
#endif