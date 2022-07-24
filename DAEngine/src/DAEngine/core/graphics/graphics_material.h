#pragma once
#include "DAEngine/core/graphics/graphics_pipeline.h"
#include "DAEngine/asset/asset.h"

namespace da::core
{
	class CMaterial
	{
	public:
		inline CMaterial(const CString& albedo) {};

	private:
		CGraphicsPipeline m_pipeline;
		CAsset m_albedo;
	};
}