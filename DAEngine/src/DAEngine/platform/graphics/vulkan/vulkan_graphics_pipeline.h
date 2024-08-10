#pragma once

#include "core.h"

#ifdef DA_GRAPHICS_VULKAN

#include "graphics/graphics_api.h"
#include "graphics/graphics_pipeline.h"
#include "platform/graphics/vulkan/vulkan_graphics_api.h"
#include "graphics/graphics_renderable.h"
#include <vulkan/vulkan.h>

namespace da::platform
{
	struct FVulkanMeshData
	{
		VkBuffer VertexBuffer;
		VkBuffer IndexBuffer;
		VkDeviceMemory VertexMemory;
		VkDeviceMemory IndexMemory;
		da::graphics::IRenderable* Renderable;
		da::graphics::CMaterial* Material;
	};

	class CVulkanGraphicsPipeline : public graphics::CGraphicsPipeline
	{
	public:
		CVulkanGraphicsPipeline(graphics::CGraphicsApi& graphicsApi, const std::string& vertexShader, const std::string& fragShader, 
			graphics::FVertexBindingDescription vertexBinding,
			const std::vector<graphics::FVertexInputAttributeDescription>& inputAttribDesc);

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

		inline virtual void addRenderable(da::graphics::IRenderable* renderable, da::graphics::CMaterial* material) override{
			m_renderables.push_back(createMeshData(renderable, material));
		}

		inline void removeRenderable(da::graphics::IRenderable* renderable)
		{
			const auto& it = std::find_if(m_renderables.begin(), m_renderables.end(), ([renderable](const FVulkanMeshData& data) {return data.Renderable == renderable; }));
			if (it == m_renderables.end())
			{
				return;
			}

			m_renderables.erase(it);
		}

		virtual void render(VkCommandBuffer& buffer, int frame);

	protected:
		virtual std::vector<VkDescriptorSetLayoutBinding> addDescriptorSets();
		virtual void updateRasterizer(VkPipelineRasterizationStateCreateInfo& rasterizer) {};
		virtual void updateDepthStencil(VkPipelineDepthStencilStateCreateInfo& depthStencil) {};

	private:
		void createGraphicsPipeline();
		void createDescriptorSets();
		FVulkanMeshData createMeshData(da::graphics::IRenderable* renderable, da::graphics::CMaterial* material) const;
		VkShaderModule createShaderModule(const std::vector<char>& code, VkDevice device);
		std::vector<FVulkanMeshData> m_renderables;
		CVulkanGraphicsApi& m_vulkanGraphicsApi;
		VkPipeline m_graphicsPipeline;
		VkPipelineLayout m_pipelineLayout;
		VkDescriptorSetLayout m_descriptorSetLayout;
		bool m_initialized = false;

	};

}

#endif