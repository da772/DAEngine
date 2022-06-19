#pragma once
#if !defined(DA_TEST)

#include "daengine/core/containers.h"
#include "graphics_pipeline.h"
#include "graphics_vertex.h"

namespace da::core
{
	class CModel
	{
	public:
		CModel(const CString& path);
		~CModel();

		inline const TList<FVertexBase> getVertices() const { return m_vertices; }

		inline const TList<uint32_t> getIndices() const { return m_indices; }


	private:
		CString m_path;
		TList<FVertexBase> m_vertices;
		TList<uint32_t> m_indices;

	};
}

#endif
