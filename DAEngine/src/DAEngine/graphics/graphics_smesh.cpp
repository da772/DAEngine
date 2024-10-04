
#include "core/asset/asset.h"
#include "graphics_smesh.h"
#include "factory/factory_graphics_texture2d.h"

#if !defined(DA_TEST)
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <assimp/pbrmaterial.h>
#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.hpp>
#endif
#include "assimp_conversion_helpers.h"


namespace da
{
	using namespace da;

	CStaticMesh::CStaticMesh(const std::string& path, bool process) : m_path(path)
	{
		if (!process)
		{
			std::fstream in;
			in.open(path.c_str(), std::ios::in| std::ios::binary);

			size_t meshCount = 0;
			size_t materialCount = 0;

			in.read((char*)&meshCount, sizeof(size_t));
			in.read((char*)&materialCount, sizeof(size_t));

			m_meshes.resize(meshCount);
			m_materials.resize(materialCount);

			for (size_t i = 0; i < meshCount; i++)
			{
				FMesh mesh;
				size_t vertexCount = 0;
				size_t indexCount = 0;

				in.read((char*)&mesh.MaterialIndex, sizeof(size_t));
				// temp
				mesh.MaterialIndex = 0;
				in.read((char*)&vertexCount, sizeof(size_t));
				in.read((char*)&indexCount, sizeof(size_t));

				mesh.Vertices.resize(vertexCount);
				mesh.Indices.resize(indexCount);

				in.read((char*)mesh.Vertices.data(), vertexCount * sizeof(FVertexBase));
				in.read((char*)mesh.Indices.data(), indexCount*sizeof(uint32_t));
				m_meshes[i] = mesh;
			}

			in.close();
			return;
		}

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
			| aiProcess_CalcTangentSpace
			| aiProcess_FixInfacingNormals
			| aiProcess_PreTransformVertices
			| aiProcess_TransformUVCoords
			| aiProcess_FlipWindingOrder
			| aiProcess_FlipUVs
		);

		m_meshes = {};
		m_meshes.reserve(pScene->mNumMeshes);

		std::queue<aiNode*> q;
		q.push(pScene->mRootNode);
		size_t count = 0;
		while (!q.empty()) {

			aiNode* node = q.front();
			q.pop();

			for (size_t i = 0; i < node->mNumMeshes; i++)
			{
				std::vector<FVertexBase> vertices;
				std::vector<uint32_t> indices;

				aiMesh* mesh = pScene->mMeshes[node->mMeshes[i]];

				glm::mat4 parentTransform = glm::mat4(1.f);//node->mParent ? AssimpGLMHelpers::ConvertMatrixToGLMFormat(node->mParent->mTransformation) : glm::mat4(1.f);
				glm::mat4 transform = glm::mat4(1.f); //AssimpGLMHelpers::ConvertMatrixToGLMFormat(node->mTransformation) * parentTransform;

				for (size_t v = 0; v < mesh->mNumVertices; v++) {

					FVertexBase vertex{};
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


		m_materials = {};
		m_materials.reserve(pScene->mNumMaterials);

		for (size_t i = 0; i < pScene->mNumMaterials; i++) {

			aiMaterial* material = pScene->mMaterials[i];

			da::FMaterialData out;

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
				out.setBaseColorTexture(CTexture2DFactory::Create(fileBaseColor.C_Str()));
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
				out.setMetallicRoughnessTexture(da::CTexture2DFactory::Create(fileMetallicRoughness.C_Str()));
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
				out.setNormalTexture(da::CTexture2DFactory::Create(fileNormals.C_Str()));
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
				out.setOcclusionTexture(da::CTexture2DFactory::Create(fileOcclusion.C_Str()));
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
				out.setEmissiveTexture(da::CTexture2DFactory::Create(fileEmissive.C_Str()));
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

	CStaticMesh::~CStaticMesh()
	{
	
	}

	da::FMaterialData& CStaticMesh::getMaterial(size_t index)
	{
		return m_materials[index];
		ASSERT(index < m_materials.size());
	}

}