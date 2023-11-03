#include "dapch.h"
#ifdef DA_GRAPHICS_VULKAN
#include "vulkan_graphics_material.h"
#include <chrono>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#if DA_DEBUG || DA_RELEASE
#define VK_CHECK(x, y) ASSERT(x == y)
#endif

#if DA_FINAL 
#define VK_CHECK(x, y) x
#endif

namespace da::platform
{
	CVulkanGraphicsMaterial::CVulkanGraphicsMaterial(da::core::CGraphicsPipeline& pipeline) :
		m_vulkanPipeline(*dynamic_cast<CVulkanGraphicsPipeline*>(&pipeline)),
		m_vulkanApi(*dynamic_cast<CVulkanGraphicsApi*>(&m_vulkanPipeline.getGraphicsApi()))
	{
		
	}

	void CVulkanGraphicsMaterial::initialize()
	{
		createUniformBuffers();
		createLightUniformBuffers();
		createDescriptorPools();
		createDescriptorSets();
	}

	void CVulkanGraphicsMaterial::update(int frame)
	{
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
		UniformBufferObject ubo{};
		ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(Position.x, Position.y, Position.z)) 
			*glm::rotate(glm::mat4(1.0f), glm::radians(90.f), glm::vec3(0.0f, 0.0f, 1.0f)) 
			*glm::rotate(glm::mat4(1.0f), time * glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		ubo.model = glm::scale(ubo.model, glm::vec3(.25f, .25f, .25f));
		ubo.view = glm::lookAt(glm::vec3(0.0f, 1.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.35f), glm::vec3(0.0f, 0.0f, 1.0f));
		ubo.proj = glm::perspective(glm::radians(60.0f), m_vulkanApi.getSwapChainExt().width / (float)m_vulkanApi.getSwapChainExt().height, 0.1f, 10.0f);

		ubo.proj[1][1] *= -1;

		void* data;
		vkMapMemory(m_vulkanApi.getDevice(), m_uniformBuffersMemory[frame], 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(m_vulkanApi.getDevice(), m_uniformBuffersMemory[frame]);
	}

	void CVulkanGraphicsMaterial::shutdown()
	{
		for (size_t i = 0; i < m_vulkanApi.MAX_FRAMES_IN_FLIGHT; i++) {
			vkDestroyBuffer(m_vulkanApi.getDevice(), m_uniformBuffers[i], &m_vulkanApi.getAllocCallbacks());
			vkFreeMemory(m_vulkanApi.getDevice(), m_uniformBuffersMemory[i], &m_vulkanApi.getAllocCallbacks());
			vkDestroyBuffer(m_vulkanApi.getDevice(), m_lightUniformBuffers[i], &m_vulkanApi.getAllocCallbacks());
			vkFreeMemory(m_vulkanApi.getDevice(), m_lightBuffersMemory[i], &m_vulkanApi.getAllocCallbacks());
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


	void CVulkanGraphicsMaterial::createLightUniformBuffers()
	{
		VkPhysicalDeviceProperties properties;
		vkGetPhysicalDeviceProperties(m_vulkanApi.getPhysicalDevice(), &properties);
		VkDeviceSize bufferSize = sizeof(LightUniformBuffer);

		m_lightUniformBuffers.resize(m_vulkanApi.MAX_FRAMES_IN_FLIGHT);
		m_lightBuffersMemory.resize(m_vulkanApi.MAX_FRAMES_IN_FLIGHT);

		for (size_t i = 0; i < m_vulkanApi.MAX_FRAMES_IN_FLIGHT; i++) {
			m_vulkanApi.createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, m_lightUniformBuffers[i], m_lightBuffersMemory[i]);
		}
	}

	std::vector<VkDescriptorPoolSize> CVulkanGraphicsMaterial::getDescriptorPools()
	{
		std::vector<VkDescriptorPoolSize> poolSizes(2);
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = static_cast<uint32_t>(m_vulkanApi.MAX_FRAMES_IN_FLIGHT);
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = static_cast<uint32_t>(m_vulkanApi.MAX_FRAMES_IN_FLIGHT);
		return poolSizes;
	}

	void CVulkanGraphicsMaterial::createDescriptorPools()
	{
		std::vector<VkDescriptorPoolSize> poolSizes = getDescriptorPools();

		VkDescriptorPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
		poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
		poolInfo.pPoolSizes = poolSizes.data();
		poolInfo.maxSets = static_cast<uint32_t>(m_vulkanApi.MAX_FRAMES_IN_FLIGHT);

		auto result = vkCreateDescriptorPool(m_vulkanApi.getDevice(), &poolInfo, &m_vulkanApi.getAllocCallbacks(), &m_descriptorPool);

		VK_CHECK(result, VK_SUCCESS);
	}

	void CVulkanGraphicsMaterial::getDescriptorSet(int i)
	{
		
	}

	void CVulkanGraphicsMaterial::createDescriptorSets()
	{
		std::vector<VkDescriptorSetLayout> layouts(m_vulkanApi.MAX_FRAMES_IN_FLIGHT, m_vulkanPipeline.getDescriptorSetLayout());
		VkDescriptorSetAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
		allocInfo.descriptorPool = m_descriptorPool;
		allocInfo.descriptorSetCount = static_cast<uint32_t>(m_vulkanApi.MAX_FRAMES_IN_FLIGHT);
		allocInfo.pSetLayouts = layouts.data();

		m_descriptorSets.resize(m_vulkanApi.MAX_FRAMES_IN_FLIGHT);

		auto res = vkAllocateDescriptorSets(m_vulkanApi.getDevice(), &allocInfo, m_descriptorSets.data());

		VK_CHECK(res, VK_SUCCESS);

		for (size_t i = 0; i < m_vulkanApi.MAX_FRAMES_IN_FLIGHT; i++) {

			std::vector<VkWriteDescriptorSet> descriptorWrites;
			getDescriptorSet(i);
		}

	}

	void CVulkanGraphicsMaterial::render(VkCommandBuffer& commandBuffer, int frame)
	{
		m_vulkanPipeline.render(commandBuffer, frame);
	}

	

}

#endif