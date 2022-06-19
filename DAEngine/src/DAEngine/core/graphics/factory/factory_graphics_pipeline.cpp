#include "dapch.h"
#include "core/core.h"
#include "factory_graphics_pipeline.h"

#ifdef DA_GRAPHICS_VULKAN
#include "platform/graphics/vulkan/vulkan_graphics_pipeline.h"
#endif

namespace da::core {

	da::core::CGraphicsPipeline* CGraphicsPipelineFactory::Create(CGraphicsApi& graphicsApi, const CString& vertexShaderPath, const CString& fragShaderPath, FVertexBindingDescription vertexBinding, const TArray<FVertexInputAttributeDescription>& inputAttribDesc)
	{
#ifdef DA_GRAPHICS_VULKAN
		return new da::platform::CVulkanGraphicsPipeline(graphicsApi, vertexShaderPath, fragShaderPath, vertexBinding, inputAttribDesc);
#endif
		return nullptr;
	}

}