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

	class CVulkanGraphicsMaterial : public da::core::CMaterial
	{
	public:
		CVulkanGraphicsMaterial(da::core::CGraphicsPipeline& pipeline
			, const CBasicString <da::memory::CGraphicsAllocator>& albedo
			, const CBasicString <da::memory::CGraphicsAllocator>& normal
			, const CBasicString <da::memory::CGraphicsAllocator>& roughness
			, const CBasicString <da::memory::CGraphicsAllocator>& metallic
		);
		virtual void initialize() override;
		virtual void update(int frame) override;
		virtual void shutdown() override;
		void render(VkCommandBuffer& commandBuffer, int frame);

		inline const TArray<VkDescriptorSet, memory::CGraphicsAllocator>& getDescriptorSets() const {return m_descriptorSets;};
		

	private:
		CVulkanGraphicsPipeline& m_vulkanPipeline;
		CVulkanGraphicsApi& m_vulkanApi;
		CVulkanGraphicsTexture2D m_albedo;
		CVulkanGraphicsTexture2D m_normal;
		CVulkanGraphicsTexture2D m_roughness;
		CVulkanGraphicsTexture2D m_metallic;

	private:
		TList<VkBuffer, memory::CGraphicsAllocator> m_uniformBuffers;
		TList<VkDeviceMemory, memory::CGraphicsAllocator> m_uniformBuffersMemory;
		VkDescriptorPool m_descriptorPool;
		TArray<VkDescriptorSet, memory::CGraphicsAllocator> m_descriptorSets;
		void createUniformBuffers();
		void createDescriptorPools();
		void createDesciprtorSets();
	};



}

#endif