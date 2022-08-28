#pragma once
#ifdef DA_GRAPHICS_VULKAN
#include "DAEngine/core/graphics/graphics_material.h"
#include "DAEngine/core/graphics/graphics_pipeline.h"
#include "DAEngine/core/containers.h"
#include "DAEngine/platform/graphics/vulkan/vulkan_graphics_pipeline.h"
#include "DAEngine/platform/graphics/vulkan/vulkan_graphics_texture2d.h"
#include <glm/glm.hpp>

namespace da::platform
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

	class CVulkanGraphicsMaterial : public da::core::CMaterial
	{
	public:
		CVulkanGraphicsMaterial(da::core::CGraphicsPipeline& pipeline);
		virtual void initialize() override;
		virtual void update(int frame) override;
		virtual void shutdown() override;
		void render(VkCommandBuffer& commandBuffer, int frame);

		inline const TArray<VkDescriptorSet, memory::CGraphicsAllocator>& getDescriptorSets() const {return m_descriptorSets;};
		
	protected:
		virtual TArray<VkDescriptorPoolSize, memory::CGraphicsAllocator> getDescriptorPools();
		virtual void getDescriptorSet(int frame);

		CVulkanGraphicsPipeline& m_vulkanPipeline;
		CVulkanGraphicsApi& m_vulkanApi;
		TList<VkBuffer, memory::CGraphicsAllocator> m_uniformBuffers;
		TList<VkBuffer, memory::CGraphicsAllocator> m_lightUniformBuffers;
		TArray<VkDescriptorSet, memory::CGraphicsAllocator> m_descriptorSets;
		TList<VkDeviceMemory, memory::CGraphicsAllocator> m_uniformBuffersMemory;
		TList<VkDeviceMemory, memory::CGraphicsAllocator> m_lightBuffersMemory;

	private:
		VkDescriptorPool m_descriptorPool;
		void createUniformBuffers();
		void createLightUniformBuffers();
		void createDescriptorPools();
		void createDescriptorSets();
	};



}

#endif