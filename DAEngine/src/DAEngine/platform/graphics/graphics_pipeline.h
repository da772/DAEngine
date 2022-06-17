#pragma once
#include "core/containers.h"
#include "platform/graphics/graphics_api.h"

namespace da::platform {

	class CGraphicsPipeline
	{
	public:
		CGraphicsPipeline(CGraphicsApi& graphicsApi, const CString& vertexShaderPath, const CString& fragShaderPath);

		inline virtual void create() {};
		inline virtual void destroy() {};

	protected:
		const CString& m_vertexShaderPath, m_fragShaderPath;
		CGraphicsApi& m_graphicsApi;

	};

}