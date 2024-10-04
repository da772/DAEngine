#pragma once

#include "graphics/graphics_material.h"
#include "platform/graphics/bgfx/bgfx_graphics_material.h"
#include <graphics/graphics_material_data.h>

namespace bgfx {
	struct TextureHandle;
}

namespace da {
	class CBgfxTexture2D;
}

namespace da {

	class CBgfxPbrMaterial 
	{
	public:

		CBgfxPbrMaterial(const std::string& albedo = "", const std::string& normal = "", const std::string& roughness = "", const std::string& emissive = "");

		virtual void initialize();
		virtual void shutdown();

		inline const da::FMaterialData& getMaterial() const { return m_materialData; }
	private:
		da::FMaterialData m_materialData;
		da::CGraphicsTexture2D* m_albedo = nullptr, * m_normal = nullptr, * m_roughness = nullptr, *m_emissive = nullptr;
	};
}