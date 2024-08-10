
#include "vulkan_graphics_material_cubemap.h"
#ifdef DA_GRAPHICS_VULKAN

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <chrono>

namespace da::platform
{
	CVulkanGraphicsMaterialCubeMap::CVulkanGraphicsMaterialCubeMap(da::graphics::CGraphicsPipeline& pipeline
		, const std::string& texture)
		: CVulkanGraphicsMaterial(pipeline)
		, m_texture(CVulkanGraphicsTextureCube(texture, pipeline.getGraphicsApi()))
	{

	}

	void CVulkanGraphicsMaterialCubeMap::initialize()
	{
		m_texture.initialize();
		CVulkanGraphicsMaterial::initialize();
	}

	void CVulkanGraphicsMaterialCubeMap::shutdown()
	{
		m_texture.shutdown();
		CVulkanGraphicsMaterial::shutdown();
	}

	void CVulkanGraphicsMaterialCubeMap::update(int frame)
	{
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
		UniformBufferObject ubo{};
		/*ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(0, 0, 0));
		ubo.view = glm::translate(glm::mat4(1.0f), glm::vec3(CamPosition.x, CamPosition.y, CamPosition.z))
			* glm::rotate(glm::mat4(1.0f), glm::radians(CamRot.x), glm::vec3(1.f,0.f,0.f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(CamRot.y), glm::vec3(0.f, 1.f, 0.f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(CamRot.z), glm::vec3(0.f, 0.f, 1.f));*/
		ubo.proj = glm::perspective(glm::radians(45.0f), m_vulkanApi.getSwapChainExt().width / (float)m_vulkanApi.getSwapChainExt().height, 0.1f, 100.f);

		ubo.proj[1][1] *= -1;

		ubo.model = ubo.view;
		ubo.model[3] = glm::vec4(0.f, 0.f, 0.f, 1.f);
		void* data;
		vkMapMemory(m_vulkanApi.getDevice(), m_uniformBuffersMemory[frame], 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(m_vulkanApi.getDevice(), m_uniformBuffersMemory[frame]);
	}

	std::vector<VkDescriptorPoolSize> CVulkanGraphicsMaterialCubeMap::getDescriptorPools()
	{
		std::vector<VkDescriptorPoolSize> poolSizes(2);
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = static_cast<uint32_t>(m_vulkanApi.MAX_FRAMES_IN_FLIGHT);
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[1].descriptorCount = static_cast<uint32_t>(m_vulkanApi.MAX_FRAMES_IN_FLIGHT);
		return poolSizes;
	}

	void CVulkanGraphicsMaterialCubeMap::getDescriptorSet(int i)
	{
		VkDescriptorBufferInfo bufferInfo{};
		bufferInfo.buffer = m_uniformBuffers[i];
		bufferInfo.offset = 0;
		bufferInfo.range = sizeof(UniformBufferObject);

		VkDescriptorImageInfo textureImageInfo{};
		textureImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
		textureImageInfo.imageView = m_texture.getTextureImageView();
		textureImageInfo.sampler = m_texture.getTextureImageSampler();

		std::vector<VkWriteDescriptorSet> descriptorWrites(2);
		// Buffers
		descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[0].dstSet = m_descriptorSets[i];
		descriptorWrites[0].dstBinding = 0;
		descriptorWrites[0].dstArrayElement= 0;
		descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		descriptorWrites[0].descriptorCount = 1;
		descriptorWrites[0].pBufferInfo = &bufferInfo;
		descriptorWrites[0].pNext = NULL;

		// image
		descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
		descriptorWrites[1].dstSet = m_descriptorSets[i];
		descriptorWrites[1].dstBinding = 1;
		descriptorWrites[1].dstArrayElement = 0;
		descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		descriptorWrites[1].descriptorCount = 1;
		descriptorWrites[1].pImageInfo = &textureImageInfo;
		descriptorWrites[1].pNext = NULL;

		vkUpdateDescriptorSets(m_vulkanApi.getDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}

}

#endif