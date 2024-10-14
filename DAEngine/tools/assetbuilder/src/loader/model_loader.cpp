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


std::unordered_map<CHashString, FAssetData> FMaterial::ms_materialSaved;
std::unordered_map <CHashString, FAssetData> CModelLoader::ms_modelSaved;
std::mutex CModelLoader::ms_mutex;



bool FMaterial::hasSaved(FMaterial& mat)
{
	std::lock_guard <std::mutex> lockguard(CModelLoader::ms_mutex);
	CHashString guid = mat.getHash();
	return ms_materialSaved.find(guid) != ms_materialSaved.end();
}


CModelLoader::CModelLoader(const std::string& path, const std::string& targetPath, const std::string& dir, const std::string& name)
	: m_path(path), m_dir(dir), m_name(name), m_targetPath(targetPath)
{
	m_materialTargetPath += "\\";
	m_textureTargetPath += "\\";
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
				out.m_baseColorTexture = CTextureLoader(m_path, m_name + "_Alb", m_textureTargetPath, (uint32_t)texture->mWidth, (uint32_t)texture->mHeight, (uint8_t*)texture->pcData);
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
				out.m_metallicRoughnessTexture = CTextureLoader(m_path, m_name + "_Mtl", m_textureTargetPath, (uint32_t)texture->mWidth, (uint32_t)texture->mHeight, (uint8_t*)texture->pcData);
			}
		}

		ai_real metallicFactor;
		if (AI_SUCCESS == material->Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_METALLIC_FACTOR, metallicFactor))
			out.metallicFactor = glm::clamp(metallicFactor, 0.0f, 1.0f);
		ai_real roughnessFactor;
		if (AI_SUCCESS == material->Get(AI_MATKEY_GLTF_PBRMETALLICROUGHNESS_ROUGHNESS_FACTOR, roughnessFactor))
			out.roughnessFactor = glm::clamp(roughnessFactor, 0.0f, 1.0f);

		// normal map
		 if (fileNormals.length > 0) {
			if (const aiTexture* texture = pScene->GetEmbeddedTexture(fileNormals.C_Str()))
			{
				out.m_normalTexture = CTextureLoader(m_path, m_name + "_Nrm", m_textureTargetPath, (uint32_t)texture->mWidth, (uint32_t)texture->mHeight, (uint8_t*)texture->pcData);
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
				out.m_occlusionTexture = CTextureLoader(m_path, m_name + "_Occ", m_textureTargetPath, (uint32_t)texture->mWidth, (uint32_t)texture->mHeight, (uint8_t*)texture->pcData);
			}
		}

		ai_real occlusionStrength;
		if (AI_SUCCESS == material->Get(AI_MATKEY_GLTF_TEXTURE_STRENGTH(aiTextureType_LIGHTMAP, 0), occlusionStrength))
			out.occlusionStrength = glm::clamp(occlusionStrength, 0.0f, 1.0f);

		// emissive texture
		if (const aiTexture* texture = pScene->GetEmbeddedTexture(fileEmissive.C_Str()))
		{
			out.m_emissiveTexture = CTextureLoader(m_path, std::string(fileEmissive.C_Str()), m_textureTargetPath, (uint32_t)texture->mWidth, (uint32_t)texture->mHeight, (uint8_t*)texture->pcData);
		}

		aiColor3D emissiveFactor;
		if (AI_SUCCESS == material->Get(AI_MATKEY_COLOR_EMISSIVE, emissiveFactor))
			out.emissiveFactor = { emissiveFactor.r, emissiveFactor.g, emissiveFactor.b };
		out.emissiveFactor = glm::clamp(out.emissiveFactor, 0.0f, 1.0f);

		m_materials.push_back(out);
	}

	for (uint32_t i = 0; i < m_meshes.size(); i++) {
		if (m_meshes[i].MaterialIndex != -1)
		{
			m_materials[m_meshes[i].MaterialIndex].getHash();
			m_meshes[i].MaterialIndex = m_materials[m_meshes[i].MaterialIndex].m_hash.hash();
		}
	}

	return true;
}

bool CModelLoader::saveModel()
{
	importer.FreeScene();
	for (uint32_t i = 0; i < m_materials.size(); i++)
	{
		if (FMaterial::hasSaved(m_materials[i]))
		{
			continue;
		}

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

		std::string guidStr(m_materials[i].getHash().c_str());
		std::string path = (m_materialTargetPath + guidStr + ".mat");

		std::string matName = m_name + "_Mat_" + std::to_string(i);
		FMaterial::ms_materialSaved[m_materials[i].getHash()] = { m_path, matName, path, m_materials[i].m_hash};
		
		std::ofstream out;
		out.open(path.c_str(), std::ios::out | std::ios::trunc);
		
		out << m_materials[i].baseColorFactor.x << "," << m_materials[i].baseColorFactor.y << "," << m_materials[i].baseColorFactor.z << "\n";
		out << m_materials[i].emissiveFactor.x << "," << m_materials[i].emissiveFactor.y << "," << m_materials[i].emissiveFactor.z << "\n";
		out << m_materials[i].metallicFactor << "\n";
		out << m_materials[i].doubleSided << "\n";
		out << m_materials[i].blend << "\n";
		out << m_materials[i].normalScale << "\n";
		out << m_materials[i].occlusionStrength << "\n";
		out << m_materials[i].roughnessFactor << "\n";
		out << m_materials[i].uvScale.x << "," << m_materials[i].uvScale.y << "\n";

		out << m_materials[i].m_baseColorTexture.getName().c_str() << "\n";
		out << m_materials[i].m_occlusionTexture.getName().c_str() << "\n";
		out << m_materials[i].m_normalTexture.getName().c_str() << "\n";
		out << m_materials[i].m_metallicRoughnessTexture.getName().c_str() << "\n";
		out << m_materials[i].m_emissiveTexture.getName().c_str() << "\n";

		out.close();
	}

	std::ostringstream outStream;
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

	std::string& path = m_targetPath;
	std::ofstream out;
	out.open(path.c_str(), std::ios::out | std::ios::trunc | std::ios::binary);
	out.write(outStream.str().c_str(), outStream.str().size());
	out.close();

	CHashString hash = HASHSTR(m_name.c_str());

	std::string relPath;
	size_t i = m_path.find("assets");
	relPath = m_path.substr(i, relPath.size() - i);

	std::string relOutPath;
	i = path.find("assets");
	relOutPath = path.substr(i, path.size() - i);

	ms_modelSaved[hash] = { relPath, m_name, relOutPath, HASHSTR(relOutPath.c_str()) };
	return true;
}

CHashString FMaterial::getHash()
{
	return m_hash;
}
