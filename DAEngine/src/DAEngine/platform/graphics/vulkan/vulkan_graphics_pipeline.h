#pragma once

#include "DAEngine/core/core.h"

#ifdef DA_GRAPHICS_VULKAN

#include "DAEngine/core/graphics/graphics_api.h"
#include "DAEngine/core/graphics/graphics_pipeline.h"
#include "DAEngine/platform/graphics/vulkan/vulkan_graphics_api.h"
#include "DAEngine/core/graphics/graphics_renderable.h"
#include <vulkan/vulkan.h>

namespace da::platform
{
	struct FVulkanMeshData
	{
		VkBuffer VertexBuffer;
		VkBuffer IndexBuffer;
		VkDeviceMemory VertexMemory;
		VkDeviceMemory IndexMemory;
		da::core::IRenderable* Renderable;
		da::core::CMaterial* Material;
	};

	class CVulkanGraphicsPipeline : public core::CGraphicsPipeline
	{
	public:
		CVulkanGraphicsPipeline(core::CGraphicsApi& graphicsApi, const CBasicString<memory::CGraphicsAllocator>& vertexShader, const CBasicString<memory::CGraphicsAllocator>& fragShader, 
			core::FVertexBindingDescription vertexBinding,
			const TArray<core::FVertexInputAttributeDescription, memory::CGraphicsAllocator>& inputAttribDesc);

		virtual void create() override;
		virtual void destroy() override;
		virtual void update(int frame) override;
		void clean();

		inline VkPipeline& getPipeline()  {
			return m_graphicsPipeline;
		}

		inline VkPipelineLayout& getPipelineLayout()  {
			return m_pipelineLayout;
		}

		inline VkDescriptorSetLayout& getDescriptorSetLayout()  {
			return m_descriptorSetLayout;
		}

		inline virtual void addRenderable(da::core::IRenderable* renderable, da::core::CMaterial* material) override{
			m_renderables.push(createMeshData(renderable, material));
		}

		inline void removeRenderable(da::core::IRenderable* renderable)
		{
			TEnumerator<FVulkanMeshData> itr = m_renderables.find([renderable](const FVulkanMeshData& data) {return data.Renderable == renderable;});
			if (itr == m_renderables.end())
			{
				return;
			}

			m_renderables.remove(itr);
		}

		virtual void render(VkCommandBuffer& buffer, int frame);

	protected:
		virtual TArray<VkDescriptorSetLayoutBinding> addDescriptorSets();
		virtual void updateRasterizer(VkPipelineRasterizationStateCreateInfo& rasterizer) {};
		virtual void updateDepthStencil(VkPipelineDepthStencilStateCreateInfo& depthStencil) {};

	private:
		void createGraphicsPipeline();
		void createDescriptorSets();
		FVulkanMeshData createMeshData(da::core::IRenderable* renderable, da::core::CMaterial* material) const;
		VkShaderModule createShaderModule(const TList<char, memory::CGraphicsAllocator>& code, VkDevice device);
		TList<FVulkanMeshData, da::memory::CGraphicsAllocator> m_renderables;
		CVulkanGraphicsApi& m_vulkanGraphicsApi;
		VkPipeline m_graphicsPipeline;
		VkPipelineLayout m_pipelineLayout;
		VkDescriptorSetLayout m_descriptorSetLayout;
		bool m_initialized = false;

	};

}

#endif