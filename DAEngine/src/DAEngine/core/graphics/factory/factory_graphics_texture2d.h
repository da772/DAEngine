#pragma once
#include "DAEngine/core/factory.h"
#include "daengine/core/graphics/graphics_texture2d.h"

namespace da::graphics
{
	class CTexture2DFactory : da::core::CFactory<CGraphicsTexture2D>
	{
	public:
		static CGraphicsTexture2D* Create(const std::string& path, CGraphicsApi& graphicsApi);
		static CGraphicsTexture2D* Create(const std::string& path);
		static CGraphicsTexture2D* Create(const std::string& name, size_t width, size_t height, char* data);
		static void Remove(CGraphicsTexture2D* texture);

	private:
		static CTexture2DFactory ms_factory;
#ifdef DA_REVIEW
		friend class da::core::CFactoryDebug;
#endif
	};
}
