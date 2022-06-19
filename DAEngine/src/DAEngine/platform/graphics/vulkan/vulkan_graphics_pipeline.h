#pragma once

#include "daengine/core/core.h"

#ifdef DA_GRAPHICS_VULKAN

#include "daengine/core/graphics/graphics_api.h"
#include "daengine/core/graphics/graphics_pipeline.h"
#include "daengine/platform/graphics/vulkan/vulkan_graphics_api.h"
#include <vulkan/vulkan.h>

namespace da::platform
{

	class CVulkanGraphicsPipeline : public core::CGraphicsPipeline
	{
	public:
		CVulkanGraphicsPipeline(core::CGraphicsApi& graphicsApi, const CString& vertexShader, const CString& fragShader, core::FVertexBindingDescription vertexBinding,
			const TArray<core::FVertexInputAttributeDescription>& inputAttribDesc);

		virtual void create() override;
		virtual void destroy() override;

		inline VkPipeline& getPipeline()  {
			return m_graphicsPipeline;
		}

		inline VkPipelineLayout& getPipelineLayout()  {
			return m_pipelineLayout;
		}

		inline VkDescriptorSetLayout& getDescriptorSetLayout()  {
			return m_descriptorSetLayout;
		}

	private:
		void createDescriptorSets();
		void createGraphicsPipeline();
		VkShaderModule createShaderModule(const TList<char>& code, VkDevice device);

	private:
		CVulkanGraphicsApi& m_vulkanGraphicsApi;
		VkPipeline m_graphicsPipeline;
		VkPipelineLayout m_pipelineLayout;
		VkDescriptorSetLayout m_descriptorSetLayout;

	};

}

#endif