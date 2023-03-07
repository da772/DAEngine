#include "dapch.h"
#include "graphics_smesh.h"
#include "core/memory/memory.h"
#include "asset/asset.h"
#if !defined(DA_TEST)
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#endif


namespace da::core
{
	CStaticMesh::CStaticMesh(const CBasicString<memory::CGraphicsAllocator>& path) : m_path(path)
	{
		CAsset file(path);
#if !defined(DA_TEST)
		Assimp::Importer importer;
	
		TList<FVertexBase, memory::CGraphicsAllocator> vertices;
		TList<uint32_t, memory::CGraphicsAllocator> indices;

		const aiScene* pScene = importer.ReadFileFromMemory(file.data(), file.size()*sizeof(char), aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		for (size_t i = 0; i < pScene->mNumMeshes; i++)
		{
			for (size_t v = 0; v < pScene->mMeshes[i]->mNumVertices; v++) {
				FVertexBase vertex{};
				vertex.Pos = {
					pScene->mMeshes[i]->mVertices[v].x,
					pScene->mMeshes[i]->mVertices[v].y,
					pScene->mMeshes[i]->mVertices[v].z
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

					vertex.BitTangent = {
						pScene->mMeshes[i]->mBitangents[v].x,
						pScene->mMeshes[i]->mBitangents[v].y,
						pScene->mMeshes[i]->mBitangents[v].z,
					};
				}

				vertices.push(vertex);
			}

			for (size_t j = 0; j < pScene->mMeshes[i]->mNumFaces; j++) {
				for (size_t m = 0; m < pScene->mMeshes[i]->mFaces[j].mNumIndices; m++) {
					indices.push(pScene->mMeshes[i]->mFaces[j].mIndices[m]);
				}
				
			}
		}

		m_vertices = TArray<FVertexBase, memory::CGraphicsAllocator>(vertices);
		m_indices = TArray<uint32_t, memory::CGraphicsAllocator>(indices);

		importer.FreeScene();
#endif
	}

	CStaticMesh::~CStaticMesh()
	{
	}

}