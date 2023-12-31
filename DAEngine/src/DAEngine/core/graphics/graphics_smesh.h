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
		virtual ~CStaticMesh();

		inline const std::vector<FVertexBase>& getVertices() const override { return m_vertices; }
		inline const std::vector<uint32_t>& getIndices() const override { return m_indices; }
		inline virtual void* getNativeVB() const { return nullptr; }
		inline virtual void* getNativeIB() const { return nullptr; }
		inline const std::string& getPath() const { return m_path; }

	private:
		std::string m_path;

	protected:
		std::vector<FVertexBase> m_vertices;
		std::vector<uint32_t> m_indices;

	};
}
