#include "dapch.h"
#include "bgfx_graphics_pbr_material.h"
#include "daengine/platform/graphics/bgfx/cluster/bgfx_material.h"
#include "daengine/core/graphics/graphics_texture2d.h"
#include "DAEngine/core/graphics/factory/factory_graphics_texture2d.h"


namespace da::platform {

	CBgfxPbrMaterial::CBgfxPbrMaterial(const std::string& albedo, const std::string& normal, const std::string& roughness, const std::string& emissive)
	{
		m_materialData = {};
		if (albedo != "")
			m_materialData.setBaseColorTexture(da::graphics::CTexture2DFactory::Create(albedo));
		if (normal != "")
			m_materialData.setNormalTexture(da::graphics::CTexture2DFactory::Create(normal));
		if (roughness != "")
			m_materialData.setMetallicRoughnessTexture(da::graphics::CTexture2DFactory::Create(roughness));
		if (emissive != "") {
			m_materialData.setEmissiveTexture(da::graphics::CTexture2DFactory::Create(emissive));
			m_materialData.emissiveFactor = { 1.f,1.f,1.f };
		}

	}

	void CBgfxPbrMaterial::initialize()
	{
		
	}

	void CBgfxPbrMaterial::shutdown()
	{
		da::graphics::CTexture2DFactory::Remove(m_albedo);
		da::graphics::CTexture2DFactory::Remove(m_normal);
		da::graphics::CTexture2DFactory::Remove(m_roughness);
		da::graphics::CTexture2DFactory::Remove(m_emissive);
	}
}


