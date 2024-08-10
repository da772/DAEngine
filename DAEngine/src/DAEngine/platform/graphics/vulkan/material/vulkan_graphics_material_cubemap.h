#pragma once
#include "platform/graphics/vulkan/vulkan_graphics_material.h"
#ifdef DA_GRAPHICS_VULKAN
#include "platform/graphics/vulkan/vulkan_graphics_texturecube.h"

namespace da::platform
{
	class CVulkanGraphicsMaterialCubeMap: public CVulkanGraphicsMaterial
	{
	public:
		CVulkanGraphicsMaterialCubeMap(da::graphics::CGraphicsPipeline& pipeline
			, const std::string& texture);

		virtual void initialize() override;
		virtual void shutdown() override;
		virtual void update(int frame) override;

		virtual std::vector<VkDescriptorPoolSize> getDescriptorPools() override;
		virtual void getDescriptorSet(int frame) override;

	private:
		CVulkanGraphicsTextureCube m_texture;
	};
}

#endif