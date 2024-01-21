#include "dapch.h"
#include "graphics_smesh.h"

#include "asset/asset.h"
#if !defined(DA_TEST)
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.hpp>
#endif

namespace da::core
{
	CStaticMesh::CStaticMesh(const std::string& path) : m_path(path)
	{
		CAsset file(path.c_str());
#if !defined(DA_TEST)
		Assimp::Importer importer;
	
		std::vector<FVertexBase> vertices;
		std::vector<uint32_t> indices;

		const aiScene* pScene = importer.ReadFileFromMemory(file.data(), file.size()*sizeof(char), aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_ConvertToLeftHanded | aiProcess_CalcTangentSpace | aiProcess_FixInfacingNormals);
		static glm::mat4 transformMat = glm::translate(glm::mat4(1.0f), { 0.f,0.f,0.f }) * glm::rotate(glm::mat4(1.0f), glm::radians(-180.f), glm::vec3(0.0f, 1.0f, 0.0f));

		for (size_t i = 0; i < pScene->mNumMeshes; i++)
		{
			for (size_t v = 0; v < pScene->mMeshes[i]->mNumVertices; v++) {
				FVertexBase vertex{};
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
					vertex.Normal = {
						pScene->mMeshes[i]->mNormals[v].x,
						pScene->mMeshes[i]->mNormals[v].y,
						pScene->mMeshes[i]->mNormals[v].z
					};
				}

				if (pScene->mMeshes[i]->HasTangentsAndBitangents())
				{
					vertex.Tangent = {
						pScene->mMeshes[i]->mTangents[v].x,
						pScene->mMeshes[i]->mTangents[v].y,
						pScene->mMeshes[i]->mTangents[v].z,
					};
				}

				vertices.push_back(vertex);
			}

			for (size_t j = 0; j < pScene->mMeshes[i]->mNumFaces; j++) {
				for (size_t m = 0; m < pScene->mMeshes[i]->mFaces[j].mNumIndices; m++) {
					indices.push_back(pScene->mMeshes[i]->mFaces[j].mIndices[m]);
				}
				
			}
		}

		m_vertices = std::vector<FVertexBase>(vertices);
		m_indices = std::vector<uint32_t>(indices);

		importer.FreeScene();
#endif
	}

	CStaticMesh::~CStaticMesh()
	{
	}

}