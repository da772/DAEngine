#include "model_loader.h"
#include "../log.hpp"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <assimp/pbrmaterial.h>
#include <queue>
#include <vector>
#include <fstream>
#include <stl/guid.h>


std::map<CHashString, FAssetData> FMaterial::ms_materialSaved;
std::map <CHashString, FAssetData> CModelLoader::ms_modelSaved;
std::map<CHashString, FAssetData> CModelLoader::ms_skeleSaved;
std::map<CHashString, FAssetData> CModelLoader::ms_AnimSaved;

std::mutex CModelLoader::ms_mutex;




CModelLoader::CModelLoader(const std::string& path, const std::string& targetPath, const std::string& dir, const std::string& name)
	: m_path(path), m_dir(dir), m_name(name), m_targetPath(targetPath)
{
	size_t it = dir.find("assets");
	m_materialTargetPath = dir;
	m_materialTargetPath.insert(it + sizeof("assets"), "materials\\");
	std::filesystem::create_directories(m_materialTargetPath);

	m_animTargetPath = dir;
	m_animTargetPath.insert(it + sizeof("assets"), "anim\\");
	std::filesystem::create_directories(m_animTargetPath);

	m_textureTargetPath = dir;
	m_textureTargetPath.insert(it + sizeof("assets"), "textures\\");
}

bool CModelLoader::loadModel()
{

	importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);
	// Settings for aiProcess_SplitLargeMeshes
	// Limit vertices to 65k (we use 16-bit indices)
	importer.SetPropertyInteger(AI_CONFIG_PP_SLM_VERTEX_LIMIT, std::numeric_limits<uint16_t>::max());

	const aiScene* pScene = importer.ReadFile(m_path.c_str(),
		aiProcess_Triangulate
		| aiProcess_GenSmoothNormals
		| aiProcess_CalcTangentSpace
		| aiProcess_FixInfacingNormals
		| aiProcess_PreTransformVertices
		| aiProcess_TransformUVCoords
		| aiProcess_FlipWindingOrder
		| aiProcess_FlipUVs
	);

	if (pScene->HasAnimations() || pScene->mNumMeshes && pScene->mMeshes[0]->HasBones())
	{
		return loadSkeleton(pScene);
	}

	return loadStatic(pScene);
}

