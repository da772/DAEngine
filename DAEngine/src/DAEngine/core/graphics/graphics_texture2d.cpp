#include "dapch.h"
#include "graphics_texture2d.h"

namespace da::core
{
	CGraphicsTexture2D::CGraphicsTexture2D(const std::string& path) : m_path(path), m_textureNative(nullptr)
	{

	}

	CGraphicsTexture2D::CGraphicsTexture2D(const std::string& name, uint32_t width, uint32_t height) : m_path(name), m_width(width), m_height(height), m_textureNative(nullptr)
	{

	}

}