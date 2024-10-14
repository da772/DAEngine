#pragma once

#define DA_REVIEW

#include <string>
#include <unordered_map>
#include <thread>
#include <glm/glm.hpp>
#include "stl/hashstring.h"
#include "texture_loader.h"
#include <assimp/Importer.hpp>
#include <graphics/graphics_vertex.h>
#include <graphics/graphics_smesh.h>
#include <graphics/graphics_material_data.h>


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

	CHashString m_hash;
	CHashString getHash();

	static std::unordered_map<CHashString, FAssetData> ms_materialSaved;
	static bool hasSaved(FMaterial& mat);
};

class CModelLoader
{
public:
	CModelLoader(const std::string& path, const std::string& targetPath, const std::string& dir, const std::string& name);

	bool loadModel();
	bool saveModel();

	static std::unordered_map<CHashString, FAssetData> ms_modelSaved;
	static std::mutex ms_mutex;

private:
	std::string m_path, m_targetPath, m_dir, m_materialTargetPath, m_textureTargetPath, m_modelTargetPath, m_name;
	std::vector<da::FMesh> m_meshes = {};
	std::vector<FMaterial> m_materials = {};
	Assimp::Importer importer;
};