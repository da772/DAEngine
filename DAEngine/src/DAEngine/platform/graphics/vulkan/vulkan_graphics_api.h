#pragma once
#include "core/core.h"

#if defined(DA_GRAPHICS_VULKAN) && defined(DA_WINDOW_GLFW)
#include "graphics/graphics_api.h"
#include <vulkan/vulkan.h>
#include <optional>
#include <memory>
#include "graphics/graphics_smesh.h"


namespace da::core {
	class CMaterial;
}

namespace da::platform
{
	class CVulkanGraphicsTexture2D;
	class CVulkanGraphicsPipeline;

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		inline bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	struct FrameBufferAttachment {
		VkImage image;
		VkDeviceMemory mem;
		VkImageView view;
	};

	struct OffscreenPass {
		int32_t width, height;
		VkFramebuffer frameBuffer;
		FrameBufferAttachment depth;
		VkRenderPass renderPass;
		VkSampler depthSampler;
		VkDescriptorImageInfo descriptor;
	};

	class CVulkanGraphicsApi : public graphics::CGraphicsApi
	{
	public:
		CVulkanGraphicsApi(da::core::CWindow* windowModule);
		~CVulkanGraphicsApi();
		virtual void initialize() override;
		virtual void update() override;
		virtual void shutdown() override;
		virtual void submitPipeline(da::graphics::CGraphicsPipeline* pipeline);

	public:
		inline const VkInstance& getInstance() const { return m_instance; }
		inline const VkPhysicalDevice& getPhysicalDevice() const { return m_physicalDevice; }
		inline const VkDevice& getDevice() const { return m_device; }
		inline const uint32_t& getQueueFamily() const { return m_queueFamilyIndices[0]; }
		inline const VkQueue& getQueue() const { return m_graphicsQueue; }
		inline const VkSurfaceKHR getSurface() const { return m_surface; }
		inline const VkSwapchainKHR& getSwapChain() const { return m_swapChain; }
        inline const VkCommandPool& getCommandPool() const {return m_commandPool; }
        inline const VkRenderPass& getRenderPass() const {return m_renderPass;}
		inline const VkExtent2D getSwapChainExt() const { return m_swapChainExtent; }
		

	private:
		void createInstance();
		bool checkValidationLayerSupport(const std::vector<const char*>& validationLayers);
		std::vector<const char*> getRequiredExtensions();
		void setupDebugCallback();
		void selectPhysicalDevice();
		void createLogicalDevice();
		void createSurface();
		void createSwapChain();
		void createImageViews();
		void createRenderPass();
		void prepareOffscreenRenderpass();
		void createFramebuffers();
		void createCommandPool();
		void createCommandBuffers();
		void createSyncObjects();
		void createTextureImageView(); 
		void createDepthResources();
		void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSample, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
			VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		void beginRecordingCommandBuffer(VkCommandBuffer commandBuffer, VkFramebuffer frameBuffer, VkRenderPass renderPass);
		void beginRecordingShadowCommandBuffer(VkCommandBuffer commandBuffer, VkFramebuffer frameBuffer, VkRenderPass renderPass);
        void stopRecordingCommandBuffer(VkCommandBuffer commandBuffer);
		
		void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
		void createColorResources();

		VkPhysicalDevice findDevices(const std::vector<VkPhysicalDevice>& devices);

		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, std::optional<VkSurfaceKHR> surface);
		VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkFormat findDepthFormat();

		void prepareOffscreenFramebuffer();

		void cleanupSwapChain();
		void recreateSwapChain();

    public:
        VkCommandBuffer beginSingleTimeCommands() const;
        void endSingleTimeCommands(VkCommandBuffer commandBuffer) const;
        void immediateSubmit(std::function<void (VkCommandBuffer cmd)>&& func) const;
        VkSampleCountFlagBits getMaxUsableSampleCount() const;
		void submitRenderFunction(std::function<void(VkCommandBuffer cmd)>* func);
		void removeRenderFunction(std::function<void(VkCommandBuffer cmd)>* func);
		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags peoperties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
		VkImageView createImageView(VkImage image, VkImageViewType type, uint32_t layerCount, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t);
		VkAllocationCallbacks& getAllocCallbacks() { return m_allocCallbacks; }
		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		const int MAX_FRAMES_IN_FLIGHT = 2;

	private:
		VkInstance m_instance;
		VkDebugUtilsMessengerEXT m_debugMessenger;
		VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
		VkDevice m_device;
		VkQueue m_graphicsQueue;
		std::vector<const char*> m_validationLayers;
		VkSurfaceKHR m_surface;
		VkQueue m_presentQueue;
		std::vector<VkImage> m_swapChainImages;
		std::vector<VkImageView> m_swapChainImageViews;
		VkFormat m_swapChainImageFormat;
		VkExtent2D m_swapChainExtent;
		
		VkCommandPool m_commandPool;
		VkSwapchainKHR m_swapChain;
		VkRenderPass m_renderPass;
		std::vector<VkCommandBuffer> m_commandBuffers;
		std::vector<VkSemaphore> m_imageAvailableSemaphores;
		std::vector<VkSemaphore> m_renderFinishedSemaphores;
		std::vector<VkFence> m_inFlightFences;
		std::vector<VkFramebuffer> m_swapChainFramebuffers;
		uint32_t m_queueFamilyIndices[2];

		OffscreenPass m_shadowPass;

		VkImage m_depthImage;
		VkDeviceMemory m_depthImageMemory;
		VkImageView m_depthImageView;
		VkSampleCountFlagBits m_msaaSamples = VK_SAMPLE_COUNT_1_BIT;
        uint32_t m_imageIndex;


		std::vector<CVulkanGraphicsPipeline*> m_pipelines;

		// Render target
		VkImage m_colorImage;
		VkDeviceMemory m_colorImageMemory;
		VkImageView m_colorImageView;

		uint32_t m_currentFrame = 0;

		uint32_t m_mipLevels = 0;

		std::vector<std::function<void(VkCommandBuffer cmd)>*> m_renderFunctions;

		VkAllocationCallbacks m_allocCallbacks;

	};
}
#endif
