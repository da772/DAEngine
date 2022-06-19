#pragma once
#include "daengine/core/containers.h"
#include "daengine/core/graphics/graphics_api.h"
#include "daengine/core/graphics/graphics_vertex.h"

namespace da::core {

	class CGraphicsPipeline
	{
	public:
		CGraphicsPipeline(CGraphicsApi& graphicsApi, const CString& vertexShaderPath, const CString& fragShaderPath, FVertexBindingDescription vertexBinding, 
			const TArray<FVertexInputAttributeDescription>& inputAttribDesc);

		inline virtual void create() {};
		inline virtual void destroy() {};

	protected:
		CString m_vertexShaderPath, m_fragShaderPath;
		CGraphicsApi& m_graphicsApi;
		FVertexBindingDescription m_bindingDesc;
		TArray<FVertexInputAttributeDescription> m_inputAttrbDesc;

	};

}