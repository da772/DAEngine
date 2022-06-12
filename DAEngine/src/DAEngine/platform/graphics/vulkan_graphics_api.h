#pragma once
#include "platform/platform.h"

#if defined(DA_GRAPHICS_VULKAN) && defined(DA_WINDOW_GLFW)
#include "graphics_api.h"
#include "vulkan/vulkan.h"
#include <optional>

namespace da::platform::graphics
{

	struct QueueFamilyIndices {
		std::optional<uint32_t> graphicsFamily;
		std::optional<uint32_t> presentFamily;

		inline bool isComplete() {
			return graphicsFamily.has_value() && presentFamily.has_value();
		}
	};

	class CVulkanGraphicsApi : public CGraphicsApi
	{
	public:
		CVulkanGraphicsApi(const CWindow& windowModule);

		virtual void initalize() override;
		virtual void update() override;
		virtual void shutdown() override;

	public:
		inline const VkInstance& getInstance() const { return m_instance; }
		inline const VkPhysicalDevice& getPhysicalDevice() const { return m_physicalDevice; }
		inline const VkDevice& getDevice() const { return m_device; }
		inline const uint32_t& getQueueFamily() const { return m_queueFamilyIndices[0]; }
		inline const VkQueue& getQueue() const { return m_graphicsQueue; }
		inline const VkSurfaceKHR getSurface() const { return m_surface; }
		inline const VkSwapchainKHR& getSwapChain() const { return m_swapChain; }
		

	private:
		void createInstance();
		bool checkValidationLayerSupport(const TList<const char*>& validationLayers);
		TList<const char*> getRequiredExtensions();
		void setupDebugCallback();
		void selectPhysicalDevice();
		void createLogicalDevice();
		void createSurface();
		void createSwapChain();
		void createImageViews();
		void createDescriptorSetLayout();
		void createGraphicsPipeline();
		void createRenderPass();
		void createFramebuffers();
		void createVertexBuffers();
		void createIndexBuffers();
		void createUniformBuffers();
		void createDescriptorPools();
		void createDescriptorSets();
		void createCommandPool();
		void createCommandBuffers();
		void createSyncObjects();
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags peoperties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

		VkPhysicalDevice findDevices(const TList<VkPhysicalDevice>& devices);

		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, std::optional<VkSurfaceKHR> surface);

		void updateUniformBuffer(uint32_t frame);

		void cleanupSwapChain();
		void recreateSwapChain();

		const int MAX_FRAMES_IN_FLIGHT = 2;
	private:
		VkInstance m_instance;
		VkDebugUtilsMessengerEXT m_debugMessenger;
		VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
		VkDevice m_device;
		VkQueue m_graphicsQueue;
		TList<const char*> m_validationLayers;
		VkSurfaceKHR m_surface;
		VkQueue m_presentQueue;
		TList<VkImage> m_swapChainImages;
		TList<VkImageView> m_swapChainImageViews;
		VkFormat m_swapChainImageFormat;
		VkExtent2D m_swapChainExtent;
		VkPipelineLayout m_pipelineLayout;
		VkCommandPool m_commandPool;
		VkSwapchainKHR m_swapChain;
		VkPipeline m_graphicsPipeline;
		VkRenderPass m_renderPass;
		TList<VkCommandBuffer> m_commandBuffers;
		TList<VkSemaphore> m_imageAvailableSemaphores;
		TList<VkSemaphore> m_renderFinishedSemaphores;
		TList<VkFence> m_inFlightFences;
		TList<VkFramebuffer> m_swapChainFramebuffers;
		uint32_t m_queueFamilyIndices[2];
		VkBuffer m_vertexBuffer;
		VkDeviceMemory m_vertexBufferMemory;
		VkBuffer m_indexBuffer;
		VkDeviceMemory m_indexBufferMemory;
		VkDescriptorSetLayout m_descriptorSetLayout;
		TList<VkBuffer> m_uniformBuffers;
		TList<VkDeviceMemory> m_uniformBuffersMemory;
		VkDescriptorPool m_descriptorPool;
		TList<VkDescriptorSet> m_descriptorSets;

		uint32_t m_currentFrame = 0;


	};
}
#endif