
#include "graphics_pipeline.h"

namespace da
{

	CGraphicsPipeline::CGraphicsPipeline(CGraphicsApi& graphicsApi, const std::string& vertexShaderPath, const std::string& fragShaderPath, FVertexBindingDescription vertexBinding, 
		const std::vector<FVertexInputAttributeDescription>& inputAttribDesc) :
		
		 m_vertexShaderPath(vertexShaderPath)
		,m_fragShaderPath(fragShaderPath)
		,m_graphicsApi(graphicsApi)
		,m_inputAttrbDesc(inputAttribDesc)
		,m_bindingDesc(vertexBinding)
	{

	}



}