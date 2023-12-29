#include "dapch.h"
#include "bgfx_graphics_pbr_material.h"
#include "daengine/platform/graphics/bgfx/cluster/bgfx_material.h"
#include "daengine/platform/graphics/bgfx/bgfx_texture2d.h"
#include "daengine/core/graphics/graphics_texture2d.h"


namespace da::platform {

	CBgfxPbrMaterial::CBgfxPbrMaterial(const std::string& vsShader, const std::string& fsShader, const std::string& albedo, const std::string& normal, const std::string& roughness)
	{
		m_albedo = new da::platform::bgfx::CBgfxTexture2D(albedo);
		m_normal = new da::platform::bgfx::CBgfxTexture2D(normal);
		m_roughness = new da::platform::bgfx::CBgfxTexture2D(roughness);
		m_material = new CBgfxMaterial();
	}


	void CBgfxPbrMaterial::initialize()
	{
		m_material->baseColorTexture = { m_albedo->getHandle() };
		m_material->doubleSided = true;
		m_material->normalTexture = { m_normal->getHandle() };
		m_material->metallicRoughnessTexture = { m_roughness->getHandle() };
	}

	void CBgfxPbrMaterial::update(int frame)
	{
		
	}

	void CBgfxPbrMaterial::shutdown()
	{
		delete m_albedo;
		delete m_normal;
		delete m_roughness;
		delete m_material;
	}

}


