#pragma once
#include "nav_mesh.h"

namespace da::ai
{
	class CTiledNavMesh : public INavMesh
	{
	public:
		CTiledNavMesh(da::graphics::CStaticMesh* mesh);

		void initialize() override;
		void update(float dt) override;
		void shutdown() override;
#ifdef DA_REVIEW
		void debugRender() override;
#endif

	private:
		void buildTiles();
		void setAgentPosition(const glm::vec3& pos);
		uint8_t* buildTileMesh(const int tx, const int ty, const float* bmin, const float* bmax, int& dataSize, da::graphics::CStaticMesh* mesh, const float* verts, uint32_t vsize, const uint32_t* tris, uint32_t ntris);
		void setupMesh(da::graphics::CStaticMesh* mesh);


	private:
		float m_lastBuiltTileBmin[3], m_lastBuiltTileBmax[3];
		dtNavMesh* m_navMesh;
		dtNavMeshQuery* m_navQuery;
		dtCrowd* m_crowd = nullptr;
		struct ::rcPolyMesh* m_pmesh;
		struct ::rcPolyMeshDetail* m_dmesh;

		float m_bmax[3] = { -1e9, -1e9, -1e9 };
		float m_bmin[3] = { 1e9, 1e9, 1e9 };

		const float m_tileSize = 32.f;
		const float m_cellSize = .3f;
		float* m_verts;
		uint32_t m_nverts;
		uint32_t* m_tris;
		uint32_t m_ntris;
		int m_idx = -1;
	};
}