#include "dapch.h"
#include "asset/asset.h"
#include "graphics_skmesh.h"
#include "factory/factory_graphics_texture2d.h"

#if !defined(DA_TEST)
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <assimp/pbrmaterial.h>
#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.hpp>
#include "assimp_conversion_helpers.h"
#endif


namespace da::graphics
{
	CSkeletalMesh::CSkeletalMesh(const std::string& path, bool inverseNormals) : m_path(path)
	{
		CAsset file(path.c_str());
#if !defined(DA_TEST)
		Assimp::Importer importer;

		importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_LINE | aiPrimitiveType_POINT);
		// Settings for aiProcess_SplitLargeMeshes
		// Limit vertices to 65k (we use 16-bit indices)
		importer.SetPropertyInteger(AI_CONFIG_PP_SLM_VERTEX_LIMIT, std::numeric_limits<uint16_t>::max());

		const aiScene* pScene = importer.ReadFileFromMemory(file.data(), file.size() * sizeof(char),
			aiProcess_Triangulate
			| aiProcess_GenSmoothNormals
			| aiProcess_ConvertToLeftHanded
			| aiProcess_CalcTangentSpace
			| aiProcess_FixInfacingNormals
			| aiProcess_PreTransformVertices
			| aiProcess_TransformUVCoords
		);

		static glm::mat4 transformMat = glm::mat4(1.f);//glm::toMat4(glm::quat(glm::radians(glm::vec3(90.f))));

		m_meshes = {};
		m_meshes.reserve(pScene->mNumMeshes);

		std::queue<aiNode*> q;
		q.push(pScene->mRootNode);

