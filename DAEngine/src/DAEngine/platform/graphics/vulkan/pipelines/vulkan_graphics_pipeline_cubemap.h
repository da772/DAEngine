#pragma once

#include "daengine/platform/graphics/vulkan/vulkan_graphics_pipeline.h"

#ifdef DA_GRAPHICS_VULKAN

namespace da::platform
{
	class CVulkanGraphicsPipelineCubemap : public CVulkanGraphicsPipeline
	{
	public:
		CVulkanGraphicsPipelineCubemap(da::core::CGraphicsApi& pipeline);

	protected:
		TArray<VkDescriptorSetLayoutBinding> addDescriptorSets() override;
		virtual void updateRasterizer(VkPipelineRasterizationStateCreateInfo& rasterizer) override;
		virtual void updateDepthStencil(VkPipelineDepthStencilStateCreateInfo& depthStencil) override;
	};
}

#endif

