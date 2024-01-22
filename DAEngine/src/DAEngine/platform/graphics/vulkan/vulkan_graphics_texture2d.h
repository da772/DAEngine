#include "core/core.h"
#include "core/graphics/graphics_texture2d.h"

#ifdef DA_GRAPHICS_VULKAN
#include <vulkan/vulkan.h>
#include "vulkan_graphics_api.h"

namespace da::platform
{
	class CVulkanGraphicsTexture2D : public graphics::CGraphicsTexture2D
	{
	public:
		CVulkanGraphicsTexture2D(const std::string& path, graphics::CGraphicsApi& graphicsApi);
		~CVulkanGraphicsTexture2D();

		void initialize();
		void shutdown();

		inline VkImage& getTextureImage() { return m_textureImage; }
		inline VkImageView& getTextureImageView() { return m_textureImageView; }
		inline VkSampler& getTextureImageSampler() { return m_textureSampler;}

	private:
		void createTexture();
		void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSample, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
		bool hasStencilComponent(VkFormat format);
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		void generateMipMaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
		void createTextureSampler();


	private:
		uint32_t m_mipLevels;
		CVulkanGraphicsApi& m_vulkanGraphicsApi;
		VkImage m_textureImage;
		VkDeviceMemory m_textureImageMemory;
		VkImageView m_textureImageView;
		VkSampler m_textureSampler;

	};
}


#endif