#pragma once
#ifdef DA_GRAPHICS_VULKAN
#include "graphics/graphics_material.h"
#include "graphics/graphics_pipeline.h"
#include "dastd.h"
#include "platform/graphics/vulkan/vulkan_graphics_pipeline.h"
#include "platform/graphics/vulkan/vulkan_graphics_texture2d.h"
#include <glm/glm.hpp>

namespace da
{
	struct UniformBufferObject {
		glm::mat4 model;
		glm::mat4 view;
		glm::mat4 proj;
	};

	struct LightData {
		glm::vec4 color;
		glm::vec4 pos;
		glm::vec4 dir;
	};

	struct LightUniformBuffer {
		LightData data[255];
		uint32_t count;
	};

	class CVulkanGraphicsMaterial : public da::CMaterial
	{
	public:
		CVulkanGraphicsMaterial(da::CGraphicsPipeline& pipeline);
		virtual void initialize() override;
		virtual void update(int frame);
		virtual void shutdown() override;
		void render(VkCommandBuffer& commandBuffer, int frame);

		inline const std::vector<VkDescriptorSet>& getDescriptorSets() const {return m_descriptorSets;};
		
	protected:
		virtual std::vector<VkDescriptorPoolSize> getDescriptorPools();
		virtual void getDescriptorSet(int frame);

		CVulkanGraphicsPipeline& m_vulkanPipeline;
		CVulkanGraphicsApi& m_vulkanApi;
		std::vector<VkBuffer> m_uniformBuffers;
		std::vector<VkBuffer> m_lightUniformBuffers;
		std::vector<VkDescriptorSet> m_descriptorSets;
		std::vector<VkDeviceMemory> m_uniformBuffersMemory;
		std::vector<VkDeviceMemory> m_lightBuffersMemory;

	private:
		VkDescriptorPool m_descriptorPool;
		void createUniformBuffers();
		void createLightUniformBuffers();
		void createDescriptorPools();
		void createDescriptorSets();
	};



}

#endif