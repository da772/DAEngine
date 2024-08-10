
#include "vulkan_graphics_material_pbr.h"
#ifdef DA_GRAPHICS_VULKAN

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <chrono>

namespace da::platform
{
	CVulkanGraphicsMaterialPBR::CVulkanGraphicsMaterialPBR(da::graphics::CGraphicsPipeline& pipeline
		, const std::string& albedo
		, const std::string& normal
		, const std::string& roughness
		, const std::string& metallic
		, const std::string& ao)
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

	LightUniformBuffer lbo;

	void CVulkanGraphicsMaterialPBR::update(int frame)
	{
		static auto startTime = std::chrono::high_resolution_clock::now();

		auto currentTime = std::chrono::high_resolution_clock::now();
		float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();
		UniformBufferObject ubo{};
		/*ubo.model = glm::translate(glm::mat4(1.0f), glm::vec3(Position.x, Position.y, Position.z))
			* glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f))
			* glm::rotate(glm::mat4(1.0f), time * glm::radians(RotationSpeed), glm::vec3(0.0f, 1.0f, 0.0f));
		ubo.model = glm::scale(ubo.model, glm::vec3(Scale.x, Scale.y, Scale.z));
		ubo.view = glm::lookAt(glm::vec3(CamPosition.x, CamPosition.y, CamPosition.z), glm::vec3(Position.x, Position.y, Position.z), { 0.f, 0.f, 1.f });*/
			/*glm::translate(glm::mat4(1.0f), glm::vec3(CamPosition.x, CamPosition.y, CamPosition.z))
			* glm::rotate(glm::mat4(1.0f), glm::radians(CamRot.x), glm::vec3(1.f, 0.f, 0.f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(CamRot.y), glm::vec3(0.f, 1.f, 0.f))
			* glm::rotate(glm::mat4(1.0f), glm::radians(CamRot.z), glm::vec3(0.f, 0.f, 1.f));
			*/
		
		ubo.proj = glm::perspective(glm::radians(90.0f), m_vulkanApi.getSwapChainExt().width / (float)m_vulkanApi.getSwapChainExt().height, 0.1f, 1000.0f);

		ubo.proj[1][1] *= -1;

		void* data;
		vkMapMemory(m_vulkanApi.getDevice(), m_uniformBuffersMemory[frame], 0, sizeof(ubo), 0, &data);
		memcpy(data, &ubo, sizeof(ubo));
		vkUnmapMemory(m_vulkanApi.getDevice(), m_uniformBuffersMemory[frame]);
		
		if (lbo.count != 16) {
			//LightUniformBuffer lbo;

			int i = 0;
			for (int x = -2; x < 2; x++) {
				for (int y = -2; y < 2; y++) {
					if (i >= 255) {
						break;
					}
					lbo.data[i].pos = { (float)x,0.15f,(float)y, 10.f };
					lbo.data[i].color = { ((double)rand() / (RAND_MAX)),((double)rand() / (RAND_MAX)),((double)rand() / (RAND_MAX)) , 1.f };
					i++;
				}
			}

			lbo.count = 16;
		}
		

		glm::vec3 eyeScale;
		glm::quat eyeRot;
		glm::vec3 eyePos;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(ubo.view, eyeScale, eyeRot, eyePos, skew, perspective);
		eyeRot = glm::conjugate(eyeRot);

		glm::vec3 euler = glm::eulerAngles(eyeRot) * 3.14159f / 180.f;

		lbo.data[0].pos = { eyePos, 1.f };
		lbo.data[0].color = { 1.f, 0.f, 0.f, 1.f };
		lbo.data[0].dir = { euler, glm::cos(glm::radians(15.f))};
		lbo.count = 16;

		vkMapMemory(m_vulkanApi.getDevice(), m_lightBuffersMemory[frame], 0, sizeof(lbo), 0, &data);
		memcpy(data, &lbo, sizeof(lbo));
		vkUnmapMemory(m_vulkanApi.getDevice(), m_lightBuffersMemory[frame]);
		
	}

