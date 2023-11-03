#pragma once
#include "daengine/core/containers.h"
#include "daengine/core/graphics/graphics_api.h"
#include "daengine/core/graphics/graphics_vertex.h"

namespace da::core {

	class IRenderable;
	class CMaterial;

	class CGraphicsPipeline
	{
	public:
		CGraphicsPipeline(CGraphicsApi& graphicsApi, const std::string& vertexShaderPath, const std::string& fragShaderPath, FVertexBindingDescription vertexBinding, 
			const std::vector<FVertexInputAttributeDescription>& inputAttribDesc);

		inline virtual void create() {};
		inline virtual void destroy() {};
		inline virtual void update(int frame) {};
		inline virtual void addRenderable(da::core::IRenderable* renderable, da::core::CMaterial* material) {};

		inline CGraphicsApi& getGraphicsApi() { return m_graphicsApi; };

	protected:
		std::string m_vertexShaderPath, m_fragShaderPath;
		CGraphicsApi& m_graphicsApi;
		FVertexBindingDescription m_bindingDesc;
		std::vector<FVertexInputAttributeDescription> m_inputAttrbDesc;
	};

}