		for (size_t i = 0; i < pScene->mNumMeshes; i++)
		{
			std::vector<FSkeletalVertexBase> vertices;
			std::vector<uint32_t> indices;
			FSkeletalMesh mesh = {};

			for (size_t v = 0; v < pScene->mMeshes[i]->mNumVertices; v++) {
				FSkeletalVertexBase vertex{};
				glm::vec3 pos = transformMat * glm::vec4(pScene->mMeshes[i]->mVertices[v].x, pScene->mMeshes[i]->mVertices[v].y, pScene->mMeshes[i]->mVertices[v].z, 1.f);
				vertex.Pos = {
					pos.x,
					pos.y,
					pos.z
				};

				if (pScene->mMeshes[i]->HasTextureCoords(0))
				{
					vertex.TexCoord = {
						pScene->mMeshes[i]->mTextureCoords[0][v].x,
						pScene->mMeshes[i]->mTextureCoords[0][v].y
					};
				}

				if (pScene->mMeshes[i]->HasNormals())
				{
					glm::vec3 normals = glm::normalize(transformMat * glm::vec4(pScene->mMeshes[i]->mNormals[v].x, pScene->mMeshes[i]->mNormals[v].y, pScene->mMeshes[i]->mNormals[v].z, 1.f));
					if (inverseNormals) {
						normals *= -1.f;
					}
					vertex.Normal = {
						normals.x,
						normals.y,
						normals.z
					};
				}

				if (pScene->mMeshes[i]->HasTangentsAndBitangents())
				{
					glm::vec3 tangents = glm::normalize(transformMat * glm::vec4(pScene->mMeshes[i]->mTangents[v].x, pScene->mMeshes[i]->mTangents[v].y, pScene->mMeshes[i]->mTangents[v].z, 1.f));
					if (inverseNormals) {
						tangents *= -1.f;
					}
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

			for (size_t j = 0; j < pScene->mMeshes[i]->mNumFaces; j++) {
				for (size_t m = 0; m < pScene->mMeshes[i]->mFaces[j].mNumIndices; m++) {
					indices.push_back(pScene->mMeshes[i]->mFaces[j].mIndices[m]);
				}

			}

			for (size_t b = 0; b < pScene->mMeshes[i]->mNumBones; b++) {
				int boneId = -1;
				CHashString name = CHashString(pScene->mMeshes[i]->mBones[b]->mName.C_Str(), pScene->mMeshes[i]->mBones[b]->mName.length);

				const std::unordered_map<CHashString, FBoneInfo>::iterator& it = mesh.BoneMap.find(name);

				if (it != mesh.BoneMap.end()) {
					boneId = it->second.id;
				}
				else {
					boneId = mesh.BoneCounter++;
					FBoneInfo boneInfo;
					boneInfo.id = boneId;
					boneInfo.offset = AssimpGLMHelpers::ConvertMatrixToGLMFormat(pScene->mMeshes[i]->mBones[b]->mOffsetMatrix);
					mesh.BoneMap[name] = std::move(boneInfo);
				}

				uint32_t numWeights = pScene->mMeshes[i]->mBones[b]->mNumWeights;
				aiVertexWeight* weights = pScene->mMeshes[i]->mBones[b]->mWeights;

				for (uint32_t w = 0; w < numWeights; w++) {
					int vertexId = weights[w].mVertexId;
					float weight = weights[w].mWeight;

					setVertexBoneData(vertices[vertexId], boneId, weight);
				}


			}

			mesh.Vertices = std::move(vertices);
			mesh.Indices = std::move(indices);
			mesh.MaterialIndex = pScene->mMeshes[i]->mMaterialIndex;

			m_meshes.push_back(mesh);
		}

		m_materials = {};
		m_materials.reserve(pScene->mNumMaterials);

		for (size_t i = 0; i < pScene->mNumMaterials; i++) {

			aiMaterial* material = pScene->mMaterials[i];

			da::graphics::FMaterialData out;

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

			if (CAsset::exists(fileBaseColor.C_Str()))
			{
				out.setBaseColorTexture(da::graphics::CTexture2DFactory::Create(fileBaseColor.C_Str()));
			}
			else if (fileBaseColor.length > 0)
			{
				if (const aiTexture* texture = pScene->GetEmbeddedTexture(fileBaseColor.C_Str()))
				{
					out.setBaseColorTexture(CTexture2DFactory::Create(fileBaseColor.C_Str(), texture->mWidth, texture->mHeight, (char*)texture->pcData));
				}
				else {
					LOG_INFO(ELogChannel::Graphics, "Failed to find albedo texture at: %s for mesh %s", fileBaseColor.C_Str(), m_path);
				}
			}

			aiColor4D baseColorFactor;
			if (material->Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_BASE_COLOR_FACTOR, baseColorFactor) == aiReturn::aiReturn_SUCCESS)
				out.baseColorFactor = { baseColorFactor.r, baseColorFactor.g, baseColorFactor.b, baseColorFactor.a };
			out.baseColorFactor = glm::clamp(out.baseColorFactor, 0.0f, 1.0f);

			// metallic/roughness
			if (CAsset::exists(fileMetallicRoughness.C_Str()))
			{
				out.setMetallicRoughnessTexture(da::graphics::CTexture2DFactory::Create(fileMetallicRoughness.C_Str()));
			}
			else if (fileMetallicRoughness.length > 0)
			{
				if (const aiTexture* texture = pScene->GetEmbeddedTexture(fileMetallicRoughness.C_Str()))
				{
					out.setMetallicRoughnessTexture(CTexture2DFactory::Create(fileMetallicRoughness.C_Str(), texture->mWidth, texture->mHeight, (char*)texture->pcData));
				}
				else {
					LOG_INFO(ELogChannel::Graphics, "Failed to find metallic roughness texture at: %s for mesh %s", fileMetallicRoughness.C_Str(), m_path);
				}
				
			}

			ai_real metallicFactor;
			if (AI_SUCCESS == material->Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLIC_FACTOR, metallicFactor))
				out.metallicFactor = glm::clamp(metallicFactor, 0.0f, 1.0f);
			ai_real roughnessFactor;
			if (AI_SUCCESS == material->Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_ROUGHNESS_FACTOR, roughnessFactor))
				out.roughnessFactor = glm::clamp(roughnessFactor, 0.0f, 1.0f);

