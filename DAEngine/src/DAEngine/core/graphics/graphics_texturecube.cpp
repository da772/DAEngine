#include "dapch.h"
#include "graphics_texturecube.h"

namespace da::core
{
	
	CGraphicsTextureCube::CGraphicsTextureCube(const CBasicString<memory::CGraphicsAllocator>& path, CGraphicsApi& graphicsApi) : m_path(path), m_graphicsApi(graphicsApi)
	{

	}

}