#pragma once

#include "platform/graphics/vulkan/vulkan_graphics_pipeline.h"
#ifdef DA_GRAPHICS_VULKAN
namespace da
{
	class CVulkanGraphicsPipelinePBR : public CVulkanGraphicsPipeline
	{
	public:
		CVulkanGraphicsPipelinePBR(da::CGraphicsApi& pipeline);

	protected:
		virtual std::vector<VkDescriptorSetLayoutBinding> addDescriptorSets() override;
	};
}
#endif

