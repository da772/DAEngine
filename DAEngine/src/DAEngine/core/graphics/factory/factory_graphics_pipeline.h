#pragma once

#include "DAEngine/core/graphics/graphics_pipeline.h"

namespace da::core {

	class CGraphicsPipelineFactory
	{
	public:
		static CGraphicsPipeline* Create(CGraphicsApi& graphicsApi, const CString& vertexShaderPath, const CString& fragShaderPath, FVertexBindingDescription vertexBinding, const TArray<FVertexInputAttributeDescription>& inputAttribDesc);
	};
}

