#include "dapch.h"
#include "graphics_model.h"
#if !defined(DA_TEST)
#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#include "core/memory/memory.h"

namespace da::core
{

	CModel::CModel(const CString& path) : m_path(path)
	{
		tinyobj::attrib_t attrib;
		std::vector<tinyobj::shape_t> shapes;
		std::vector<tinyobj::material_t> materials;
		std::string warn, err;

		tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, m_path.cstr());

		std::unordered_map<FVertexBase, uint32_t> uniqueVertices{};

		for (const auto& shape : shapes) {
			for (const auto& index : shape.mesh.indices) {
				FVertexBase vertex{};
				vertex.Pos = {
					attrib.vertices[3 * index.vertex_index + 0],
					attrib.vertices[3 * index.vertex_index + 1],
					attrib.vertices[3 * index.vertex_index + 2]
				};

				vertex.TexCoord = {
					attrib.texcoords[2 * index.texcoord_index + 0],
					1.0f - attrib.texcoords[2 * index.texcoord_index + 1]
				};

				vertex.Color = { 1.0f, 1.0f, 1.0f };

				if (uniqueVertices.count(vertex) == 0) {
					uniqueVertices[vertex] = static_cast<uint32_t>(m_vertices.size());
					m_vertices.push(vertex);
				}
				m_indices.push(uniqueVertices[vertex]);
			}
		}
	}

	CModel::~CModel()
	{
	}

}
#endif