	std::vector<VkDescriptorPoolSize> CVulkanGraphicsMaterialPBR::getDescriptorPools()
	{
		std::vector<VkDescriptorPoolSize> poolSizes(3);
		poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[0].descriptorCount = static_cast<uint32_t>(m_vulkanApi.MAX_FRAMES_IN_FLIGHT);
		poolSizes[1].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
		poolSizes[1].descriptorCount = static_cast<uint32_t>(m_vulkanApi.MAX_FRAMES_IN_FLIGHT);
		poolSizes[2].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		poolSizes[2].descriptorCount = static_cast<uint32_t>(m_vulkanApi.MAX_FRAMES_IN_FLIGHT);
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

		std::vector<VkWriteDescriptorSet> descriptorWrites;

		// Buffers
		{
			VkWriteDescriptorSet descriptorWrite = {};
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_descriptorSets[i];
			descriptorWrite.dstBinding = 0;
			descriptorWrite.dstArrayElement= 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pBufferInfo = &bufferInfo;
			descriptorWrite.pNext = NULL;
			descriptorWrites.push_back(descriptorWrite);
		}

		{
			VkWriteDescriptorSet descriptorWrite = {};
			// Lights
			VkDescriptorBufferInfo lightBufferInfo{};
			lightBufferInfo.buffer = m_lightUniformBuffers[i];
			lightBufferInfo.offset = 0;
			lightBufferInfo.range = sizeof(LightUniformBuffer);

			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_descriptorSets[i];
			descriptorWrite.dstBinding = 1;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pBufferInfo = &lightBufferInfo;
			descriptorWrite.pNext = NULL;
			descriptorWrites.push_back(descriptorWrite);
		}
		{
			VkWriteDescriptorSet descriptorWrite = {};
			// Albedo
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_descriptorSets[i];
			descriptorWrite.dstBinding = 2;
			descriptorWrite.dstArrayElement = 0;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pImageInfo = &albedoImageInfo;
			descriptorWrite.pNext = NULL;
			descriptorWrites.push_back(descriptorWrite);
		}
		{
			VkWriteDescriptorSet descriptorWrite = {};
			// Normal
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_descriptorSets[i];
			descriptorWrite.dstBinding = 2;
			descriptorWrite.dstArrayElement = 1;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pImageInfo = &normalImageInfo;
			descriptorWrite.pNext = NULL;
			descriptorWrites.push_back(descriptorWrite);
		}
		{
			VkWriteDescriptorSet descriptorWrite = {};
			// Roughness
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_descriptorSets[i];
			descriptorWrite.dstBinding = 2;
			descriptorWrite.dstArrayElement = 2;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pImageInfo = &roughnessImageInfo;
			descriptorWrite.pNext = NULL;
			descriptorWrites.push_back(descriptorWrite);
		}
		
		{
			VkWriteDescriptorSet descriptorWrite = {};
			// Metallic
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_descriptorSets[i];
			descriptorWrite.dstBinding = 2;
			descriptorWrite.dstArrayElement = 3;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pImageInfo = &metallicImageInfo;
			descriptorWrite.pNext = NULL;
			descriptorWrites.push_back(descriptorWrite);
		}
		{
			VkWriteDescriptorSet descriptorWrite = {};
			// AO
			descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
			descriptorWrite.dstSet = m_descriptorSets[i];
			descriptorWrite.dstBinding = 2;
			descriptorWrite.dstArrayElement = 4;
			descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
			descriptorWrite.descriptorCount = 1;
			descriptorWrite.pImageInfo = &aoImageInfo;
			descriptorWrite.pNext = NULL;
			descriptorWrites.push_back(descriptorWrite);
		}

		vkUpdateDescriptorSets(m_vulkanApi.getDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
	}

}

#endif