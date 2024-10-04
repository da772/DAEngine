#pragma once

#include "core.h"
#include "platform/graphics/vulkan/vulkan_graphics_pipeline.h"

#ifdef DA_GRAPHICS_VULKAN

namespace da
{
	class CVulkanGraphicsPipelineCubemap : public CVulkanGraphicsPipeline
	{
	public:
		CVulkanGraphicsPipelineCubemap(da::CGraphicsApi& pipeline);

	protected:
		std::vector<VkDescriptorSetLayoutBinding> addDescriptorSets() override;
		virtual void updateRasterizer(VkPipelineRasterizationStateCreateInfo& rasterizer) override;
		virtual void updateDepthStencil(VkPipelineDepthStencilStateCreateInfo& depthStencil) override;
	};
}

#endif

