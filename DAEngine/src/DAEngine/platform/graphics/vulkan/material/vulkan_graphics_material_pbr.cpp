#include "dapch.h"
#include "vulkan_graphics_material_pbr.h"
#ifdef DA_GRAPHICS_VULKAN

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <chrono>

namespace da::platform
{
	CVulkanGraphicsMaterialPBR::CVulkanGraphicsMaterialPBR(da::core::CGraphicsPipeline& pipeline
		, const CBasicString <da::memory::CGraphicsAllocator>& albedo
		, const CBasicString <da::memory::CGraphicsAllocator>& normal
		, const CBasicString <da::memory::CGraphicsAllocator>& roughness
		, const CBasicString <da::memory::CGraphicsAllocator>& metallic
		, const CBasicString <da::memory::CGraphicsAllocator>& ao)
		: CVulkanGraphicsMaterial(pipeline)
		, m_albedo(CVulkanGraphicsTexture2D(albedo, pipeline.getGraphicsApi()))
		, m_normal(CVulkanGraphicsTexture2D(normal, pipeline.getGraphicsApi()))
		, m_roughness(CVulkanGraphicsTexture2D(roughness, pipeline.getGraphicsApi()))
		, m_metallic(CVulkanGraphicsTexture2D(metallic, pipeline.getGraphicsApi()))
		, m_ao(CVulkanGraphicsTexture2D(ao, pipeline.getGraphicsApi()))
	{

	}

	void CVulkanGraphicsMaterialPBR::initialize()
	{
		m_albedo.initialize();
		m_normal.initialize();
		m_metallic.initialize();
		m_roughness.initialize();
		m_ao.initialize();
		CVulkanGraphicsMaterial::initialize();
	}

	void CVulkanGraphicsMaterialPBR::shutdown()
	{
		m_albedo.shutdown();
		m_normal.shutdown();
		m_roughness.shutdown();
		m_metallic.shutdown();
		m_ao.shutdown();
		CVulkanGraphicsMaterial::shutdown();
	}

	void CVulkanGraphicsMaterialPBR::update(int frame)
	{
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
		UniformBufferObject ubo{};
		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(Position.x, Position.y, Position.z))
			* glm::rotate(glm::mat4(1.0f), glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::rotate(glm::mat4(1.0f), time * glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		ubo.model = glm::scale(ubo.model, glm::vec3(.25f, .25f, .25f));
		ubo.view = glm::lookAt(glm::vec3(0.0f, 1.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.35f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.proj = glm::perspective(glm::radians(60.0f), m_vulkanApi.getSwapChainExt().width / (float)m_vulkanApi.getSwapChainExt().height, 0.1f, 10.0f);

		ubo.proj[1][1] *= -1;

		void* data;
		vkMapMemory(m_vulkanApi.getDevice(), m_uniformBuffersMemory[frame], 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(m_vulkanApi.getDevice(), m_uniformBuffersMemory[frame]);
	}

	da::core::containers::TArray<VkDescriptorPoolSize, da::memory::CGraphicsAllocator> CVulkanGraphicsMaterialPBR::getDescriptorPools()
	{
		TArray<VkDescriptorPoolSize, memory::CGraphicsAllocator> poolSizes(2);
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = static_cast<uint32_t>(m_vulkanApi.MAX_FRAMES_IN_FLIGHT);
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = static_cast<uint32_t>(m_vulkanApi.MAX_FRAMES_IN_FLIGHT);
		return poolSizes;
	}

	void CVulkanGraphicsMaterialPBR::getDescriptorSet(int i)
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = m_uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkDescriptorImageInfo albedoImageInfo{};
		albedoImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		albedoImageInfo.imageView = m_albedo.getTextureImageView();
		albedoImageInfo.sampler = m_albedo.getTextureImageSampler();

		VkDescriptorImageInfo normalImageInfo{};
		normalImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		normalImageInfo.imageView = m_normal.getTextureImageView();
		normalImageInfo.sampler = m_normal.getTextureImageSampler();

		VkDescriptorImageInfo roughnessImageInfo{};
		roughnessImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		roughnessImageInfo.imageView = m_roughness.getTextureImageView();
		roughnessImageInfo.sampler = m_roughness.getTextureImageSampler();

		VkDescriptorImageInfo metallicImageInfo{};
		metallicImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		metallicImageInfo.imageView = m_metallic.getTextureImageView();
		metallicImageInfo.sampler = m_metallic.getTextureImageSampler();

		VkDescriptorImageInfo aoImageInfo{};
		aoImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		aoImageInfo.imageView = m_ao.getTextureImageView();
		aoImageInfo.sampler = m_ao.getTextureImageSampler();

		TArray<VkWriteDescriptorSet, memory::CGraphicsAllocator> descriptorWrites(6);
		// Buffers
		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = m_descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement = 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;
		descriptorWrites[0].pNext = NULL;
		// Albedo
		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = m_descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &albedoImageInfo;
		descriptorWrites[1].pNext = NULL;
		// Normal
		descriptorWrites[2].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[2].dstSet = m_descriptorSets[i];
		descriptorWrites[2].dstBinding = 1;
		descriptorWrites[2].dstArrayElement = 1;
		descriptorWrites[2].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[2].descriptorCount = 1;
		descriptorWrites[2].pImageInfo = &normalImageInfo;
		descriptorWrites[2].pNext = NULL;
		// Roughness
		descriptorWrites[3].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[3].dstSet = m_descriptorSets[i];
		descriptorWrites[3].dstBinding = 1;
		descriptorWrites[3].dstArrayElement = 2;
		descriptorWrites[3].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[3].descriptorCount = 1;
		descriptorWrites[3].pImageInfo = &roughnessImageInfo;
		descriptorWrites[3].pNext = NULL;
		// Metallic
		descriptorWrites[4].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[4].dstSet = m_descriptorSets[i];
		descriptorWrites[4].dstBinding = 1;
		descriptorWrites[4].dstArrayElement = 3;
		descriptorWrites[4].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[4].descriptorCount = 1;
		descriptorWrites[4].pImageInfo = &metallicImageInfo;
		descriptorWrites[4].pNext = NULL;
		// AO
		descriptorWrites[5].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[5].dstSet = m_descriptorSets[i];
		descriptorWrites[5].dstBinding = 1;
		descriptorWrites[5].dstArrayElement = 4;
		descriptorWrites[5].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[5].descriptorCount = 1;
		descriptorWrites[5].pImageInfo = &aoImageInfo;
		descriptorWrites[5].pNext = NULL;

		vkUpdateDescriptorSets(m_vulkanApi.getDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}

}

#endif