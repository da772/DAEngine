#pragma once

#include "daengine/core/containers.h"
#include "graphics_pipeline.h"
#include "graphics_vertex.h"
#include "graphics_renderable.h"
#include "graphics_material.h"

namespace da::core
{
	class CStaticMesh : public IRenderable
	{
	public:
		CStaticMesh(const std::string& path);
		inline CStaticMesh() {};
		~CStaticMesh();

		inline const std::vector<FVertexBase>& getVertices() const override { return m_vertices; }
		inline const std::vector<uint32_t>& getIndices() const override { return m_indices; }

	private:
		std::string m_path;

	protected:
		std::vector<FVertexBase> m_vertices;
		std::vector<uint32_t> m_indices;

	};
}
