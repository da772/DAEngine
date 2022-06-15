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

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		TList<VkSurfaceFormatKHR> formats;
		TList<VkPresentModeKHR> presentModes;
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


		static TArray<VkVertexInputAttributeDescription> getAttributeDescriptions() {
			TArray<VkVertexInputAttributeDescription> attributeDescriptions(3);

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
        inline const VkCommandPool& getCommandPool() const {return m_commandPool; }
        inline const VkRenderPass& getRenderPass() const {return m_renderPass;}

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
		void createTextureImage();
		void createTextureImageView(); 
		void createTextureSampler();
		void createDepthResources();
		VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t);
		void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSample, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage,
			VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
		void beginRecordingCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex);
		uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void stopRecordingCommandBuffer(VkCommandBuffer commandBuffer);
		void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags peoperties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
		void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
		
		void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);
		void createColorResources();

		VkPhysicalDevice findDevices(const TList<VkPhysicalDevice>& devices);

		QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device, std::optional<VkSurfaceKHR> surface);
		VkFormat findSupportedFormat(const TList<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		VkFormat findDepthFormat();

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
		VkImage m_textureImage;
		VkDeviceMemory m_textureImageMemory;
		VkImageView m_textureImageView;
		VkSampler m_textureSampler;
		VkImage m_depthImage;
		VkDeviceMemory m_depthImageMemory;
		VkImageView m_depthImageView;
		VkSampleCountFlagBits m_msaaSamples = VK_SAMPLE_COUNT_1_BIT;
        uint32_t m_imageIndex;
        
		// Render target
		VkImage m_colorImage;
		VkDeviceMemory m_colorImageMemory;
		VkImageView m_colorImageView;

		uint32_t m_currentFrame = 0;

		uint32_t m_mipLevels = 0;

		TList<Vertex> m_vertices;
		TList<uint32_t> m_indices;


	};
}
#endif
