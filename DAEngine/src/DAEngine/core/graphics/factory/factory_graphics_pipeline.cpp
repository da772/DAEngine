#include "dapch.h"
#include "core/core.h"
#include "factory_graphics_pipeline.h"

#ifdef DA_GRAPHICS_VULKAN
#include "platform/graphics/vulkan/vulkan_graphics_pipeline.h"
#include "platform/graphics/vulkan/pipelines/vulkan_graphics_pipeline_pbr.h"
#include "platform/graphics/vulkan/pipelines/vulkan_graphics_pipeline_cubemap.h"
#endif

namespace da::core {

	std::vector<CGraphicsPipeline*> da::core::CGraphicsPipelineFactory::s_pipelines;

	da::core::CGraphicsPipeline* CGraphicsPipelineFactory::Create(CGraphicsApi& graphicsApi, const std::string& vertexShaderPath, const std::string& fragShaderPath, FVertexBindingDescription vertexBinding, const std::vector<FVertexInputAttributeDescription>& inputAttribDesc)
	{
#ifdef DA_GRAPHICS_VULKAN
		s_pipelines.push_back(new da::platform::CVulkanGraphicsPipeline(graphicsApi, vertexShaderPath, fragShaderPath, vertexBinding, inputAttribDesc));
		return s_pipelines[s_pipelines.size()-1];
#endif
		return nullptr;
	}

	const std::vector<CGraphicsPipeline*>& CGraphicsPipelineFactory::GetPipelines()
	{
		return s_pipelines;
	}

	da::core::CGraphicsPipeline* CGraphicsPipelineFactory::CreatePBR(CGraphicsApi& graphicsApi)
	{

#ifdef DA_GRAPHICS_VULKAN
		s_pipelines.push_back(new da::platform::CVulkanGraphicsPipelinePBR(graphicsApi));
		return s_pipelines[s_pipelines.size() - 1];
#endif
		return nullptr;
	}

	da::core::CGraphicsPipeline* CGraphicsPipelineFactory::CreateCubeMap(CGraphicsApi& graphicsApi)
	{
#ifdef DA_GRAPHICS_VULKAN
		s_pipelines.push_back(new da::platform::CVulkanGraphicsPipelineCubemap(graphicsApi));
		return s_pipelines[s_pipelines.size() - 1];
#endif
		return nullptr;
	}

}