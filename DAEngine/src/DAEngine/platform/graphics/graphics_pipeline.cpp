#include "dapch.h"
#include "graphics_pipeline.h"

namespace da::platform
{

	CGraphicsPipeline::CGraphicsPipeline(CGraphicsApi& graphicsApi, const CString& vertexShaderPath, const CString& fragShaderPath) : 
		m_vertexShaderPath(vertexShaderPath)
		,m_fragShaderPath(fragShaderPath)
		,m_graphicsApi(graphicsApi)
	{

	}



}