bool CModelLoader::loadSkeleton(const aiScene* pScene)
{
	m_skmeshes.reserve(pScene->mNumMeshes);
	std::queue<aiNode*> q;
	q.push(pScene->mRootNode);
	size_t count = 0;
	while (!q.empty()) {

		aiNode* node = q.front();
		q.pop();

		for (size_t i = 0; i < node->mNumMeshes; i++)
		{
			std::vector<da::FSkeletalVertexBase> vertices;
			std::vector<uint32_t> indices;

			aiMesh* mesh = pScene->mMeshes[node->mMeshes[i]];

			glm::mat4 parentTransform = glm::mat4(1.f);//node->mParent ? AssimpGLMHelpers::ConvertMatrixToGLMFormat(node->mParent->mTransformation) : glm::mat4(1.f);
			glm::mat4 transform = glm::mat4(1.f); //AssimpGLMHelpers::ConvertMatrixToGLMFormat(node->mTransformation) * parentTransform;

			for (size_t v = 0; v < mesh->mNumVertices; v++) {

				da::FSkeletalVertexBase vertex{};
				glm::vec3 pos = transform * glm::vec4(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z, 1.f);
				vertex.Pos = {
					pos.x,
					pos.y,
					pos.z
				};

				if (mesh->HasTextureCoords(0))
				{
					vertex.TexCoord = {
						mesh->mTextureCoords[0][v].x,
						mesh->mTextureCoords[0][v].y
					};
				}

				if (mesh->HasNormals())
				{
					glm::vec3 normals = transform * glm::vec4(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z, 0.f);//glm ::normalize(transform * glm::vec4(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z, 1.f));
					vertex.Normal = {
						normals.x,
						normals.y,
						normals.z
					};
				}

				if (mesh->HasTangentsAndBitangents())
				{
					glm::vec3 tangents = transform * glm::vec4(mesh->mTangents[v].x, mesh->mTangents[v].y, mesh->mTangents[v].z, 0.f); //glm ::normalize(transform * glm::vec4(mesh->mTangents[v].x, mesh->mTangents[v].y, mesh->mTangents[v].z, 1.f));
					vertex.Tangent = {
						tangents.x,
						tangents.y,
						tangents.z,
					};
				}

				for (int z = 0; z < MAX_BONE_INFLUENCE; z++)
				{
					vertex.m_BoneIDs[z] = -1;
					vertex.m_Weights[z] = 0.0f;
				}

				vertices.push_back(vertex);
			}

			for (size_t j = 0; j < mesh->mNumFaces; j++) {
				for (size_t m = 0; m < mesh->mFaces[j].mNumIndices; m++) {
					indices.push_back(mesh->mFaces[j].mIndices[m]);
				}

			}

			for (size_t b = 0; b < mesh->mNumBones; b++) {
				int boneId = -1;
				CHashString name = CHashString(mesh->mBones[b]->mName.C_Str(), mesh->mBones[b]->mName.length);

				const std::unordered_map<CHashString, da::FBoneInfo>::iterator& it = m_boneMap.find(name);

				if (it != m_boneMap.end()) {
					boneId = it->second.id;
				}
				else {
					boneId = m_boneCount++;
					da::FBoneInfo boneInfo;
					boneInfo.id = boneId;
					boneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(mesh->mBones[b]->mOffsetMatrix);
					m_boneMap[name] = std::move(boneInfo);
				}

				uint32_t numWeights = mesh->mBones[b]->mNumWeights;
				aiVertexWeight* weights = mesh->mBones[b]->mWeights;

				for (uint32_t w = 0; w < numWeights; w++) {
					int vertexId = weights[w].mVertexId;
					float weight = weights[w].mWeight;

					setVertexBoneData(vertices[vertexId], boneId, weight);
				}
			}

			m_skmeshes.push_back({ std::move(vertices), std::move(indices), mesh->mMaterialIndex });

		}
		for (size_t i = 0; i < node->mNumChildren; i++) {
			q.push(node->mChildren[i]);
		}
	}

	m_materials.reserve(pScene->mNumMaterials);

	for (size_t i = 0; i < pScene->mNumMaterials; i++) {

		aiMaterial* material = pScene->mMaterials[i];

		FMaterial out;

		// technically there is a difference between MASK and BLEND mode
		// but for our purposes it's enough if we sort properly
		aiString alphaMode;
		material->Get(AI_MATKEY_GLTF_ALPHAMODE, alphaMode);
		aiString alphaModeOpaque;
		alphaModeOpaque.Set("OPAQUE");
		out.blend = alphaMode != alphaModeOpaque;

		material->Get(AI_MATKEY_TWOSIDED, out.doubleSided);

		// texture files

		aiString fileBaseColor, fileMetallicRoughness, fileNormals, fileOcclusion, fileEmissive;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &fileBaseColor);

		// TODO AI_MATKEY_METALLIC_TEXTURE + AI_MATKEY_ROUGHNESS_TEXTURE
		material->GetTexture(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLICROUGHNESS_TEXTURE, &fileMetallicRoughness);
		material->GetTexture(aiTextureType_NORMALS, 0, &fileNormals);
		// TODO aiTextureType_AMBIENT_OCCLUSION, what's the difference?
		material->GetTexture(aiTextureType_LIGHTMAP, 0, &fileOcclusion);
		material->GetTexture(aiTextureType_EMISSIVE, 0, &fileEmissive);

		// diffuse
		if (fileBaseColor.length > 0)
		{
			if (const aiTexture* texture = pScene->GetEmbeddedTexture(fileBaseColor.C_Str()))
			{
				out.m_baseColorTexture = CTextureLoader(m_path, m_name + "_Alb_" + std::to_string(i), m_textureTargetPath, (uint32_t)texture->mWidth, (uint32_t)texture->mHeight, (uint8_t*)texture->pcData);
			}
		}

		aiColor4D baseColorFactor;
		if (material->Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_FACTOR, baseColorFactor) == aiReturn::aiReturn_SUCCESS)
			out.baseColorFactor = { baseColorFactor.r, baseColorFactor.g, baseColorFactor.b, baseColorFactor.a };

		out.baseColorFactor = glm::clamp(out.baseColorFactor, 0.0f, 1.0f);

		// metallic/roughness
		if (fileMetallicRoughness.length > 0)
		{
			if (const aiTexture* texture = pScene->GetEmbeddedTexture(fileMetallicRoughness.C_Str()))
			{
				out.m_metallicRoughnessTexture = CTextureLoader(m_path, m_name + "_Mtl_" + std::to_string(i), m_textureTargetPath, (uint32_t)texture->mWidth, (uint32_t)texture->mHeight, (uint8_t*)texture->pcData);
			}
		}

		ai_real metallicFactor;
		if (AI_SUCCESS == material->Get(AI_MATKEY_SHININESS, metallicFactor))
			out.metallicFactor = glm::clamp(metallicFactor, 0.0f, 1.0f);
		ai_real roughnessFactor;
		if (AI_SUCCESS == material->Get(AI_MATKEY_REFLECTIVITY, roughnessFactor))
			out.roughnessFactor = glm::clamp(roughnessFactor, 0.0f, 1.0f);

		// normal map
		if (fileNormals.length > 0) {
			if (const aiTexture* texture = pScene->GetEmbeddedTexture(fileNormals.C_Str()))
			{
				out.m_normalTexture = CTextureLoader(m_path, m_name + "_Nrm_" + std::to_string(i), m_textureTargetPath, (uint32_t)texture->mWidth, (uint32_t)texture->mHeight, (uint8_t*)texture->pcData);
			}
		}

		ai_real normalScale;
		if (AI_SUCCESS == material->Get(AI_MATKEY_GLTF_TEXTURE_SCALE(aiTextureType_NORMALS, 0), normalScale))
			out.normalScale = normalScale;

		// occlusion texture

		if (fileOcclusion == fileMetallicRoughness)
		{
			// some GLTF files combine metallic/roughness and occlusion values into one texture
			// don't load it twice
			out.m_occlusionTexture = out.m_metallicRoughnessTexture;
		}
		else if (fileOcclusion.length > 0) {
			if (const aiTexture* texture = pScene->GetEmbeddedTexture(fileOcclusion.C_Str()))
			{
				out.m_occlusionTexture = CTextureLoader(m_path, m_name + "_Occ_" + std::to_string(i), m_textureTargetPath, (uint32_t)texture->mWidth, (uint32_t)texture->mHeight, (uint8_t*)texture->pcData);
			}
		}

		ai_real occlusionStrength;
		if (AI_SUCCESS == material->Get(AI_MATKEY_GLTF_TEXTURE_STRENGTH(aiTextureType_LIGHTMAP, 0), occlusionStrength))
			out.occlusionStrength = glm::clamp(occlusionStrength, 0.0f, 1.0f);

		// emissive texture
		if (const aiTexture* texture = pScene->GetEmbeddedTexture(fileEmissive.C_Str()))
		{
			out.m_emissiveTexture = CTextureLoader(m_path, m_name + "_Ems_" + std::to_string(i), m_textureTargetPath, (uint32_t)texture->mWidth, (uint32_t)texture->mHeight, (uint8_t*)texture->pcData);
		}

		aiColor3D emissiveFactor;
		if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveFactor))
			out.emissiveFactor = { emissiveFactor.r, emissiveFactor.g, emissiveFactor.b };
		out.emissiveFactor = glm::clamp(out.emissiveFactor, 0.0f, 1.0f);

		m_materials.push_back(out);
	}

	if (pScene->HasAnimations())
	{
		m_animData.HasAnim = true;
		aiAnimation* anim = pScene->mAnimations[0];
		m_animData.Duration = anim->mDuration;
		m_animData.TicksPerSecond = anim->mTicksPerSecond;

		ReadHeirarchyData(m_animData.Node, pScene->mRootNode);
		ReadMissingBones(anim);
	}

	return true;
}

