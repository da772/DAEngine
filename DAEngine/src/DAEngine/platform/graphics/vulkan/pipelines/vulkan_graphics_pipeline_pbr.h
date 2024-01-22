#pragma once

#include "DAEngine/platform/graphics/vulkan/vulkan_graphics_pipeline.h"
#ifdef DA_GRAPHICS_VULKAN
namespace da::platform
{
	class CVulkanGraphicsPipelinePBR : public CVulkanGraphicsPipeline
	{
	public:
		CVulkanGraphicsPipelinePBR(da::graphics::CGraphicsApi& pipeline);

	protected:
		virtual std::vector<VkDescriptorSetLayoutBinding> addDescriptorSets() override;
	};
}
#endif

