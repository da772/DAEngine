#include "dapch.h"
#include "graphics_material_data.h"
#include "graphics_texture2d.h"


namespace da::graphics
{

	void FMaterialData::setNormalTexture(CGraphicsTexture2D* texture)
	{
		if (m_normalTexture) delete m_normalTexture;

		m_normalTexture = texture;
	}

	void FMaterialData::setBaseColorTexture(CGraphicsTexture2D* texture)
	{
		if (m_baseColorTexture) {
			
			delete m_baseColorTexture;
		}

		m_baseColorTexture = texture;
	}

	void FMaterialData::setEmissiveTexture(CGraphicsTexture2D* texture)
	{
		if (m_emissiveTexture) delete m_emissiveTexture;

		m_emissiveTexture = texture;
	}

	void FMaterialData::setOcclusionTexture(CGraphicsTexture2D* texture)
	{
		if (m_occlusionTexture) delete m_occlusionTexture;

		m_occlusionTexture = texture;
	}

	void FMaterialData::setMetallicRoughnessTexture(CGraphicsTexture2D* texture)
	{
		if (m_metallicRoughnessTexture) delete m_metallicRoughnessTexture;

		m_metallicRoughnessTexture = texture;
	}

	da::graphics::CGraphicsTexture2D* FMaterialData::getBaseColorTexture() const
	{
		return m_baseColorTexture;
	}

	da::graphics::CGraphicsTexture2D* FMaterialData::getEmissiveTexture() const
	{
		return m_emissiveTexture;
	}

	da::graphics::CGraphicsTexture2D* FMaterialData::getOcclusionTexture() const
	{
		return m_occlusionTexture;
	}

	da::graphics::CGraphicsTexture2D* FMaterialData::getMetallicRoughnessTexture() const
	{
		return m_metallicRoughnessTexture;
	}

	da::graphics::CGraphicsTexture2D* FMaterialData::getNormalTexture() const
	{
		return m_normalTexture;
	}

}