bool CModelLoader::loadStatic(const aiScene* pScene)
{
	m_meshes.reserve(pScene->mNumMeshes);
	std::queue<aiNode*> q;
	q.push(pScene->mRootNode);
	size_t count = 0;
	while (!q.empty()) {

		aiNode* node = q.front();
		q.pop();

		for (size_t i = 0; i < node->mNumMeshes; i++)
		{
			std::vector<da::FVertexBase> vertices;
			std::vector<uint32_t> indices;

			aiMesh* mesh = pScene->mMeshes[node->mMeshes[i]];

			glm::mat4 parentTransform = glm::mat4(1.f);//node->mParent ? AssimpGLMHelpers::ConvertMatrixToGLMFormat(node->mParent->mTransformation) : glm::mat4(1.f);
			glm::mat4 transform = glm::mat4(1.f); //AssimpGLMHelpers::ConvertMatrixToGLMFormat(node->mTransformation) * parentTransform;

			for (size_t v = 0; v < mesh->mNumVertices; v++) {

				da::FVertexBase vertex{};
				glm::vec3 pos = transform * glm::vec4(mesh->mVertices[v].x, mesh->mVertices[v].y, mesh->mVertices[v].z, 1.f);
				vertex.Pos = {
					pos.x,
					pos.y,
					pos.z
				};

				if (mesh->HasTextureCoords(0))
				{
					vertex.TexCoord = {
						mesh->mTextureCoords[0][v].x,
						mesh->mTextureCoords[0][v].y
					};
				}

				if (mesh->HasNormals())
				{
					glm::vec3 normals = transform * glm::vec4(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z, 0.f);//glm ::normalize(transform * glm::vec4(mesh->mNormals[v].x, mesh->mNormals[v].y, mesh->mNormals[v].z, 1.f));
					vertex.Normal = {
						normals.x,
						normals.y,
						normals.z
					};
				}

				if (mesh->HasTangentsAndBitangents())
				{
					glm::vec3 tangents = transform * glm::vec4(mesh->mTangents[v].x, mesh->mTangents[v].y, mesh->mTangents[v].z, 0.f); //glm ::normalize(transform * glm::vec4(mesh->mTangents[v].x, mesh->mTangents[v].y, mesh->mTangents[v].z, 1.f));
					vertex.Tangent = {
						tangents.x,
						tangents.y,
						tangents.z,
					};
				}

				vertices.push_back(vertex);
			}

			for (size_t j = 0; j < mesh->mNumFaces; j++) {
				for (size_t m = 0; m < mesh->mFaces[j].mNumIndices; m++) {
					indices.push_back(mesh->mFaces[j].mIndices[m]);
				}

			}

			m_meshes.push_back({ std::move(vertices), std::move(indices), mesh->mMaterialIndex });

		}
		for (size_t i = 0; i < node->mNumChildren; i++) {
			q.push(node->mChildren[i]);
		}
	}

	m_materials.reserve(pScene->mNumMaterials);

	for (size_t i = 0; i < pScene->mNumMaterials; i++) {

		aiMaterial* material = pScene->mMaterials[i];

		FMaterial out;

		// technically there is a difference between MASK and BLEND mode
		// but for our purposes it's enough if we sort properly
		aiString alphaMode;
		material->Get(AI_MATKEY_GLTF_ALPHAMODE, alphaMode);
		aiString alphaModeOpaque;
		alphaModeOpaque.Set("OPAQUE");
		out.blend = alphaMode != alphaModeOpaque;

		material->Get(AI_MATKEY_TWOSIDED, out.doubleSided);

		// texture files

		aiString fileBaseColor, fileMetallicRoughness, fileNormals, fileOcclusion, fileEmissive;
		material->GetTexture(aiTextureType_DIFFUSE, 0, &fileBaseColor);

		// TODO AI_MATKEY_METALLIC_TEXTURE + AI_MATKEY_ROUGHNESS_TEXTURE
		material->GetTexture(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLICROUGHNESS_TEXTURE, &fileMetallicRoughness);
		material->GetTexture(aiTextureType_NORMALS, 0, &fileNormals);
		// TODO aiTextureType_AMBIENT_OCCLUSION, what's the difference?
		material->GetTexture(aiTextureType_LIGHTMAP, 0, &fileOcclusion);
		material->GetTexture(aiTextureType_EMISSIVE, 0, &fileEmissive);

		// diffuse
		if (fileBaseColor.length > 0)
		{
			if (const aiTexture* texture = pScene->GetEmbeddedTexture(fileBaseColor.C_Str()))
			{
				out.m_baseColorTexture = CTextureLoader(m_path, m_name + "_Alb_" + std::to_string(i), m_textureTargetPath, (uint32_t)texture->mWidth, (uint32_t)texture->mHeight, (uint8_t*)texture->pcData);
			}
		}

		aiColor4D baseColorFactor;
		if (material->Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_FACTOR, baseColorFactor) == aiReturn::aiReturn_SUCCESS)
			out.baseColorFactor = { baseColorFactor.r, baseColorFactor.g, baseColorFactor.b, baseColorFactor.a };

		out.baseColorFactor = glm::clamp(out.baseColorFactor, 0.0f, 1.0f);

		// metallic/roughness
		if (fileMetallicRoughness.length > 0)
		{
			if (const aiTexture* texture = pScene->GetEmbeddedTexture(fileMetallicRoughness.C_Str()))
			{
				out.m_metallicRoughnessTexture = CTextureLoader(m_path, m_name + "_Mtl_" + std::to_string(i), m_textureTargetPath, (uint32_t)texture->mWidth, (uint32_t)texture->mHeight, (uint8_t*)texture->pcData);
			}
		}

		ai_real metallicFactor;
		if (AI_SUCCESS == material->Get(AI_MATKEY_SHININESS, metallicFactor))
			out.metallicFactor = glm::clamp(metallicFactor, 0.0f, 1.0f);
		ai_real roughnessFactor;
		if (AI_SUCCESS == material->Get(AI_MATKEY_REFLECTIVITY, roughnessFactor))
			out.roughnessFactor = glm::clamp(roughnessFactor, 0.0f, 1.0f);

		// normal map
		if (fileNormals.length > 0) {
			if (const aiTexture* texture = pScene->GetEmbeddedTexture(fileNormals.C_Str()))
			{
				out.m_normalTexture = CTextureLoader(m_path, m_name + "_Nrm_" + std::to_string(i), m_textureTargetPath, (uint32_t)texture->mWidth, (uint32_t)texture->mHeight, (uint8_t*)texture->pcData);
			}
		}

		ai_real normalScale;
		if (AI_SUCCESS == material->Get(AI_MATKEY_GLTF_TEXTURE_SCALE(aiTextureType_NORMALS, 0), normalScale))
			out.normalScale = normalScale;

		// occlusion texture

		if (fileOcclusion == fileMetallicRoughness)
		{
			// some GLTF files combine metallic/roughness and occlusion values into one texture
			// don't load it twice
			out.m_occlusionTexture = out.m_metallicRoughnessTexture;
		}
		else if (fileOcclusion.length > 0) {
			if (const aiTexture* texture = pScene->GetEmbeddedTexture(fileOcclusion.C_Str()))
			{
				out.m_occlusionTexture = CTextureLoader(m_path, m_name + "_Occ_" + std::to_string(i), m_textureTargetPath, (uint32_t)texture->mWidth, (uint32_t)texture->mHeight, (uint8_t*)texture->pcData);
			}
		}

		ai_real occlusionStrength;
		if (AI_SUCCESS == material->Get(AI_MATKEY_GLTF_TEXTURE_STRENGTH(aiTextureType_LIGHTMAP, 0), occlusionStrength))
			out.occlusionStrength = glm::clamp(occlusionStrength, 0.0f, 1.0f);

		// emissive texture
		if (const aiTexture* texture = pScene->GetEmbeddedTexture(fileEmissive.C_Str()))
		{
			out.m_emissiveTexture = CTextureLoader(m_path, m_name + "_Ems_" + std::to_string(i), m_textureTargetPath, (uint32_t)texture->mWidth, (uint32_t)texture->mHeight, (uint8_t*)texture->pcData);
		}

		aiColor3D emissiveFactor;
		if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveFactor))
			out.emissiveFactor = { emissiveFactor.r, emissiveFactor.g, emissiveFactor.b };
		out.emissiveFactor = glm::clamp(out.emissiveFactor, 0.0f, 1.0f);

		m_materials.push_back(out);
	}

	return true;
}

