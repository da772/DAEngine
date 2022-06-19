#include "dapch.h"
#include "graphics_pipeline.h"

namespace da::core
{

	CGraphicsPipeline::CGraphicsPipeline(CGraphicsApi& graphicsApi, const CString& vertexShaderPath, const CString& fragShaderPath, FVertexBindingDescription vertexBinding, 
		const TArray<FVertexInputAttributeDescription>& inputAttribDesc) :
		
		 m_vertexShaderPath(vertexShaderPath)
		,m_fragShaderPath(fragShaderPath)
		,m_graphicsApi(graphicsApi)
		,m_inputAttrbDesc(inputAttribDesc)
		,m_bindingDesc(vertexBinding)
	{

	}



}