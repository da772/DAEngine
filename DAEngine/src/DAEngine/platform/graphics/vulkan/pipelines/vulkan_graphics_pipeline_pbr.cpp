#include "dapch.h"
#include "vulkan_graphics_pipeline_pbr.h"

namespace da::platform
{
	TArray<da::core::FVertexInputAttributeDescription> getAttributeDescription();
	da::core::FVertexBindingDescription getBindingDescription();

	CVulkanGraphicsPipelinePBR::CVulkanGraphicsPipelinePBR(da::core::CGraphicsApi& pipeline) :
		CVulkanGraphicsPipeline(pipeline
			, "shaders/pbr_shader.vert.spv"
			, "shaders/pbr_shader.frag.spv"
			, getBindingDescription()
			, getAttributeDescription()
		)
	{

	}

	da::core::FVertexBindingDescription getBindingDescription()
	{
		da::core::FVertexBindingDescription result;
		result.binding = 0;
		result.stride = sizeof(da::core::FVertexBase);
		result.inputRate = 0;
		return result;
	}

	TArray<da::core::FVertexInputAttributeDescription> getAttributeDescription()
	{
		TArray<da::core::FVertexInputAttributeDescription> attributeDescriptions(6);
		// Position
		attributeDescriptions[0].binding = 0;
		attributeDescriptions[0].location = 0;
		attributeDescriptions[0].format = 106; // VK_FORMAT_R32G32B32_SFLOAT = 106,
		attributeDescriptions[0].offset = offsetof(da::core::FVertexBase, Pos);

		// Color
		attributeDescriptions[1].binding = 0;
		attributeDescriptions[1].location = 1;
		attributeDescriptions[1].format = 106; // VK_FORMAT_R32G32B32_SFLOAT = 106
		attributeDescriptions[1].offset = offsetof(da::core::FVertexBase, Color);

		// Tex Coord
		attributeDescriptions[2].binding = 0;
		attributeDescriptions[2].location = 2;
		attributeDescriptions[2].format = 106; // VK_FORMAT_R32G32B32_SFLOAT = 106,
		attributeDescriptions[2].offset = offsetof(da::core::FVertexBase, TexCoord);

		// Normal
		attributeDescriptions[3].binding = 0;
		attributeDescriptions[3].location = 3;
		attributeDescriptions[3].format = 106; // VK_FORMAT_R32G32B32_SFLOAT = 106,
		attributeDescriptions[3].offset = offsetof(da::core::FVertexBase, Normal);

		// Tangent
		attributeDescriptions[4].binding = 0;
		attributeDescriptions[4].location = 4;
		attributeDescriptions[4].format = 106; // VK_FORMAT_R32G32B32_SFLOAT = 106,
		attributeDescriptions[4].offset = offsetof(da::core::FVertexBase, Tangent);

		// BitTangent
		attributeDescriptions[5].binding = 0;
		attributeDescriptions[5].location = 5;
		attributeDescriptions[5].format = 106; // VK_FORMAT_R32G32B32_SFLOAT = 106,
		attributeDescriptions[5].offset = offsetof(da::core::FVertexBase, BitTangent);

		return attributeDescriptions;
	}

	TArray<VkDescriptorSetLayoutBinding> CVulkanGraphicsPipelinePBR::addDescriptorSets()
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