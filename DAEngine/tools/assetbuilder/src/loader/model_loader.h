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
#include <graphics/graphics_skmesh.h>
#include <graphics/graphics_material_data.h>
#include <graphics/assimp_conversion_helpers.h>
#include <graphics/skeletal_animation.h>


struct FAnimData
{
	da::FAssimpNodeData Node;
	float Duration;
	float TicksPerSecond;
	bool HasAnim : 1 = false;
};

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
};

class CModelLoader
{
public:
	CModelLoader(const std::string& path, const std::string& targetPath, const std::string& dir, const std::string& name);

	bool loadModel();
	bool saveModel();

	static std::unordered_map<CHashString, FAssetData> ms_modelSaved;
	static std::unordered_map<CHashString, FAssetData> ms_skeleSaved;
	static std::unordered_map<CHashString, FAssetData> ms_AnimSaved;
	
	static std::mutex ms_mutex;

private:
	bool loadSkeleton(const aiScene* pScehe);
	bool loadStatic(const aiScene* pScene);
	const da::FBoneInfo& addBone(CHashString name);
	void ReadHeirarchyData(da::FAssimpNodeData& dest, const aiNode* src);
	void ReadMissingBones(const aiAnimation* animation);
	void setVertexBoneData(da::FSkeletalVertexBase& vertex, int boneID, float weight) const;

private:
	std::string m_path, m_targetPath, m_dir, m_materialTargetPath, m_textureTargetPath, m_animTargetPath, m_modelTargetPath, m_name;
	std::vector<da::FMesh> m_meshes = {};
	std::vector<da::FSkeletalMesh> m_skmeshes = {};
	std::unordered_map<CHashString, da::FBoneInfo> m_boneMap;
	FAnimData m_animData;
	int m_boneCount = 0;
	std::vector<FMaterial> m_materials = {};
	Assimp::Importer importer;
	std::unordered_map<CHashString, da::FBoneInfo> m_BoneInfoMap;
	std::unordered_map<CHashString, da::CAnimatedBone> m_Bones;
};