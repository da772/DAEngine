#include "dapch.h"
#include "bgfx_graphics_pbr_material.h"
#include "daengine/platform/graphics/bgfx/cluster/bgfx_material.h"
#include "daengine/platform/graphics/bgfx/bgfx_texture2d.h"
#include "daengine/core/graphics/graphics_texture2d.h"


namespace da::platform {

	CBgfxPbrMaterial::CBgfxPbrMaterial(const std::string& albedo, const std::string& normal, const std::string& roughness, const std::string& emissive)
	{
		m_materialData = {};
		if (albedo != "")
			m_materialData.setBaseColorTexture(new da::platform::bgfx::CBgfxTexture2D(albedo));
		if (normal != "")
			m_materialData.setNormalTexture(new da::platform::bgfx::CBgfxTexture2D(normal));
		if (roughness != "")
			m_materialData.setMetallicRoughnessTexture(new da::platform::bgfx::CBgfxTexture2D(roughness));
		if (emissive != "") {
			m_materialData.setEmissiveTexture(new da::platform::bgfx::CBgfxTexture2D(emissive));
			m_materialData.emissiveFactor = { 1.f,1.f,1.f };
		}

	}

	void CBgfxPbrMaterial::initialize()
	{
		
	}

	void CBgfxPbrMaterial::update(int frame)
	{
		
	}

	void CBgfxPbrMaterial::shutdown()
	{
		delete m_albedo;
		delete m_normal;
		delete m_roughness;
		delete m_emissive;
	}
}


