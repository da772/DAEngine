#pragma once

#include "DAEngine/platform/graphics/vulkan/vulkan_graphics_pipeline.h"
#ifdef DA_GRAPHICS_VULKAN
namespace da::platform
{
	class CVulkanGraphicsPipelinePBR : public CVulkanGraphicsPipeline
	{
	public:
		CVulkanGraphicsPipelinePBR(da::core::CGraphicsApi& pipeline);

	protected:
		virtual TArray<VkDescriptorSetLayoutBinding> addDescriptorSets() override;
	};
}
#endif

