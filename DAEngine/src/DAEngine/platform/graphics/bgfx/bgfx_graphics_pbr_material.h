#pragma once

#include "daengine/core/graphics/graphics_material.h"
#include "daengine/platform/graphics/bgfx/bgfx_graphics_material.h"
#include <daengine/core/graphics/graphics_material_data.h>

namespace bgfx {
	struct TextureHandle;
}

namespace da::platform::bgfx {
	class CBgfxTexture2D;
}

namespace da::platform {

	struct CBgfxMaterial;

	class CBgfxPbrMaterial : public CBgfxGraphicsMaterial
	{
	public:

		CBgfxPbrMaterial(const std::string& albedo = "", const std::string& normal = "", const std::string& roughness = "", const std::string& emissive = "");

		virtual void initialize() override;
		virtual void update(int frame) override;
		virtual void shutdown() override;

		inline const da::graphics::FMaterialData& getMaterial() const { return m_materialData; }
	private:
		da::graphics::FMaterialData m_materialData;
		da::platform::bgfx::CBgfxTexture2D* m_albedo = nullptr, * m_normal = nullptr, * m_roughness = nullptr, *m_emissive = nullptr;
	};
}