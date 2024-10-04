
#include "vulkan_graphics_pipeline_pbr.h"
#ifdef DA_GRAPHICS_VULKAN
namespace da
{
	std::vector<da::FVertexInputAttributeDescription> getAttributeDescription();
	da::FVertexBindingDescription getBindingDescription();

	CVulkanGraphicsPipelinePBR::CVulkanGraphicsPipelinePBR(da::CGraphicsApi& pipeline) :
		CVulkanGraphicsPipeline(pipeline
			, "shaders/pbr_shader.vert.spv"
			, "shaders/pbr_shader.frag.spv"
			, getBindingDescription()
			, getAttributeDescription()
		)
	{

	}

	da::FVertexBindingDescription getBindingDescription()
	{
		da::FVertexBindingDescription result;
		result.binding = 0;
		result.stride = sizeof(da::FVertexBase);
		result.inputRate = 0;
		return result;
	}

	std::vector<da::FVertexInputAttributeDescription> getAttributeDescription()
	{
		std::vector<da::FVertexInputAttributeDescription> attributeDescriptions(6);
		// Position
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = 106; // VK_FORMAT_R32G32B32_SFLOAT = 106,
		attributeDescriptions[0].offset = offsetof(da::FVertexBase, Pos);

		// Color
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = 106; // VK_FORMAT_R32G32B32_SFLOAT = 106
		//attributeDescriptions[1].offset = offsetof(da::FVertexBase, Color);

		// Tex Coord
		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = 106; // VK_FORMAT_R32G32B32_SFLOAT = 106,
		attributeDescriptions[2].offset = offsetof(da::FVertexBase, TexCoord);

		// Normal
		attributeDescriptions[3].binding = 0;
		attributeDescriptions[3].location = 3;
		attributeDescriptions[3].format = 106; // VK_FORMAT_R32G32B32_SFLOAT = 106,
		attributeDescriptions[3].offset = offsetof(da::FVertexBase, Normal);

		// Tangent
		attributeDescriptions[4].binding = 0;
		attributeDescriptions[4].location = 4;
		attributeDescriptions[4].format = 106; // VK_FORMAT_R32G32B32_SFLOAT = 106,
		attributeDescriptions[4].offset = offsetof(da::FVertexBase, Tangent);

		// BitTangent
		attributeDescriptions[5].binding = 0;
		attributeDescriptions[5].location = 5;
		attributeDescriptions[5].format = 106; // VK_FORMAT_R32G32B32_SFLOAT = 106,
		//attributeDescriptions[5].offset = offsetof(da::FVertexBase, BitTangent);

		return attributeDescriptions;
	}

	std::vector<VkDescriptorSetLayoutBinding> CVulkanGraphicsPipelinePBR::addDescriptorSets()
	{
		VkDescriptorSetLayoutBinding uboLayoutBinding = {};
		uboLayoutBinding.binding = 0;
		uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		uboLayoutBinding.descriptorCount = 1;
		uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
		uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

		VkDescriptorSetLayoutBinding lightLayoutBinding = {};
		lightLayoutBinding.binding = 1;
		lightLayoutBinding.descriptorCount = 1;
		lightLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		lightLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;
		lightLayoutBinding.pImmutableSamplers = nullptr; // Optional

		VkDescriptorSetLayoutBinding samplerLayoutBinding = {};
		samplerLayoutBinding.binding = 2;
		samplerLayoutBinding.descriptorCount = 5;
		samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		samplerLayoutBinding.pImmutableSamplers = nullptr;
		samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

		return { uboLayoutBinding, lightLayoutBinding, samplerLayoutBinding };
	}

}
#endif