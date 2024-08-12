#pragma once

#include "graphics/graphics_pipeline.h"

using namespace da::graphics;

namespace da::factory {

	class CGraphicsPipelineFactory
	{
	public:
		static CGraphicsPipeline* Create(CGraphicsApi& graphicsApi, const std::string& vertexShaderPath, const std::string& fragShaderPath, FVertexBindingDescription vertexBinding, const std::vector<FVertexInputAttributeDescription>& inputAttribDesc);

		static CGraphicsPipeline* CreatePBR(CGraphicsApi& graphicsApi);
		static CGraphicsPipeline* CreateCubeMap(CGraphicsApi& graphicsApi);

		static const std::vector<CGraphicsPipeline*>& GetPipelines();
		
	private:
		static std::vector<CGraphicsPipeline*> s_pipelines;
	};
}

