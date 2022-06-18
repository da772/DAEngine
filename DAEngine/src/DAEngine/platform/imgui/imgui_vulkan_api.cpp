#include "dapch.h"
#include "imgui_vulkan_api.h"


#if defined(DA_GRAPHICS_VULKAN) && defined(DA_WINDOW_GLFW)

#include <imgui.h>
#include <backends/imgui_impl_vulkan.cpp>
#include <backends/imgui_impl_glfw.cpp>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

namespace da::platform {

	CImGuiVulkanApi::CImGuiVulkanApi(core::CGraphicsApi& graphicsApi) : 
        CImGuiApi(graphicsApi)
        ,m_vulkanGraphics(*static_cast<platform::CVulkanGraphicsApi*>(&graphicsApi))
        ,m_funcPtr(new std::function<void(VkCommandBuffer cmd)>(std::bind(&CImGuiVulkanApi::renderImGui, this, std::placeholders::_1)))
	{

    }

	void CImGuiVulkanApi::onInitialize()
	{
        

        //1: create descriptor pool for IMGUI
            // the size of the pool is very oversize, but it's copied from imgui demo itself.
        VkDescriptorPoolSize pool_sizes[] =
        {
            { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
            { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
            { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
            { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
        };

        VkDescriptorPoolCreateInfo pool_info = {};
        pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
        pool_info.flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT;
        pool_info.maxSets = 1000;
        pool_info.poolSizeCount = sizeof(pool_sizes) / sizeof(VkDescriptorPoolSize);
        pool_info.pPoolSizes = pool_sizes;


        auto result = vkCreateDescriptorPool(m_vulkanGraphics.getDevice(), &pool_info, nullptr, &m_imguiPool);
       // VK_CHECK(result, VK_SUCCESS);
    
        // 2: initialize imgui library
        //this initializes imgui for Vulkan GLFW
        ImGui_ImplGlfw_InitForVulkan((GLFWwindow*)m_window.getNativeWindow(), true);

        //this initializes imgui for Vulkan
        ImGui_ImplVulkan_InitInfo init_info = {};
        init_info.Instance = m_vulkanGraphics.getInstance();
        init_info.PhysicalDevice = m_vulkanGraphics.getPhysicalDevice();
        init_info.Device = m_vulkanGraphics.getDevice();
        init_info.Queue = m_vulkanGraphics.getQueue();
        init_info.DescriptorPool = m_imguiPool;
        init_info.MinImageCount = 3;
        init_info.ImageCount = 3;
        init_info.MSAASamples = m_vulkanGraphics.getMaxUsableSampleCount();
        
        ImGui_ImplVulkan_Init(&init_info, m_vulkanGraphics.getRenderPass());
        
        m_vulkanGraphics.immediateSubmit([] (VkCommandBuffer cmd) {
            ImGui_ImplVulkan_CreateFontsTexture(cmd);
        });
        //clear font textures from cpu data
        ImGui_ImplVulkan_DestroyFontUploadObjects();
        m_vulkanGraphics.submitRenderFunction(m_funcPtr);
	}

    bool open = true;

	void CImGuiVulkanApi::onUpdate()
	{
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //imgui commands
        ImGui::ShowDemoWindow(&open);
        ImGui::Render();
	}

	void CImGuiVulkanApi::onShutdown()
	{
        m_vulkanGraphics.removeRenderFunction(m_funcPtr);
        delete m_funcPtr;
        vkDeviceWaitIdle(m_vulkanGraphics.getDevice());
        // Todo:: shutdown before vulkan and glfw
        vkDestroyDescriptorPool(m_vulkanGraphics.getDevice(), m_imguiPool, nullptr);
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
	}

	void CImGuiVulkanApi::renderImGui(VkCommandBuffer cmd)
	{
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd);
	}

}

#endif
