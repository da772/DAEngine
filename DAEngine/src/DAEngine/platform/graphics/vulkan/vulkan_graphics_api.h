#pragma once
#include "daengine/core/core.h"

#if defined(DA_GRAPHICS_VULKAN) && defined(DA_WINDOW_GLFW)
#include "daengine/core/graphics/graphics_api.h"
#include <vulkan/vulkan.h>
#include <optional>
#include <memory>
#include "DAEngine/core/memory/global_allocator.h"


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
		TList<VkSurfaceFormatKHR, memory::CGraphicsAllocator> formats;
		TList<VkPresentModeKHR, memory::CGraphicsAllocator> presentModes;
	};


	struct Vertex {
		Vector3f Pos;
		Vector3f Color;
		Vector2f TexCoord;

		static VkVertexInputBindingDescription getBindingDescription() {
			VkVertexInputBindingDescription bindingDescription{};
			bindingDescription.binding = 0;
			bindingDescription.stride = sizeof(Vertex);
			bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

			return bindingDescription;
		}


		static TArray<VkVertexInputAttributeDescription, da::memory::CGraphicsAllocator> getAttributeDescriptions() {
			TArray<VkVertexInputAttributeDescription, da::memory::CGraphicsAllocator> attributeDescriptions(3);

			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[0].offset = offsetof(Vertex, Pos);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = VK_FORMAT_R32G32B32_SFLOAT;
			attributeDescriptions[1].offset = offsetof(Vertex, Color);

			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = VK_FORMAT_R32G32_SFLOAT;
			attributeDescriptions[2].offset = offsetof(Vertex, TexCoord);

			return attributeDescriptions;
		}

		bool operator==(const Vertex& other) const {
			return Pos == other.Pos && Color == other.Color && TexCoord == other.TexCoord;
		}
	};


	class CVulkanGraphicsApi : public core::CGraphicsApi
	{
	public:
		CVulkanGraphicsApi(const core::CWindow& windowModule);
		~CVulkanGraphicsApi();
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
        inline const VkCommandPool& getCommandPool() const {return m_commandPool; }
        inline const VkRenderPass& getRenderPass() const {return m_renderPass;}
		inline const VkExtent2D getSwapChainExt() const { return m_swapChainExtent; }

	private:
		void createInstance();
		bool checkValidationLayerSupport(const TList<const char*, memory::CGraphicsAllocator>& validationLayers);
		TList<const char*, memory::CGraphicsAllocator> getRequiredExtensions();
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
		void createTextureImage();
		void createTextureImageView(); 
		void createTextureSampler();
		void createDepthResources();
		void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSample, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
			VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		void beginRecordingCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
        void stopRecordingCommandBuffer(VkCommandBuffer commandBuffer);
		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		
		void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
		void createColorResources();

		VkPhysicalDevice findDevices(const TList<VkPhysicalDevice, memory::CGraphicsAllocator>& devices);

		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, std::optional<VkSurfaceKHR> surface);
		VkFormat findSupportedFormat(const TList<VkFormat, memory::CGraphicsAllocator>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkFormat findDepthFormat();

		VkShaderModule createShaderModule(const TList<char, memory::CGraphicsAllocator>& code, VkDevice device);

		void generateMipMaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);

		void updateUniformBuffer(uint32_t frame);

		void cleanupSwapChain();
		void recreateSwapChain();

		const int MAX_FRAMES_IN_FLIGHT = 2;

		void loadModel();
    public:
        VkCommandBuffer beginSingleTimeCommands() const;
        void endSingleTimeCommands(VkCommandBuffer commandBuffer) const;
        void immediateSubmit(std::function<void (VkCommandBuffer cmd)>&& func) const;
        VkSampleCountFlagBits getMaxUsableSampleCount() const;
		void submitRenderFunction(std::function<void(VkCommandBuffer cmd)>* func);
		void removeRenderFunction(std::function<void(VkCommandBuffer cmd)>* func);
		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags peoperties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
		VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t);
		VkAllocationCallbacks& getAllocCallbacks() { return m_allocCallbacks; }

	private:
		VkInstance m_instance;
		VkDebugUtilsMessengerEXT m_debugMessenger;
		VkPhysicalDevice m_physicalDevice = VK_NULL_HANDLE;
		VkDevice m_device;
		VkQueue m_graphicsQueue;
		TList<const char*> m_validationLayers;
		VkSurfaceKHR m_surface;
		VkQueue m_presentQueue;
		TList<VkImage, memory::CGraphicsAllocator> m_swapChainImages;
		TList<VkImageView, memory::CGraphicsAllocator> m_swapChainImageViews;
		VkFormat m_swapChainImageFormat;
		VkExtent2D m_swapChainExtent;
		
		VkCommandPool m_commandPool;
		VkSwapchainKHR m_swapChain;
		VkRenderPass m_renderPass;
		TList<VkCommandBuffer, memory::CGraphicsAllocator> m_commandBuffers;
		TList<VkSemaphore, memory::CGraphicsAllocator> m_imageAvailableSemaphores;
		TList<VkSemaphore, memory::CGraphicsAllocator> m_renderFinishedSemaphores;
		TList<VkFence, memory::CGraphicsAllocator> m_inFlightFences;
		TList<VkFramebuffer, memory::CGraphicsAllocator> m_swapChainFramebuffers;
		uint32_t m_queueFamilyIndices[2];
		VkBuffer m_vertexBuffer;
		VkDeviceMemory m_vertexBufferMemory;
		VkBuffer m_indexBuffer;
		VkDeviceMemory m_indexBufferMemory;
		TList<VkBuffer, memory::CGraphicsAllocator> m_uniformBuffers;
		TList<VkDeviceMemory, memory::CGraphicsAllocator> m_uniformBuffersMemory;
		VkDescriptorPool m_descriptorPool;
		TList<VkDescriptorSet, memory::CGraphicsAllocator> m_descriptorSets;

		CVulkanGraphicsTexture2D* m_textureImage;

		VkImage m_depthImage;
		VkDeviceMemory m_depthImageMemory;
		VkImageView m_depthImageView;
		VkSampleCountFlagBits m_msaaSamples = VK_SAMPLE_COUNT_1_BIT;
        uint32_t m_imageIndex;


		CVulkanGraphicsPipeline* m_graphicsPipeline = nullptr;
		/*
		VkPipeline m_graphicsPipeline;
		VkDescriptorSetLayout m_descriptorSetLayout;
		VkPipelineLayout m_pipelineLayout;
		*/
        
		// Render target
		VkImage m_colorImage;
		VkDeviceMemory m_colorImageMemory;
		VkImageView m_colorImageView;

		uint32_t m_currentFrame = 0;

		uint32_t m_mipLevels = 0;

		TList<Vertex, memory::CGraphicsAllocator> m_vertices;
		TList<uint32_t, memory::CGraphicsAllocator> m_indices;

		TList<std::function<void(VkCommandBuffer cmd)>*, memory::CGraphicsAllocator> m_renderFunctions;


		memory::CGraphicsAllocator m_allocator;
		VkAllocationCallbacks m_allocCallbacks;

	};
}
#endif
