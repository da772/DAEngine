
#include "graphics_material_data.h"
#include "graphics_texture2d.h"
#include "graphics/factory/factory_graphics_texture2d.h"

namespace da
{
	using namespace da;

	void FMaterialData::setNormalTexture(CGraphicsTexture2D* texture)
	{
		if (m_normalTexture) CTexture2DFactory::Remove(m_normalTexture);

		m_normalTexture = texture;
	}

	void FMaterialData::setBaseColorTexture(CGraphicsTexture2D* texture)
	{
		if (m_baseColorTexture) {
			
			CTexture2DFactory::Remove(m_baseColorTexture);
		}

		m_baseColorTexture = texture;
	}

	void FMaterialData::setEmissiveTexture(CGraphicsTexture2D* texture)
	{
		if (m_emissiveTexture) CTexture2DFactory::Remove(m_emissiveTexture);

		m_emissiveTexture = texture;
	}

	void FMaterialData::setOcclusionTexture(CGraphicsTexture2D* texture)
	{
		if (m_occlusionTexture) CTexture2DFactory::Remove(m_occlusionTexture);

		m_occlusionTexture = texture;
	}

	void FMaterialData::setMetallicRoughnessTexture(CGraphicsTexture2D* texture)
	{
		if (m_metallicRoughnessTexture) CTexture2DFactory::Remove(m_metallicRoughnessTexture);

		m_metallicRoughnessTexture = texture;
	}

	da::CGraphicsTexture2D* FMaterialData::getBaseColorTexture() const
	{
		return m_baseColorTexture;
	}

	da::CGraphicsTexture2D* FMaterialData::getEmissiveTexture() const
	{
		return m_emissiveTexture;
	}

	da::CGraphicsTexture2D* FMaterialData::getOcclusionTexture() const
	{
		return m_occlusionTexture;
	}

	da::CGraphicsTexture2D* FMaterialData::getMetallicRoughnessTexture() const
	{
		return m_metallicRoughnessTexture;
	}

	da::CGraphicsTexture2D* FMaterialData::getNormalTexture() const
	{
		return m_normalTexture;
	}

	void FMaterialData::cleanup()
	{
		if (m_baseColorTexture) CTexture2DFactory::Remove(m_baseColorTexture);
		if (m_emissiveTexture)  CTexture2DFactory::Remove(m_emissiveTexture);
		if (m_occlusionTexture) CTexture2DFactory::Remove(m_occlusionTexture);
		if (m_normalTexture)    CTexture2DFactory::Remove(m_normalTexture);
		if (m_metallicRoughnessTexture) CTexture2DFactory::Remove(m_metallicRoughnessTexture);

		m_baseColorTexture = nullptr;
		m_emissiveTexture = nullptr;
		m_occlusionTexture = nullptr;
		m_normalTexture = nullptr;
		m_metallicRoughnessTexture = nullptr;
	}

}