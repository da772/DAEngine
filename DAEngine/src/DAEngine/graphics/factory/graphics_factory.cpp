
#include "platform/platform.h"
#include "graphics/factory/graphics_factory.h"

#ifdef DA_GRAPHICS_VULKAN
#include "platform/graphics/vulkan/vulkan_graphics_api.h"
#endif

#ifdef DA_GRAPHICS_BGFX
#include "platform/graphics/bgfx/bgfx_graphics_api.h"
#endif

namespace da
{

	CGraphicsApi* CGraphicsFactory::Create(da::CWindow* window)
	{
#ifdef DA_GRAPHICS_BGFX
		return new CbgfxGraphicsApi(window);
		#elif defined(DA_GRAPHICS_VULKAN)
		return new CVulkanGraphicsApi(window);
#endif
		return nullptr;

	}

}

