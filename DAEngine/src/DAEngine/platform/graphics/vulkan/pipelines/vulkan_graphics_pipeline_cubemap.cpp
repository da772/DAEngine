#include "dapch.h"
#include "vulkan_graphics_pipeline_cubemap.h"

#ifdef DA_GRAPHICS_VULKAN

namespace da::platform
{
	static TArray<da::core::FVertexInputAttributeDescription> getAttributeDescriptions();
	static da::core::FVertexBindingDescription getBindingDescriptions();

	CVulkanGraphicsPipelineCubemap::CVulkanGraphicsPipelineCubemap(da::core::CGraphicsApi& pipeline) :
		CVulkanGraphicsPipeline(pipeline
			, "shaders/skybox.vert.spv"
			, "shaders/skybox.frag.spv"
			, getBindingDescriptions()
			, getAttributeDescriptions()
		)
	{

	}

	da::core::FVertexBindingDescription getBindingDescriptions()
	{
		da::core::FVertexBindingDescription result;
		result.binding = 0;
		result.stride = sizeof(da::core::FVertexBase);
		result.inputRate = 0;
		return result;
	}

	TArray<da::core::FVertexInputAttributeDescription> getAttributeDescriptions()
	{
		TArray<da::core::FVertexInputAttributeDescription> attributeDescriptions(1);
		// Position
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = 106; // VK_FORMAT_R32G32B32_SFLOAT = 106,
		attributeDescriptions[0].offset = offsetof(da::core::FVertexBase, Pos);

		return attributeDescriptions;
	}

	void CVulkanGraphicsPipelineCubemap::createDescriptorSets(VkDescriptorSetLayoutBinding& uboLayoutBinding, VkDescriptorSetLayoutBinding& samplerLayoutBinding)
	{
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
	}

	void CVulkanGraphicsPipelineCubemap::updateRasterizer(VkPipelineRasterizationStateCreateInfo& rasterizer)
	{
		rasterizer.cullMode = VK_CULL_MODE_FRONT_BIT;
	}

	void CVulkanGraphicsPipelineCubemap::updateDepthStencil(VkPipelineDepthStencilStateCreateInfo& depthStencil)
	{
		depthStencil.depthWriteEnable = VK_FALSE;
		depthStencil.depthTestEnable = VK_FALSE;
		depthStencil.stencilTestEnable = VK_FALSE;
	}

}

#endif