const da::FBoneInfo& CModelLoader::addBone(CHashString name)
{
	const std::unordered_map<CHashString, da::FBoneInfo>::iterator& it = m_boneMap.find(name);

	if (it != m_boneMap.end())
	{
		return it->second;
	}

	m_boneMap[name] = { m_boneCount++, glm::mat4(1.f) };
	return m_boneMap[name];
}

void CModelLoader::ReadHeirarchyData(da::FAssimpNodeData& dest, const aiNode* src)
{
	dest.name = src->mName.data;
	dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
	dest.childrenCount = src->mNumChildren;

	for (int i = 0; i < src->mNumChildren; i++)
	{
		da::FAssimpNodeData newData;
		ReadHeirarchyData(newData, src->mChildren[i]);
		dest.children.push_back(newData);
	}
}

void CModelLoader::ReadMissingBones(const aiAnimation* animation)
{
	int size = animation->mNumChannels;

	//reading channels(bones engaged in an animation and their keyframes)
	for (int i = 0; i < size; i++)
	{
		auto channel = animation->mChannels[i];
		CHashString boneName = channel->mNodeName.data;

		bool boneMapped = m_BoneInfoMap.find(boneName) != m_BoneInfoMap.end() && false;

		if (!boneMapped)
		{
			m_BoneInfoMap[boneName].id = 0;
			m_BoneInfoMap[boneName].offset = {};
		}

		if (m_Bones.find(boneName) != m_Bones.end())
		{
			continue;
		}

		m_Bones[boneName] = da::CAnimatedBone(channel->mNodeName.data, 0, channel);
	}
}

