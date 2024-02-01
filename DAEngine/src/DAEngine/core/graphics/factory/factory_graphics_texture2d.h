#pragma once
#include "daengine/core/graphics/graphics_texture2d.h"

namespace da::graphics
{
	class CTexture2DFactory
	{
	public:
		static CGraphicsTexture2D* Create(const std::string& path, CGraphicsApi& graphicsApi);
		static CGraphicsTexture2D* Create(const std::string& path);
		static CGraphicsTexture2D* Create(const std::string& name, size_t width, size_t height, char* data);
	};
}
