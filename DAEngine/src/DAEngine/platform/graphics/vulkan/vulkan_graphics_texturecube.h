#pragma once
#ifdef DA_GRAPHICS_VULKAN
#include "graphics/graphics_texturecube.h"
#include "vulkan_graphics_api.h"
#include <ktx.h>

namespace da {
	class CVulkanGraphicsTextureCube : public da::CGraphicsTextureCube
	{
	public:
		CVulkanGraphicsTextureCube(const std::string& path, da::CGraphicsApi& graphicsApi);
		void initialize();
		void shutdown();

		inline VkImage& getTextureImage() { return m_textureImage; }
		inline VkImageView& getTextureImageView() { return m_textureImageView; }
		inline VkSampler& getTextureImageSampler() { return m_textureSampler; }

	private:
		void createTexture();
		void createTextureSampler();
		void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSample, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
		void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
		void transitionImageLayout(VkCommandBuffer buffer, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
		void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
		void generateMipMaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
		bool hasStencilComponent(VkFormat format);
		void setImageLayout(
			VkCommandBuffer cmdbuffer,
			VkImage image,
			VkImageLayout oldImageLayout,
			VkImageLayout newImageLayout,
			VkImageSubresourceRange subresourceRange,
			VkPipelineStageFlags srcStageMask,
			VkPipelineStageFlags dstStageMask);

	private:
		CVulkanGraphicsApi& m_vulkanGraphicsApi;
		uint32_t m_mipLevels;
		VkImage m_textureImage;
		VkDeviceMemory m_textureImageMemory;
		VkImageView m_textureImageView;
		VkSampler m_textureSampler;
		ktxTexture* m_ktxTexture;

	};
}
#endif
