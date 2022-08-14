#pragma once
#include "DAEngine/core/graphics/graphics_material.h"
#include "DAEngine/core/graphics/graphics_pipeline.h"

namespace da::core {

	class CMaterialFactory
	{
	public:
		static CMaterial* CreatePBR(CGraphicsPipeline& pipeline
			, const CBasicString <da::memory::CGraphicsAllocator> &albedo = "assets/black.png"
			, const CBasicString <da::memory::CGraphicsAllocator>& normal = "assets/black.png"
			, const CBasicString <da::memory::CGraphicsAllocator>& roughness = "assets/black.png"
			, const CBasicString <da::memory::CGraphicsAllocator>& metallic = "assets/black.png"
			, const CBasicString <da::memory::CGraphicsAllocator>& ao = "assets/black.png"
		
		);
		
		static CMaterial* CreateCubeMap(CGraphicsPipeline& pipeline, const CBasicString <da::memory::CGraphicsAllocator>& albedo = "assets/black.png");
	};
}