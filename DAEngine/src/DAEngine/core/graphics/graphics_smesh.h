#pragma once
#if !defined(DA_TEST)

#include "daengine/core/containers.h"
#include "graphics_pipeline.h"
#include "graphics_vertex.h"

namespace da::core
{
	class CStaticMesh
	{
	public:
		CStaticMesh(const CBasicString<memory::CGraphicsAllocator>& path);
		~CStaticMesh();

		inline const TList<FVertexBase, memory::CGraphicsAllocator> getVertices() const { return m_vertices; }
		inline const TList<uint32_t, memory::CGraphicsAllocator> getIndices() const { return m_indices; }


	private:
		CBasicString<memory::CGraphicsAllocator> m_path;
		TList<FVertexBase, memory::CGraphicsAllocator> m_vertices;
		TList<uint32_t, memory::CGraphicsAllocator> m_indices;

	};
}

#endif
