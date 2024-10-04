#pragma once
#include "graphics/imgui/imgui_api.h"

#include "platform/platform.h"

#if defined(DA_GRAPHICS_VULKAN) && defined(DA_WINDOW_GLFW)
#include "platform/graphics/vulkan/vulkan_graphics_api.h"

namespace da
{
	class CImGuiVulkanApi : public CImGuiApi {
	public:
		CImGuiVulkanApi(da::CGraphicsApi* graphicsApi);

		virtual void onInitialize() override;
		virtual void onUpdate() override;
		virtual void onShutdown() override;
	private:
		void renderImGui(VkCommandBuffer cmd);
	private:
		CVulkanGraphicsApi* m_vulkanGraphics;
        VkDescriptorPool m_imguiPool;
		std::function<void(VkCommandBuffer)>* m_funcPtr;
	};

}

#endif
