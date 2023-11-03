#pragma once
#include "DAEngine/core/graphics/graphics_material.h"
#include "DAEngine/core/graphics/graphics_pipeline.h"

namespace da::core {

	class CMaterialFactory
	{
	public:
		static CMaterial* CreatePBR(CGraphicsPipeline& pipeline
			, const std::string &albedo = "assets/white.png"
			, const std::string & normal = "assets/white.png"
			, const std::string & roughness = "assets/white.png"
			, const std::string & metallic = "assets/white.png"
			, const std::string & ao = "assets/white.png"
		
		);
		
		static CMaterial* CreateCubeMap(CGraphicsPipeline& pipeline, const std::string& albedo = "assets/black.png");
	};
}