			// normal map

			if (CAsset::exists(fileNormals.C_Str()))
			{
				out.setNormalTexture(da::graphics::CTexture2DFactory::Create(fileNormals.C_Str()));
			}
			else if (fileNormals.length > 0) {
				if (const aiTexture* texture = pScene->GetEmbeddedTexture(fileNormals.C_Str()))
				{
					out.setNormalTexture(CTexture2DFactory::Create(fileNormals.C_Str(), texture->mWidth, texture->mHeight, (char*)texture->pcData));
				}
				else {
					LOG_INFO(ELogChannel::Graphics, "Failed to find normal texture at: %s for mesh %s", fileNormals.C_Str(), m_path);
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
				out.setOcclusionTexture(out.getMetallicRoughnessTexture());
			}
			else if (CAsset::exists(fileOcclusion.C_Str()))
			{
				out.setOcclusionTexture(da::graphics::CTexture2DFactory::Create(fileOcclusion.C_Str()));
			}
			else if (fileOcclusion.length > 0) {
				if (const aiTexture* texture = pScene->GetEmbeddedTexture(fileOcclusion.C_Str()))
				{
					out.setOcclusionTexture(CTexture2DFactory::Create(fileOcclusion.C_Str(), texture->mWidth, texture->mHeight, (char*)texture->pcData));
				}
				else {
					LOG_INFO(ELogChannel::Graphics, "Failed to find occlusion texture at: %s for mesh %s", fileOcclusion.C_Str(), m_path);
				}
				
			}

			ai_real occlusionStrength;
			if (AI_SUCCESS == material->Get(AI_MATKEY_GLTF_TEXTURE_STRENGTH(aiTextureType_LIGHTMAP, 0), occlusionStrength))
				out.occlusionStrength = glm::clamp(occlusionStrength, 0.0f, 1.0f);

			// emissive texture

			if (CAsset::exists(fileEmissive.C_Str()))
			{
				out.setEmissiveTexture(da::graphics::CTexture2DFactory::Create(fileEmissive.C_Str()));
			}
			else if (fileEmissive.length > 0) {
				if (const aiTexture* texture = pScene->GetEmbeddedTexture(fileEmissive.C_Str()))
				{
					out.setOcclusionTexture(CTexture2DFactory::Create(fileEmissive.C_Str(), texture->mWidth, texture->mHeight, (char*)texture->pcData));
				}
				else {
					LOG_INFO(ELogChannel::Graphics, "Failed to find emissive texture at: %s for mesh %s", fileEmissive.C_Str(), m_path);
				}	
			}

			aiColor3D emissiveFactor;
			if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveFactor))
				out.emissiveFactor = { emissiveFactor.r, emissiveFactor.g, emissiveFactor.b };
			out.emissiveFactor = glm::clamp(out.emissiveFactor, 0.0f, 1.0f);


			m_materials.push_back(out);
		}

		importer.FreeScene();
#endif
	}

	CSkeletalMesh::~CSkeletalMesh()
	{
		for (size_t i = 0; i < m_materials.size(); i++) {
			if (m_materials[i].getBaseColorTexture()) {
				delete m_materials[i].getBaseColorTexture();
			}

			if (m_materials[i].getEmissiveTexture()) {
				delete m_materials[i].getEmissiveTexture();
			}

			if (m_materials[i].getMetallicRoughnessTexture()) {
				delete m_materials[i].getMetallicRoughnessTexture();
			}

			if (m_materials[i].getNormalTexture()) {
				delete m_materials[i].getNormalTexture();
			}

			if (m_materials[i].getOcclusionTexture()) {
				delete m_materials[i].getOcclusionTexture();
			}
		}
	}

	da::graphics::FMaterialData& CSkeletalMesh::getMaterial(size_t index)
	{
		ASSERT(index < m_materials.size());
		return m_materials[index];
	}

	void CSkeletalMesh::setVertexBoneData(FSkeletalVertexBase& vertex, int boneID, float weight) const
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

}