#include "dapch.h"
#include "platform/platform.h"
#include "graphics_factory.h"


#ifdef DA_GRAPHICS_VULKAN
#include "vulkan_graphics_api.h"
#endif

namespace da::platform
{

	CGraphicsApi* CGraphicsFactory::Create(const CWindow& window)
	{
#ifdef DA_GRAPHICS_VULKAN
		return new CVulkanGraphicsApi(window);
#endif
		
	}

}

