
#include "graphics/imgui/imgui_factory.h"
#include "platform/platform.h"


#if defined(DA_GRAPHICS_BGFX) && defined(DA_WINDOW_GLFW)
#include "platform/imgui/bgfx/imgui_bgfx_api.h"
#endif

#if defined(DA_GRAPHICS_VULKAN) && defined(DA_WINDOW_GLFW)
#include "platform/imgui/vulkan/imgui_vulkan_api.h"
#endif

namespace da
{

	da::CImGuiApi* CImGuiFactory::Create(da::CGraphicsApi* graphicsApi)
	{
#if defined(DA_GRAPHICS_BGFX) && defined(DA_WINDOW_GLFW)
		return new CImGuiBgfxApi(graphicsApi);
#endif
#if defined(DA_GRAPHICS_VULKAN) && defined(DA_WINDOW_GLFW)
		return new CImGuiVulkanApi(graphicsApi);
#endif
		return nullptr;
	}

}