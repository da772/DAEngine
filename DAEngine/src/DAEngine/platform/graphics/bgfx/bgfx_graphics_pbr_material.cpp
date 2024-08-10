
#include "bgfx_graphics_pbr_material.h"
#include "platform/graphics/bgfx/cluster/bgfx_material.h"
#include "graphics/graphics_texture2d.h"
#include "graphics/factory/factory_graphics_texture2d.h"


namespace da::platform {

	CBgfxPbrMaterial::CBgfxPbrMaterial(const std::string& albedo, const std::string& normal, const std::string& roughness, const std::string& emissive)
	{
		m_materialData = {};
		if (albedo != "")
			m_materialData.setBaseColorTexture(da::factory::CTexture2DFactory::Create(albedo));
		if (normal != "")
			m_materialData.setNormalTexture(da::factory::CTexture2DFactory::Create(normal));
		if (roughness != "")
			m_materialData.setMetallicRoughnessTexture(da::factory::CTexture2DFactory::Create(roughness));
		if (emissive != "") {
			m_materialData.setEmissiveTexture(da::factory::CTexture2DFactory::Create(emissive));
			m_materialData.emissiveFactor = { 1.f,1.f,1.f };
		}

	}

	void CBgfxPbrMaterial::initialize()
	{
		
	}

	void CBgfxPbrMaterial::shutdown()
	{
		da::factory::CTexture2DFactory::Remove(m_albedo);
		da::factory::CTexture2DFactory::Remove(m_normal);
		da::factory::CTexture2DFactory::Remove(m_roughness);
		da::factory::CTexture2DFactory::Remove(m_emissive);
	}
}


