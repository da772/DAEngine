#include "dapch.h"
#include "imgui_factory.h"
#include "platform/platform.h"

#if defined(DA_GRAPHICS_VULKAN) && defined(DA_WINDOW_GLFW)
#include "imgui_vulkan_api.h"
#endif

namespace da::platform
{

	da::platform::CImGuiApi* CImGuiFactory::Create(const CGraphicsApi& graphicsApi)
	{
#if defined(DA_GRAPHICS_VULKAN) && defined(DA_WINDOW_GLFW)
		return new CImGuiVulkanApi(graphicsApi);
#endif
		return nullptr;
	}

}