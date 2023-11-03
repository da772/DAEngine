#include "dapch.h"
#include "platform/platform.h"
#include "graphics_factory.h"

#ifdef DA_GRAPHICS_VULKAN
#include "platform/graphics/vulkan/vulkan_graphics_api.h"
#endif

#ifdef DA_GRAPHICS_BGFX
#include "platform/graphics/bgfx/bgfx_graphics_api.h"
#endif

namespace da::core
{

	CGraphicsApi* CGraphicsFactory::Create(CWindow* window)
	{
#ifdef DA_GRAPHICS_BGFX
		return new platform::CbgfxGraphicsApi(window);
		#elif defined(DA_GRAPHICS_VULKAN)
		return new platform::CVulkanGraphicsApi(window);
#endif
		return nullptr;

	}

}

