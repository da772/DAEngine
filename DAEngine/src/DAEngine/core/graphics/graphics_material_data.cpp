#include "dapch.h"
#include "graphics_material_data.h"
#include "graphics_texture2d.h"


namespace da::graphics
{

	void FMaterialData::setNormalTexture(CGraphicsTexture2D* texture, bool clear)
	{
		if (m_normalTexture && clear) delete m_normalTexture;

		m_normalTexture = texture;
	}

	void FMaterialData::setBaseColorTexture(CGraphicsTexture2D* texture, bool clear)
	{
		if (m_baseColorTexture && clear) {
			
			delete m_baseColorTexture;
		}

		m_baseColorTexture = texture;
	}

	void FMaterialData::setEmissiveTexture(CGraphicsTexture2D* texture, bool clear)
	{
		if (m_emissiveTexture && clear) delete m_emissiveTexture;

		m_emissiveTexture = texture;
	}

	void FMaterialData::setOcclusionTexture(CGraphicsTexture2D* texture, bool clear)
	{
		if (m_occlusionTexture && clear) delete m_occlusionTexture;

		m_occlusionTexture = texture;
	}

	void FMaterialData::setMetallicRoughnessTexture(CGraphicsTexture2D* texture, bool clear)
	{
		if (m_metallicRoughnessTexture && clear) delete m_metallicRoughnessTexture;

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