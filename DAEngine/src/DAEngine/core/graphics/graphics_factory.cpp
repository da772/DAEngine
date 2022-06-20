#include "dapch.h"
#include "platform/platform.h"
#include "graphics_factory.h"
#include "core/memory/global_allocator.h"

#ifdef DA_GRAPHICS_VULKAN
#include "platform/graphics/vulkan/vulkan_graphics_api.h"
#endif

namespace da::core
{

	CGraphicsApi* CGraphicsFactory::Create(const CWindow& window)
	{
#ifdef DA_GRAPHICS_VULKAN
		return new platform::CVulkanGraphicsApi(window);
#endif
		return nullptr;

	}

}

