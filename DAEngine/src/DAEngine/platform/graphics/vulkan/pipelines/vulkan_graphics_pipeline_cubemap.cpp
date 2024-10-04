
#include "vulkan_graphics_pipeline_cubemap.h"

#ifdef DA_GRAPHICS_VULKAN

namespace da
{
	static std::vector<da::FVertexInputAttributeDescription> getAttributeDescriptions();
	static da::FVertexBindingDescription getBindingDescriptions();

	CVulkanGraphicsPipelineCubemap::CVulkanGraphicsPipelineCubemap(da::CGraphicsApi& pipeline) :
		CVulkanGraphicsPipeline(pipeline
			, "shaders/skybox.vert.spv"
			, "shaders/skybox.frag.spv"
			, getBindingDescriptions()
			, getAttributeDescriptions()
		)
	{

	}

	da::FVertexBindingDescription getBindingDescriptions()
	{
		da::FVertexBindingDescription result;
		result.binding = 0;
		result.stride = sizeof(da::FVertexBase);
		result.inputRate = 0;
		return result;
	}

	std::vector<da::FVertexInputAttributeDescription> getAttributeDescriptions()
	{
		std::vector<da::FVertexInputAttributeDescription> attributeDescriptions(1);
		// Position
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = 106; // VK_FORMAT_R32G32B32_SFLOAT = 106,
		attributeDescriptions[0].offset = offsetof(da::FVertexBase, Pos);

		return attributeDescriptions;
	}

	std::vector<VkDescriptorSetLayoutBinding> CVulkanGraphicsPipelineCubemap::addDescriptorSets()
	{
		VkDescriptorSetLayoutBinding uboLayoutBinding = {};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

		VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
		samplerLayoutBinding.binding = 1;
		samplerLayoutBinding.descriptorCount = 1;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		return { uboLayoutBinding, samplerLayoutBinding };
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