void CModelLoader::setVertexBoneData(da::FSkeletalVertexBase& vertex, int boneID, float weight) const
{
	for (int i = 0; i < MAX_BONE_INFLUENCE; ++i)
	{
		if (vertex.m_BoneIDs[i] < 0)
		{
			vertex.m_Weights[i] = weight;
			vertex.m_BoneIDs[i] = boneID;
			break;
		}
	}
}

bool CModelLoader::saveModel()
{
	importer.FreeScene();
	
	{
		std::string relOutPath;
		size_t i = m_materialTargetPath.find("assets");
		relOutPath = m_materialTargetPath.substr(i, m_materialTargetPath.size() - i);

		std::string matName = m_name + "_Mat";
		std::string path = (m_materialTargetPath + matName + ".mat");
		std::string hashStr = relOutPath + m_name + "_Mat.mat";

		CHashString hash = CHashString(hashStr.c_str(), hashStr.size());

		FMaterial::ms_materialSaved[HASHSTR(hash.c_str())] = { m_path, matName, hashStr, hash };

		std::ostringstream outStream;
		size_t materialCount = m_materials.size();
		outStream.write((const char*)&materialCount, sizeof(size_t));

		for (uint32_t i = 0; i < m_materials.size(); i++)
		{
			CTextureLoader* textures[] = {
				&m_materials[i].m_baseColorTexture,
				&m_materials[i].m_normalTexture,
				&m_materials[i].m_metallicRoughnessTexture,
				&m_materials[i].m_occlusionTexture,
				&m_materials[i].m_emissiveTexture
			};

			for (uint32_t x = 0; x < sizeof(textures) / sizeof(CTextureLoader*); x++) {

				if (textures[x]->getHash() != 0)
				{
					textures[x]->saveTexture();
				}
			}
			outStream.write((const char*)&m_materials[i].baseColorFactor, sizeof(m_materials[i].baseColorFactor));
			outStream.write((const char*)&m_materials[i].emissiveFactor, sizeof(m_materials[i].emissiveFactor));
			outStream.write((const char*)&m_materials[i].metallicFactor, sizeof(m_materials[i].metallicFactor));
			outStream.write((const char*)&m_materials[i].doubleSided, sizeof(m_materials[i].doubleSided));
			outStream.write((const char*)&m_materials[i].blend, sizeof(m_materials[i].blend));
			outStream.write((const char*)&m_materials[i].normalScale, sizeof(m_materials[i].normalScale));
			outStream.write((const char*)&m_materials[i].occlusionStrength, sizeof(m_materials[i].occlusionStrength));
			outStream.write((const char*)&m_materials[i].roughnessFactor, sizeof(m_materials[i].roughnessFactor));
			outStream.write((const char*)&m_materials[i].uvScale, sizeof(m_materials[i].uvScale));
			

			uint64_t baseColorHash = m_materials[i].m_baseColorTexture.getHash().hash();
			outStream.write((const char*)&baseColorHash, sizeof(uint64_t));

			baseColorHash = m_materials[i].m_occlusionTexture.getHash().hash();
			outStream.write((const char*)&baseColorHash, sizeof(uint64_t));

			baseColorHash = m_materials[i].m_normalTexture.getHash().hash();
			outStream.write((const char*)&baseColorHash, sizeof(uint64_t));

			baseColorHash = m_materials[i].m_metallicRoughnessTexture.getHash().hash();
			outStream.write((const char*)&baseColorHash, sizeof(uint64_t));

			baseColorHash = m_materials[i].m_emissiveTexture.getHash().hash();
			outStream.write((const char*)&baseColorHash, sizeof(uint64_t));
		}

		std::ofstream out;
		out.open(path.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
		out.write(outStream.str().c_str(), outStream.str().size());
		out.close();
	}

	std::string& path = m_targetPath;

	std::string relPath;
	size_t i = m_path.find("assets");
	relPath = m_path.substr(i, relPath.size() - i);

	std::string relOutPath;
	i = path.find("assets");
	relOutPath = path.substr(i, path.size() - i);

	std::ostringstream outStream;

	if (!m_meshes.empty() && !m_animData.HasAnim)
	{
		relOutPath += ".mdel";
		m_targetPath += ".mdel";
		size_t meshCount = m_meshes.size();
		size_t materialCount = m_materials.size();
		outStream.write((const char*)&meshCount, sizeof(size_t));
		outStream.write((const char*)&materialCount, sizeof(size_t));

		for (uint32_t i = 0; i < m_meshes.size(); i++) {

			size_t materialIndex = m_meshes[i].MaterialIndex;
			size_t vertexCount = m_meshes[i].Vertices.size();
			size_t indexCount = m_meshes[i].Indices.size();
			outStream.write((const char*)&materialIndex, sizeof(size_t));
			outStream.write((const char*)&vertexCount, sizeof(size_t));
			outStream.write((const char*)&indexCount, sizeof(size_t));
			outStream.write((const char*)m_meshes[i].Vertices.data(), m_meshes[i].Vertices.size() * sizeof(da::FVertexBase));
			outStream.write((const char*)m_meshes[i].Indices.data(), m_meshes[i].Indices.size() * sizeof(uint32_t));
		}
	}
	else if (!m_skmeshes.empty())
	{
		relOutPath += ".skel";
		m_targetPath += ".skel";
		size_t meshCount = m_skmeshes.size();
		size_t materialCount = m_materials.size();
		outStream.write((const char*)&meshCount, sizeof(size_t));
		outStream.write((const char*)&materialCount, sizeof(size_t));

		for (uint32_t i = 0; i < m_skmeshes.size(); i++) {

			size_t materialIndex = m_skmeshes[i].MaterialIndex;
			size_t vertexCount = m_skmeshes[i].Vertices.size();
			size_t indexCount = m_skmeshes[i].Indices.size();
			outStream.write((const char*)&materialIndex, sizeof(size_t));
			outStream.write((const char*)&vertexCount, sizeof(size_t));
			outStream.write((const char*)&indexCount, sizeof(size_t));
			outStream.write((const char*)m_skmeshes[i].Vertices.data(), m_skmeshes[i].Vertices.size() * sizeof(da::FSkeletalVertexBase));
			outStream.write((const char*)m_skmeshes[i].Indices.data(), m_skmeshes[i].Indices.size() * sizeof(uint32_t));
		}

		outStream.write((const char*)&m_boneCount, sizeof(int));

		size_t boneMapCount = m_boneMap.size();
		outStream.write((const char*)&boneMapCount, sizeof(size_t));

		for (const std::pair<CHashString, da::FBoneInfo>& kv : m_boneMap)
		{
			size_t boneNameSize = strlen(kv.first.c_str());
			outStream.write((const char*)&boneNameSize, sizeof(size_t));
			outStream.write((const char*)kv.first.c_str(), boneNameSize);
			outStream.write((const char*)&kv.second, sizeof(da::FBoneInfo));
		}
	}

	if (m_animData.HasAnim)
	{
		std::string relOutPath;
		size_t i = m_animTargetPath.find("assets");
		relOutPath = m_animTargetPath.substr(i, m_animTargetPath.size() - i);

		std::string matName = m_name;
		std::string apath = (m_animTargetPath + matName + ".anim");
		std::string hashStr = relOutPath + m_name + ".anim";

		CHashString hash = HASHSTR(hashStr.c_str(), hashStr.size());
		ms_AnimSaved[HASHSTR(hash.c_str())] = { m_path, matName, hashStr , hash };

		std::ostringstream animStream;
		
		animStream.write((char*)&m_animData.Duration, sizeof(float));
		animStream.write((char*)&m_animData.TicksPerSecond, sizeof(float));

		std::deque<da::FAssimpNodeData*> queue = { &m_animData.Node };

		while (!queue.empty())
		{
			da::FAssimpNodeData* data = queue.front();
			size_t nameSize = strlen(data->name.c_str());
			queue.erase(queue.begin());
			animStream.write((char*)&data->transformation, sizeof(glm::mat4));
			animStream.write((char*)&nameSize, sizeof(nameSize));
			animStream.write((char*)data->name.c_str(), nameSize);
			animStream.write((char*)&data->childrenCount, sizeof(int));

			for (int i = 0; i < data->childrenCount; i++)
			{
				queue.push_front(&data->children[i]);
			}
			
		}

		uint64_t validityCheck = ANIM_PARSER_VALIDITY_SEC_01;
		animStream.write((char*)&validityCheck, sizeof(uint64_t));

		size_t boneInfoMapSize = m_BoneInfoMap.size();
		animStream.write((char*)&boneInfoMapSize, sizeof(size_t));

		for (const std::pair<CHashString, da::FBoneInfo>& kv : m_BoneInfoMap)
		{
			size_t nameSize = strlen(kv.first.c_str());
			animStream.write((char*)&nameSize, sizeof(nameSize));
			animStream.write(kv.first.c_str(), nameSize);
			animStream.write((char*)&kv.second, sizeof(da::FBoneInfo));
		}
		
		validityCheck = ANIM_PARSER_VALIDITY_SEC_02;
		animStream.write((char*)&validityCheck, sizeof(uint64_t));

		size_t boneSize = m_Bones.size();
		animStream.write((char*)&boneSize, sizeof(size_t));

		for (const std::pair<CHashString, da::CAnimatedBone>& kv : m_Bones)
		{
			size_t nameSize = strlen(kv.first.c_str());
			animStream.write((char*)&nameSize, sizeof(nameSize));
			animStream.write(kv.first.c_str(), nameSize);

			animStream.write((char*)&kv.second.m_ID, sizeof(int));
			animStream.write((char*)&kv.second.m_LocalTransform, sizeof(glm::mat4));

			animStream.write((char*)&kv.second.m_NumPositions, sizeof(int));
			for (int i = 0; i < kv.second.m_NumPositions; i++)
			{
				animStream.write((char*)&kv.second.m_Positions[i], sizeof(da::FKeyPosition));
			}

			animStream.write((char*)&kv.second.m_NumRotations, sizeof(int));
			for (int i = 0; i < kv.second.m_NumRotations; i++)
			{
				animStream.write((char*)&kv.second.m_Rotations[i], sizeof(da::FKeyRotation));
			}

			animStream.write((char*)&kv.second.m_NumScalings, sizeof(int));
			for (int i = 0; i < kv.second.m_NumScalings; i++)
			{
				animStream.write((char*)&kv.second.m_Scales[i], sizeof(da::FKeyScale));
			}
		}

		validityCheck = ANIM_PARSER_VALIDITY_SEC_03;
		animStream.write((char*)&validityCheck, sizeof(uint64_t));

		std::ofstream out;
		out.open(apath.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
		out.write(animStream.str().c_str(), animStream.str().size());
		out.close();
	}

	std::ofstream out;
	out.open(path.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
	out.write(outStream.str().c_str(), outStream.str().size());
	out.close();

	CHashString hash = CHashString(relOutPath.c_str(), relOutPath.size());

	if (m_skmeshes.empty() && !m_animData.HasAnim)
	{
		ms_modelSaved[hash] = { m_path, m_name, relOutPath, hash };
	}
	else
	{
		ms_skeleSaved[hash] = { m_path, m_name, relOutPath, hash };
	}
	
	return true;
}

CHashString FMaterial::getHash()
{
	return m_hash;
}
