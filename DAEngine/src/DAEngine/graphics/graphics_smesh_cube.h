#pragma once

#include "graphics_smesh.h"

namespace da::graphics {

	class CStaticMeshCube : public CStaticMesh
	{
	public:
		CStaticMeshCube();

	private:
		std::vector<FVertexBase> m_vertices;
		std::vector<uint32_t> m_indices;
	};
}
