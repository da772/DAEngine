#include "dapch.h"
#include "imgui_factory.h"
#include "platform/platform.h"


#if defined(DA_GRAPHICS_BGFX) && defined(DA_WINDOW_GLFW)
#include "platform/imgui/bgfx/imgui_bgfx_api.h"
#endif

#if defined(DA_GRAPHICS_VULKAN) && defined(DA_WINDOW_GLFW)
#include "platform/imgui/vulkan/imgui_vulkan_api.h"
#endif

namespace da::core
{

	da::core::CImGuiApi* CImGuiFactory::Create(CGraphicsApi* graphicsApi)
	{
#if defined(DA_GRAPHICS_BGFX) && defined(DA_WINDOW_GLFW)
		return new platform::CImGuiBgfxApi(graphicsApi);
#endif
#if defined(DA_GRAPHICS_VULKAN) && defined(DA_WINDOW_GLFW)
		return new platform::CImGuiVulkanApi(graphicsApi);
#endif
		return nullptr;
	}

}