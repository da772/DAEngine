#include "dapch.h"

#include "factory_graphics_texture2d.h"

#ifdef DA_GRAPHICS_VULKAN
#include "platform/graphics/vulkan/vulkan_graphics_texture2d.h"
#endif

#ifdef DA_GRAPHICS_BGFX
#include "platform/graphics/bgfx/bgfx_texture2d.h"
#endif

namespace da::graphics
{

	CGraphicsTexture2D* CTexture2DFactory::Create(const std::string& path, CGraphicsApi& graphicsApi)
	{
#ifdef DA_GRAPHICS_BGFX
		return new da::platform::bgfx::CBgfxTexture2D(path);
#elif defined(DA_GRAPHICS_VULKAN)
		return new da::platform::CVulkanGraphicsTexture2D(path, graphicsApi);
#endif

		return nullptr;
	}

	da::graphics::CGraphicsTexture2D* CTexture2DFactory::Create(const std::string& path)
	{
#ifdef DA_GRAPHICS_BGFX
		return new da::platform::bgfx::CBgfxTexture2D(path);
#endif
	}

	da::graphics::CGraphicsTexture2D* CTexture2DFactory::Create(const std::string& name, size_t width, size_t height, char* data)
	{
#ifdef DA_GRAPHICS_BGFX
		return new da::platform::bgfx::CBgfxTexture2D(name, width, height, data);
#endif
	}

}