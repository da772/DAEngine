#pragma once

#include "platform/graphics/graphics_pipeline.h"
#include "platform/platform.h"

#ifdef DA_GRAPHICS_VULKAN

#include "platform/graphics/graphics_api.h"
#include "platform/graphics/vulkan/vulkan_graphics_api.h"
#include <vulkan/vulkan.h>

namespace da::platform
{

	class CVulkanGraphicsPipeline : public CGraphicsPipeline
	{
	public:
		CVulkanGraphicsPipeline(CGraphicsApi& graphicsApi, const CString& vertexShader, const CString& fragShader);

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