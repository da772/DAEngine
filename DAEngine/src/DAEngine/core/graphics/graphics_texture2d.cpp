#include "dapch.h"
#include "graphics_texture2d.h"

namespace da::core
{
	CGraphicsTexture2D::CGraphicsTexture2D(const std::string& path, CGraphicsApi& graphicsApi) : m_path(path), m_graphicsApi(graphicsApi), m_textureNative(nullptr)
	{

	}

}