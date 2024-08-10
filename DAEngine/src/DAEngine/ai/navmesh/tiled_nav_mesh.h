#pragma once
#include "core.h"
#include "nav_mesh.h"
#include <recast.h>

typedef unsigned int dtPolyRef;

namespace da::ai
{
	
	class CTiledNavMesh : public INavMesh
	{
	public:
		CTiledNavMesh(da::graphics::CStaticMesh* mesh);

		void initialize() override;
		void update(float dt) override;
		void shutdown() override;
		std::vector<glm::vec3> findPath(const glm::vec3& startPos, const glm::vec3& endPos) override;
#ifdef DA_REVIEW
		void debugRender() override;
#endif

	private:
		void buildTiles();
		void setAgentPosition(const glm::vec3& pos);
		uint8_t* buildTileMesh(const int tx, const int ty, const float* bmin, const float* bmax, int& dataSize, const float* verts, uint32_t vsize, const uint32_t* tris, uint32_t ntris);
		void setupMesh(da::graphics::CStaticMesh* mesh);
		static bool getSteerTarget(dtNavMeshQuery* navQuery, const float* startPos, const float* endPos,
			const float minTargetDist,
			const dtPolyRef* path, const int pathSize,
			float* steerPos, unsigned char& steerPosFlag, dtPolyRef& steerPosRef,
			float* outPoints = 0, int* outPointCount = 0);
		static inline bool inRange(const float* v1, const float* v2, const float r, const float h)
		{
			const float dx = v2[0] - v1[0];
			const float dy = v2[1] - v1[1];
			const float dz = v2[2] - v1[2];
			return (dx * dx + dz * dz) < r * r && fabsf(dy) < h;
		}

	private:
		float m_lastBuiltTileBmin[3], m_lastBuiltTileBmax[3];
		dtNavMesh* m_navMesh;
		dtNavMeshQuery* m_navQuery;
		dtCrowd* m_crowd = nullptr;
		struct ::rcPolyMesh* m_pmesh;
		struct ::rcPolyMeshDetail* m_dmesh;

		float m_bmax[3] = { -1e9, -1e9, -1e9 };
		float m_bmin[3] = { 1e9, 1e9, 1e9 };

		float m_agentHeight = 1.5f;
		float m_agentMaxClimb = .9f;
		float m_agentRadius = .3f;
		float m_tileSize = 32.f;;
		float m_cellSize = .3f;
		float* m_verts;
		uint32_t m_nverts;
		uint32_t* m_tris;
		uint32_t m_ntris;
	};
}