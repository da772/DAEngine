#pragma once

#include "daengine/core/graphics/graphics_material.h"
#include "daengine/platform/graphics/bgfx/bgfx_graphics_material.h"

namespace da::platform::bgfx {
	class CBgfxTexture2D;
}

namespace da::platform {

	class CBgfxMaterial;

	class CBgfxPbrMaterial : public CBgfxGraphicsMaterial
	{
	public:

		CBgfxPbrMaterial(const std::string& vsShader, const std::string& fsShader, const std::string& albedo, const std::string& normal, const std::string& roughness);

		virtual void initialize() override;
		virtual void update(int frame) override;
		virtual void shutdown() override;

		inline CBgfxMaterial* getMaterial() const { return m_material; }


	private:
		da::platform::bgfx::CBgfxTexture2D * m_albedo, * m_normal, * m_roughness;
		CBgfxMaterial* m_material;
	};
}