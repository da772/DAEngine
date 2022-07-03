#include "dapch.h"
#include "graphics_model.h"
#if !defined(DA_TEST)
#include "core/memory/memory.h"
#include "core/file.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace da::core
{

	CModel::CModel(const CBasicString<memory::CGraphicsAllocator>& path) : m_path(path)
	{
		CFile file(m_path);

		Assimp::Importer importer;
	
		const aiScene* pScene = importer.ReadFileFromMemory(file.data(), file.size()*sizeof(char), aiProcess_Triangulate | aiProcess_GenNormals | aiProcess_JoinIdenticalVertices | aiProcess_FlipUVs);

		for (size_t i = 0; i < pScene->mNumMeshes; i++)
		{
			
			for (size_t v = 0; v < pScene->mMeshes[i]->mNumVertices; v++) {
				FVertexBase vertex{};
				vertex.Pos = {
					pScene->mMeshes[i]->mVertices[v].x,
					pScene->mMeshes[i]->mVertices[v].y,
					pScene->mMeshes[i]->mVertices[v].z
				};

				vertex.TexCoord = {
					pScene->mMeshes[i]->mTextureCoords[0][v].x,
					pScene->mMeshes[i]->mTextureCoords[0][v].y
				};
				

				m_vertices.push(vertex);
			}

			for (size_t j = 0; j < pScene->mMeshes[i]->mNumFaces; j++) {
				for (size_t m = 0; m < pScene->mMeshes[i]->mFaces[j].mNumIndices; m++) {
					m_indices.push(pScene->mMeshes[i]->mFaces[j].mIndices[m]);
				}
				
			}
		}

		importer.FreeScene();
		memory::pop_memory_layer();
	}

	CModel::~CModel()
	{
	}

}
#endif