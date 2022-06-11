#include "dapch.h"

#include "imgui_vulkan_api.h"
#include "platform/platform.h"

#if defined(DA_GRAPHICS_VULKAN) && defined(DA_WINDOW_GLFW)

#include <imgui.h>
#include <backends/imgui_impl_vulkan.cpp>
#include <backends/imgui_impl_glfw.cpp>

namespace da::platform {

	CImGuiVulkanApi::CImGuiVulkanApi(const CGraphicsApi& graphicsApi) : CImGuiApi(graphicsApi), m_vulkanGraphics(*static_cast<const platform::graphics::CVulkanGraphicsApi*>(&graphicsApi))
	{

	}

	ImGui_ImplVulkanH_Window wd;

	void CImGuiVulkanApi::onInitialize()
	{
		wd.Surface = m_vulkanGraphics.getSurface();
		wd.Swapchain = m_vulkanGraphics.getSwapChain();
		ImGui_ImplVulkanH_CreateOrResizeWindow(m_vulkanGraphics.getInstance(), m_vulkanGraphics.getPhysicalDevice(), m_vulkanGraphics.getDevice(), &wd, m_vulkanGraphics.getQueueFamily(), NULL, 1280, 720, 1);
	}

	VkRenderPass renderPass;

	void CImGuiVulkanApi::onUpdate()
	{
		ImGui_ImplVulkan_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGui::Render();

		VkResult err;
		ImGui_ImplVulkanH_Frame* fd = &wd.Frames[wd.FrameIndex];
		{
			err = vkWaitForFences(m_vulkanGraphics.getDevice(), 1, &fd->Fence, VK_TRUE, UINT64_MAX);    // wait indefinitely instead of periodically checking
			//check_vk_result(err);

			err = vkResetFences(m_vulkanGraphics.getDevice(), 1, &fd->Fence);
			//check_vk_result(err);
		}

		VkRenderPassBeginInfo info = {};
		info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		info.renderPass = renderPass;
		info.framebuffer = fd->Framebuffer;
		info.renderArea.extent.width = wd.Width;
		info.renderArea.extent.height = wd.Height;
		info.clearValueCount = 1;
		info.pClearValues = &wd.ClearValue;
		vkCmdBeginRenderPass(fd->CommandBuffer, &info, VK_SUBPASS_CONTENTS_INLINE);

		// Record dear imgui primitives into command buffer
		ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), fd->CommandBuffer);

		// Submit command buffer
		vkCmdEndRenderPass(fd->CommandBuffer);
		{
			VkPipelineStageFlags wait_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			VkSubmitInfo info = {};
			info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			info.waitSemaphoreCount = 1;
			info.pWaitDstStageMask = &wait_stage;
			info.commandBufferCount = 1;
			info.pCommandBuffers = &fd->CommandBuffer;
			info.signalSemaphoreCount = 1;

			err = vkEndCommandBuffer(fd->CommandBuffer);
			//check_vk_result(err);
			err = vkQueueSubmit(m_vulkanGraphics.getQueue(), 1, &info, fd->Fence);
			//check_vk_result(err);
		}
	}

	void CImGuiVulkanApi::onShutdown()
	{

	}
}

#endif