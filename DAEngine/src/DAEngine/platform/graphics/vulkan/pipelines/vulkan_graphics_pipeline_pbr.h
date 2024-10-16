#pragma once

#include "daengine/platform/graphics/vulkan/vulkan_graphics_pipeline.h"

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

