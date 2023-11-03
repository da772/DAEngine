#pragma once
#include "daengine/core/graphics/graphics_texture2d.h"

namespace da::core
{
	class CTexture2DFactory
	{
	public:
		static CGraphicsTexture2D* Create(const std::string& path, CGraphicsApi& graphicsApi);
	};
}
