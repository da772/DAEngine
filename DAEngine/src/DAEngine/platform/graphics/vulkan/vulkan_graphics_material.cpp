#include "dapch.h"
#ifdef DA_GRAPHICS_VULKAN
#include "vulkan_graphics_material.h"
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#if DA_DEBUG || DA_RELEASE
#define VK_CHECK(x, y) ASSERT(x == y)
#endif

#if DA_FINAL 
#define VK_CHECK(x, y) x
#endif

namespace da::platform
{
	CVulkanGraphicsMaterial::CVulkanGraphicsMaterial(da::core::CGraphicsPipeline& pipeline
		, const CBasicString <da::memory::CGraphicsAllocator>& albedo
		, const CBasicString <da::memory::CGraphicsAllocator>& normal
		, const CBasicString <da::memory::CGraphicsAllocator>& roughness
		, const CBasicString <da::memory::CGraphicsAllocator>& metallic
	) :
		m_vulkanPipeline(*dynamic_cast<CVulkanGraphicsPipeline*>(&pipeline))
		,m_vulkanApi(*dynamic_cast<CVulkanGraphicsApi*>(&pipeline.getGraphicsApi()))
		,m_albedo(CVulkanGraphicsTexture2D(albedo, pipeline.getGraphicsApi()))
		,m_normal(CVulkanGraphicsTexture2D(normal, pipeline.getGraphicsApi()))
		,m_roughness(CVulkanGraphicsTexture2D(roughness, pipeline.getGraphicsApi()))
		,m_metallic(CVulkanGraphicsTexture2D(metallic, pipeline.getGraphicsApi()))
	{
		
	}

	void CVulkanGraphicsMaterial::initialize()
	{
		m_albedo.initialize();
		m_normal.initialize();
		m_metallic.initialize();
		m_roughness.initialize();
		createUniformBuffers();
		createDescriptorPools();
		createDesciprtorSets();
	}

	void CVulkanGraphicsMaterial::update(int frame)
	{
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
		UniformBufferObject ubo{};
		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(Position.x, Position.y, Position.z)) * glm::rotate(glm::mat4(1.0f), time * glm::radians(20.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.proj = glm::perspective(glm::radians(75.0f), m_vulkanApi.getSwapChainExt().width / (float)m_vulkanApi.getSwapChainExt().height, 0.1f, 10.0f);

		ubo.proj[1][1] *= -1;

		void* data;
		vkMapMemory(m_vulkanApi.getDevice(), m_uniformBuffersMemory[frame], 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(m_vulkanApi.getDevice(), m_uniformBuffersMemory[frame]);
	}

	void CVulkanGraphicsMaterial::shutdown()
	{
		m_albedo.shutdown();
		m_normal.shutdown();
		m_roughness.shutdown();
		m_metallic.shutdown();
		for (size_t i = 0; i < m_vulkanApi.MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroyBuffer(m_vulkanApi.getDevice(), m_uniformBuffers[i], &m_vulkanApi.getAllocCallbacks());
			vkFreeMemory(m_vulkanApi.getDevice(), m_uniformBuffersMemory[i], &m_vulkanApi.getAllocCallbacks());
		}

		vkDestroyDescriptorPool(m_vulkanApi.getDevice(), m_descriptorPool, &m_vulkanApi.getAllocCallbacks());
	}

	void CVulkanGraphicsMaterial::createUniformBuffers()
	{
		VkDeviceSize bufferSize = sizeof(UniformBufferObject);

		m_uniformBuffers.resize(m_vulkanApi.MAX_FRAMES_IN_FLIGHT);
		m_uniformBuffersMemory.resize(m_vulkanApi.MAX_FRAMES_IN_FLIGHT);

		for (size_t i = 0; i < m_vulkanApi.MAX_FRAMES_IN_FLIGHT; i++) {
			m_vulkanApi.createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_uniformBuffers[i], m_uniformBuffersMemory[i]);
		}
	}

	void CVulkanGraphicsMaterial::createDescriptorPools()
	{
		TArray<VkDescriptorPoolSize, memory::CGraphicsAllocator> poolSizes(2);
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = static_cast<uint32_t>(m_vulkanApi.MAX_FRAMES_IN_FLIGHT);
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = static_cast<uint32_t>(m_vulkanApi.MAX_FRAMES_IN_FLIGHT);

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = static_cast<uint32_t>(m_vulkanApi.MAX_FRAMES_IN_FLIGHT);

		auto result = vkCreateDescriptorPool(m_vulkanApi.getDevice(), &poolInfo, &m_vulkanApi.getAllocCallbacks(), &m_descriptorPool);

		VK_CHECK(result, VK_SUCCESS);
	}

	void CVulkanGraphicsMaterial::createDesciprtorSets()
	{
		TArray<VkDescriptorSetLayout, memory::CGraphicsAllocator> layouts(m_vulkanApi.MAX_FRAMES_IN_FLIGHT, m_vulkanPipeline.getDescriptorSetLayout());
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_descriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(m_vulkanApi.MAX_FRAMES_IN_FLIGHT);
		allocInfo.pSetLayouts = layouts.data();

		m_descriptorSets.resize(m_vulkanApi.MAX_FRAMES_IN_FLIGHT);

		auto res = vkAllocateDescriptorSets(m_vulkanApi.getDevice(), &allocInfo, m_descriptorSets.data());

		VK_CHECK(res, VK_SUCCESS);

		for (size_t i = 0; i < m_vulkanApi.MAX_FRAMES_IN_FLIGHT; i++) {
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

			TArray<VkWriteDescriptorSet, memory::CGraphicsAllocator> descriptorWrites(5);
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

			vkUpdateDescriptorSets(m_vulkanApi.getDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
		}

	}

	void CVulkanGraphicsMaterial::render(VkCommandBuffer& commandBuffer, int frame)
	{
		m_vulkanPipeline.render(commandBuffer, frame);
	}

}

#endif