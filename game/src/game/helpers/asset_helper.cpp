#include "asset_helper.h"
#include "dacommon.h"
#include "game/generated/file_map_generator.h"


da::CStaticMesh* CAssetHelper::create(Model model)
{
	return da::CStaticMeshFactory::create(CFileMapGenerator::get_filepath(CHashString((uint64_t)model)), false);
}

da::CGraphicsTexture2D* CAssetHelper::create(Texture texture)
{
	return da::CTexture2DFactory::Create(CFileMapGenerator::get_filepath(CHashString((uint64_t)texture)));
}

da::CSkeletalMesh* CAssetHelper::create(Skeleton skeleton)
{
	return da::CSkeletalMeshFactory::create(CFileMapGenerator::get_filepath(CHashString((uint64_t)skeleton)), true);
}

void CAssetHelper::set_material(Material material, da::CSkeletalMesh* mesh)
{
	set_material_internal(material, [mesh](uint32_t i) { return &mesh->getMaterial(i); });
}

void CAssetHelper::set_material(Material material, da::CStaticMesh* mesh)
{
	set_material_internal(material, [mesh](uint32_t i) { return& mesh->getMaterial(i); });
}

void CAssetHelper::set_material_internal(Material materialEnum, std::function<da::FMaterialData* (uint32_t)> getMaterial)
{
	std::fstream in;
	in.open(CFileMapGenerator::get_filepath(CHashString((uint64_t)materialEnum)).c_str(), std::ios::in | std::ios::binary);

	size_t materialCount = 0;

	in.read((char*)&materialCount, sizeof(size_t));

	for (uint32_t i = 0; i < materialCount; i++)
	{
		da::FMaterialData& material = *getMaterial(i);
		in.read((char*)&material.baseColorFactor, sizeof(material.baseColorFactor));
		in.read((char*)&material.emissiveFactor, sizeof(material.emissiveFactor));
		in.read((char*)&material.metallicFactor, sizeof(material.metallicFactor));
		in.read((char*)&material.doubleSided, sizeof(material.doubleSided));
		in.read((char*)&material.blend, sizeof(material.blend));
		in.read((char*)&material.normalScale, sizeof(material.normalScale));
		in.read((char*)&material.occlusionStrength, sizeof(material.occlusionStrength));
		in.read((char*)&material.roughnessFactor, sizeof(material.roughnessFactor));
		in.read((char*)&material.uvScale, sizeof(material.uvScale));


		uint64_t baseColorHash = 0;
		in.read((char*)&baseColorHash, sizeof(uint64_t));
		if (baseColorHash)
		{
			material.m_baseColorTexture = create((Texture)baseColorHash);
		}

		baseColorHash = 0;
		in.read((char*)&baseColorHash, sizeof(uint64_t));
		if (baseColorHash)
		{
			material.m_occlusionTexture = create((Texture)baseColorHash);
		}

		baseColorHash = 0;
		in.read((char*)&baseColorHash, sizeof(uint64_t));
		if (baseColorHash)
		{
			material.m_normalTexture = create((Texture)baseColorHash);
		}

		baseColorHash = 0;
		in.read((char*)&baseColorHash, sizeof(uint64_t));
		if (baseColorHash)
		{
			material.m_metallicRoughnessTexture = create((Texture)baseColorHash);
		}

		baseColorHash = 0;
		in.read((char*)&baseColorHash, sizeof(uint64_t));
		if (baseColorHash)
		{
			material.m_emissiveTexture = create((Texture)baseColorHash);
		}
	}
}

