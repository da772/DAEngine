#include "dapch.h"
#include "graphics_smesh_cube.h"

namespace da::core
{

	CStaticMeshCube::CStaticMeshCube()
	{
		m_vertices.resize(8);
		m_vertices[0] = { {0,0}, {-1.f, -1.f, 1.f} };
		m_vertices[1] = { {0,0}, {1.f, -1.f, 1.f} };
		m_vertices[2] = { {0,0}, {1.f, -1.f, -1.f} };
		m_vertices[3] = { {0,0}, {-1.f, -1.f, -1.f} };
		m_vertices[4] = { {0,0}, {-1.f, 1.f, 1.f} };
		m_vertices[5] = { {0,0}, {1.f, 1.f, 1.f} };
		m_vertices[6] = { {0,0}, {1.f, 1.f, -1.f} };
		m_vertices[7] = { {0,0}, {-1.f, 1.f, -1.f} };

		m_indices = {
			1,2,6,
			6,5,1,
			0,4,7,
			7,3,0,
			4,5,6,
			6,7,4,
			0,3,2,
			2,1,0,
			0,1,5,
			5,4,0,
			3,7,6,
			6,2,3
		};
	}

}