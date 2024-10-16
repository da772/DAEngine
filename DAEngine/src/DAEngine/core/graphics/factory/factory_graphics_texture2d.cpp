#include "dapch.h"

#include "factory_graphics_texture2d.h"

#ifdef DA_GRAPHICS_VULKAN
#include "platform/graphics/vulkan/vulkan_graphics_texture2d.h"
#endif

namespace da::core
{

	da::core::CGraphicsTexture2D* CTexture2DFactory::Create(const CString& path, CGraphicsApi& graphicsApi)
	{
#ifdef DA_GRAPHICS_VULKAN
		return new da::platform::CVulkanGraphicsTexture2D(path, graphicsApi);
#endif

		return nullptr;
	}

}