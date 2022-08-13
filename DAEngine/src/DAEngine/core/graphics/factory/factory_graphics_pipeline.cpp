#include "dapch.h"
#include "core/core.h"
#include "factory_graphics_pipeline.h"
#include "core/memory/global_allocator.h"

#ifdef DA_GRAPHICS_VULKAN
#include "platform/graphics/vulkan/vulkan_graphics_pipeline.h"
#endif

namespace da::core {

	TList<CGraphicsPipeline*, da::memory::CGraphicsAllocator> da::core::CGraphicsPipelineFactory::s_pipelines;

	da::core::CGraphicsPipeline* CGraphicsPipelineFactory::Create(CGraphicsApi& graphicsApi, const CString& vertexShaderPath, const CString& fragShaderPath, FVertexBindingDescription vertexBinding, const TArray<FVertexInputAttributeDescription>& inputAttribDesc)
	{
#ifdef DA_GRAPHICS_VULKAN
		s_pipelines.push(new da::platform::CVulkanGraphicsPipeline(graphicsApi, vertexShaderPath, fragShaderPath, vertexBinding, inputAttribDesc));
		return s_pipelines[s_pipelines.size()-1];
#endif
		return nullptr;
	}

	const TList<CGraphicsPipeline*, da::memory::CGraphicsAllocator>& CGraphicsPipelineFactory::GetPipelines()
	{
		return s_pipelines;
	}

}