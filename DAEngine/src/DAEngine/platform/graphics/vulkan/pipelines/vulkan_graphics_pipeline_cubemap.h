#pragma once

#include "core/core.h"
#include "platform/graphics/vulkan/vulkan_graphics_pipeline.h"

#ifdef DA_GRAPHICS_VULKAN

namespace da::platform
{
	class CVulkanGraphicsPipelineCubemap : public CVulkanGraphicsPipeline
	{
	public:
		CVulkanGraphicsPipelineCubemap(da::graphics::CGraphicsApi& pipeline);

	protected:
		std::vector<VkDescriptorSetLayoutBinding> addDescriptorSets() override;
		virtual void updateRasterizer(VkPipelineRasterizationStateCreateInfo& rasterizer) override;
		virtual void updateDepthStencil(VkPipelineDepthStencilStateCreateInfo& depthStencil) override;
	};
}

#endif

