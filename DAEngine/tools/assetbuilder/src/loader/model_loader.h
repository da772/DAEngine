#pragma once

#define DA_REVIEW

#include <string>
#include <unordered_map>
#include <glm/glm.hpp>
#include "stl/hashstring.h"
#include "texture_loader.h"
#include <assimp/Importer.hpp>
#include <graphics/graphics_vertex.h>
#include <graphics/graphics_smesh.h>
#include <graphics/graphics_material_data.h>

using namespace da::graphics;


struct FMaterial
{
	bool blend = false;
	bool doubleSided = false;

	glm::vec4 baseColorFactor = { 1.0f, 1.0f, 1.0f, 1.0f };
	glm::vec2 uvScale = { 1.f,1.f };
	float metallicFactor = .5f;
	float roughnessFactor = .5f;
	float normalScale = 1.0f;
	float occlusionStrength = 1.0f;
	glm::vec3 emissiveFactor = { 0.0f, 0.0f, 0.0f };

	CTextureLoader m_baseColorTexture = {};
	CTextureLoader m_emissiveTexture = {};
	CTextureLoader m_occlusionTexture = {};
	CTextureLoader m_normalTexture = {};
	CTextureLoader m_metallicRoughnessTexture = {};

	uint32_t m_hash;
	da::core::CGuid getHash();

	static std::unordered_map<da::core::CGuid, FAssetData> ms_materialSaved;
	static bool hasSaved(FMaterial& mat);
};

class CModelLoader
{
public:
	CModelLoader(const std::string& path, const std::string& targetPath, const std::string& dir, const std::string& name);

	bool loadModel();
	bool saveModel();

private:
	std::string m_path, m_targetPath, m_dir, m_materialTargetPath, m_textureTargetPath, m_modelTargetPath, m_name;
	std::vector<FMesh> m_meshes = {};
	std::vector<FMaterial> m_materials = {};
	Assimp::Importer importer;
};