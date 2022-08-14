#pragma once
#if !defined(DA_TEST)

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
		CStaticMesh(const CBasicString<memory::CGraphicsAllocator>& path);
		inline CStaticMesh() {};
		~CStaticMesh();

		inline const TArray<FVertexBase, memory::CGraphicsAllocator>& getVertices() const override { return m_vertices; }
		inline const TArray<uint32_t, memory::CGraphicsAllocator>& getIndices() const override { return m_indices; }

	private:
		CBasicString<memory::CGraphicsAllocator> m_path;

	protected:
		TArray<FVertexBase, memory::CGraphicsAllocator> m_vertices;
		TArray<uint32_t, memory::CGraphicsAllocator> m_indices;

	};
}

#endif
