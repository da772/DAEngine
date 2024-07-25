#include "dapch.h"
#include "tiled_nav_mesh.h"
#include "nav_mesh_manager.h"


#include "chunky_mesh.h"
#include "core/ecs/scene_manager.h"
#include "core/ecs/scene.h"
#include "core/ecs/entity.h"
#include "core/graphics/graphics_debug_render.h"
#include "core/graphics/graphics_smesh.h"
#include "platform/graphics/bgfx/bgfx_static_mesh.h";

#include <DetourNavMeshBuilder.h>
#include <DetourNavMeshQuery.h>
#include <DetourCrowd.h>
#include <RecastDebugDraw.h>
#include <DetourDebugDraw.h>


namespace da::ai
{
	CTiledNavMesh ::CTiledNavMesh (da::graphics::CStaticMesh* mesh)
	{
		setupMesh(mesh);
	}

	void CTiledNavMesh ::initialize()
	{
		m_navQuery = dtAllocNavMeshQuery();
		m_navMesh = dtAllocNavMesh();
		dtNavMeshParams params;
		params.orig[0] = m_bmin[0];
		params.orig[1] = 0.f;
		params.orig[2] = m_bmin[2];
		//rcVcopy(, m_geom->getNavMeshBoundsMin());
		params.tileWidth = m_tileSize * m_cellSize;
		params.tileHeight = m_tileSize * m_cellSize;
		params.maxTiles = 512;
		params.maxPolys = 8192;
		m_navMesh->init(&params);
		m_navQuery->init(m_navMesh, 2048);
		buildTiles();
		setAgentPosition({ -43.331f, 12.621, 3.652 });
	}


	void CTiledNavMesh ::setAgentPosition(const glm::vec3& npos)
	{
		const float agentRadius = .6f;
		const float agentHeight = 2.f;

		if (!m_crowd) {
			m_crowd = new dtCrowd();
			m_crowd->init(128, agentRadius, m_navMesh);
		}

		const float pos[3] = { -11.f, 7.11f,-16.2f };

		dtCrowdAgentParams ap;
		memset(&ap, 0, sizeof(ap));
		ap.radius = agentRadius;
		ap.height = agentHeight;
		ap.maxAcceleration = 8.0f;
		ap.maxSpeed = 3.5f;
		ap.collisionQueryRange = ap.radius * 12.0f;
		ap.pathOptimizationRange = ap.radius * 30.0f;
		ap.updateFlags = 0;
		/*
		if (m_toolParams.m_anticipateTurns)
			ap.updateFlags |= DT_CROWD_ANTICIPATE_TURNS;
		if (m_toolParams.m_optimizeVis)
			ap.updateFlags |= DT_CROWD_OPTIMIZE_VIS;
		if (m_toolParams.m_optimizeTopo)
			ap.updateFlags |= DT_CROWD_OPTIMIZE_TOPO;
		if (m_toolParams.m_obstacleAvoidance)
			ap.updateFlags |= DT_CROWD_OBSTACLE_AVOIDANCE;
		if (m_toolParams.m_separation)
			ap.updateFlags |= DT_CROWD_SEPARATION;
			*/
		ap.obstacleAvoidanceType = (unsigned char)3;
		ap.separationWeight = 2;

		if (m_idx == -1) {

			m_idx = m_crowd->addAgent(pos, &ap);

			if (m_idx == -1)
			{
				LOG_ERROR(ELogChannel::AI, "crowd: Could not agent");
				return;
			}
		}

		const dtQueryFilter* filter = m_crowd->getFilter(0);
		const float* halfExtents = m_crowd->getQueryExtents();
		dtPolyRef targetRef;
		float target[3];

		float newPos[3] = { npos.x, npos.z, npos.y };

	
		m_navQuery->findNearestPoly(newPos, halfExtents, filter, &targetRef, target);

		const dtCrowdAgent* ag = m_crowd->getAgent(m_idx);
		m_crowd->resetMoveTarget(m_idx);
		if (ag && ag->active)
			m_crowd->requestMoveTarget(m_idx, targetRef, target);
	}


	void CTiledNavMesh ::buildTiles()
	{
		da::graphics::CStaticMesh* mesh = new da::platform::CBgfxStaticMesh("assets/navmesh.fbx", false);


		int gw = 0, gh = 0;
		rcCalcGridSize(m_bmin, m_bmax, m_cellSize, &gw, &gh);
		const int ts = (int)m_tileSize;
		const int tw = (gw + ts - 1) / ts;
		const int th = (gh + ts - 1) / ts;
		const float tcs = m_tileSize * m_cellSize;

		for (int y = 0; y < th; ++y)
		{
			for (int x = 0; x < tw; ++x)
			{
				m_lastBuiltTileBmin[0] = m_bmin[0] + x * tcs;
				m_lastBuiltTileBmin[1] = m_bmin[1];
				m_lastBuiltTileBmin[2] = m_bmin[2] + y * tcs;

				m_lastBuiltTileBmax[0] = m_bmin[0] + (x + 1) * tcs;
				m_lastBuiltTileBmax[1] = m_bmax[1];
				m_lastBuiltTileBmax[2] = m_bmin[2] + (y + 1) * tcs;

				int dataSize = 0;
				unsigned char* data = buildTileMesh(x, y, m_lastBuiltTileBmin, m_lastBuiltTileBmax, dataSize, mesh, m_verts, m_nverts, m_tris, m_ntris);
				if (data)
				{
					// Remove any previous data (navmesh owns and deletes the data).
					m_navMesh->removeTile(m_navMesh->getTileRefAt(x, y, 0), 0, 0);
					// Let the navmesh own the data.
					dtStatus status = m_navMesh->addTile(data, dataSize, DT_TILE_FREE_DATA, 0, 0);
					if (dtStatusFailed(status))
						dtFree(data);
				}
			}
		}
	}

	uint8_t* CTiledNavMesh ::buildTileMesh(const int tx, const int ty, const float* bmin, const float* bmax, int& dataSize, da::graphics::CStaticMesh* mesh, const float* verts, uint32_t nverts, const uint32_t* tris, uint32_t ntris)
	{

		const float agentHeight = 2.f;
		const float agentMaxClimb = .9f;
		const float agentRadius = .6f;
		const float tileSize = 32;

		rcConfig cfg;

		// Rasterization
		cfg.cs = .3f;
		cfg.ch = .2f;
		// Agent
		cfg.walkableSlopeAngle = 45.f;
		cfg.walkableHeight = (int)ceilf(agentHeight / cfg.ch);
		cfg.walkableClimb = (int)floorf(agentMaxClimb / cfg.ch);
		cfg.walkableRadius = (int)ceilf(agentRadius / cfg.cs);
		// Polygonization
		cfg.maxEdgeLen = (int)(12.f / cfg.cs);
		cfg.maxSimplificationError = 1.3f;
		// Region
		cfg.minRegionArea = 8.f * 8.f;
		cfg.mergeRegionArea = 20.f * 20.f;
		cfg.maxVertsPerPoly = 6.f;
		// Tiling
		cfg.tileSize = 32;
		cfg.borderSize = cfg.walkableRadius + 3;
		cfg.width = cfg.tileSize + cfg.borderSize * 2;
		cfg.height = cfg.tileSize + cfg.borderSize * 2;
		cfg.detailSampleDist = 6.f < 0.9f ? 0 : cfg.cs * 6.f;
		cfg.detailSampleMaxError = cfg.ch * 1.f;

		rcVcopy(cfg.bmin, bmin);
		rcVcopy(cfg.bmax, bmax);
		cfg.bmin[0] -= cfg.borderSize * cfg.cs;
		cfg.bmin[2] -= cfg.borderSize * cfg.cs;
		cfg.bmax[0] += cfg.borderSize * cfg.cs;
		cfg.bmax[2] += cfg.borderSize * cfg.cs;

		rcChunkyTriMesh chunkyMesh;
		if (!rcCreateChunkyTriMesh(verts, (const int*)tris, ntris, 256, &chunkyMesh))
		{
			LOG_ERROR(ELogChannel::AI, "buildNavigation: Could not create chunky mesh");
			return 0;
		}

		rcHeightfield* solid = rcAllocHeightfield();

		if (!rcCreateHeightfield(&CNavMeshManager::getCtx(), *solid, cfg.width, cfg.height, cfg.bmin, cfg.bmax, cfg.cs, cfg.ch))
		{
			LOG_ERROR(ELogChannel::AI, "buildNavigation: Could not create solid heightfield.");
			return 0;
		}

		uint8_t* triareas = new uint8_t[chunkyMesh.maxTrisPerChunk];

		float tbmin[2], tbmax[2];
		tbmin[0] = cfg.bmin[0];
		tbmin[1] = cfg.bmin[2];
		tbmax[0] = cfg.bmax[0];
		tbmax[1] = cfg.bmax[2];
		int cid[512];// TODO: Make grow when returning too many items.
		const int ncid = rcGetChunksOverlappingRect(&chunkyMesh, tbmin, tbmax, cid, 512);

		if (!ncid)
		{
			LOG_ERROR(ELogChannel::AI, "buildNavigation: Could not create chunky mesh");
			return 0;
		}

		int tileTriCount = 0;

		for (int i = 0; i < ncid; ++i)
		{
			const rcChunkyTriMeshNode& node = chunkyMesh.nodes[cid[i]];
			const int* ctris = &chunkyMesh.tris[node.i * 3];
			const int nctris = node.n;

			tileTriCount += nctris;

			memset(triareas, 0, nctris * sizeof(unsigned char));
			rcMarkWalkableTriangles(&CNavMeshManager::getCtx(), cfg.walkableSlopeAngle,
				verts, nverts, ctris, nctris, triareas);

			if (!rcRasterizeTriangles(&CNavMeshManager::getCtx(), verts, nverts, ctris, triareas, nctris, *solid, cfg.walkableClimb)) {
				LOG_ERROR(ELogChannel::AI, "Failed to rasterize");
				return 0;
			}
		}

		delete[] triareas;
		triareas = 0;

		// as well as filter spans where the character cannot possibly stand.
		rcFilterLowHangingWalkableObstacles(&CNavMeshManager::getCtx(), cfg.walkableClimb, *solid);
		rcFilterLedgeSpans(&CNavMeshManager::getCtx(), cfg.walkableHeight, cfg.walkableClimb, *solid);
		rcFilterWalkableLowHeightSpans(&CNavMeshManager::getCtx(), cfg.walkableHeight, *solid);

		rcCompactHeightfield* chf = rcAllocCompactHeightfield();

		if (!rcBuildCompactHeightfield(&CNavMeshManager::getCtx(), cfg.walkableHeight, cfg.walkableClimb, *solid, *chf))
		{
			LOG_ERROR(ELogChannel::AI, "buildNavigation: Could not build compact data.");
			return 0;
		}

		rcFreeHeightField(solid);
		solid = 0;

		if (!rcErodeWalkableArea(&CNavMeshManager::getCtx(), cfg.walkableRadius, *chf))
		{
			LOG_ERROR(ELogChannel::AI, "buildNavigation: Could not erode.");
			return 0;
		}

		/* // Slower
		if (!rcBuildDistanceField(&CNavMeshManager::getCtx(), *chf))
		{
			LOG_ERROR(ELogChannel::AI, "buildNavigation: Could not build distance field.");
			return 0;
		}


		// Partition the walkable surface into simple regions without holes.
		if (!rcBuildRegions(&CNavMeshManager::getCtx(), *chf, cfg.borderSize, cfg.minRegionArea, cfg.mergeRegionArea))
		{
			LOG_ERROR(ELogChannel::AI, "buildNavigation: Could not build watershed regions.");
			return 0;
		}
		*/


		if (!rcBuildRegionsMonotone(&CNavMeshManager::getCtx(), *chf, cfg.borderSize, cfg.minRegionArea, cfg.mergeRegionArea))
		{
			LOG_ERROR(ELogChannel::AI, "buildNavigation: Could not build monotone regions.");
			return 0;
		}

		rcContourSet* cset = rcAllocContourSet();

		if (!rcBuildContours(&CNavMeshManager::getCtx(), *chf, cfg.maxSimplificationError, cfg.maxEdgeLen, *cset))
		{
			LOG_ERROR(ELogChannel::AI, "buildNavigation: Could not create contours.");
			return 0;
		}

		if (cset->nconts == 0)
		{
			LOG_ERROR(ELogChannel::AI, "buildNavigation: Could not create contours.");
			return 0;
		}

		m_pmesh = rcAllocPolyMesh();

		if (!rcBuildPolyMesh(&CNavMeshManager::getCtx(), *cset, cfg.maxVertsPerPoly, *m_pmesh))
		{
			LOG_ERROR(ELogChannel::AI, "buildNavigation: Could not triangulate contours.");
			return 0;
		}

		m_dmesh = rcAllocPolyMeshDetail();

		if (!rcBuildPolyMeshDetail(&CNavMeshManager::getCtx(), *m_pmesh, *chf,
			cfg.detailSampleDist, cfg.detailSampleMaxError,
			*m_dmesh))
		{
			LOG_ERROR(ELogChannel::AI, "buildNavigation: Could build polymesh detail.");
			return 0;
		}

		rcFreeCompactHeightfield(chf);
		chf = 0;
		rcFreeContourSet(cset);
		cset = 0;

		unsigned char* navData = 0;
		int navDataSize = 0;

		for (int i = 0; i < m_pmesh->npolys; ++i)
		{
			if (m_pmesh->areas[i] == RC_WALKABLE_AREA)
				m_pmesh->areas[i] = 0;

			m_pmesh->flags[i] = 1;
		}

		dtNavMeshCreateParams params;
		memset(&params, 0, sizeof(params));
		params.verts = m_pmesh->verts;
		params.vertCount = m_pmesh->nverts;
		params.polys = m_pmesh->polys;
		params.polyAreas = m_pmesh->areas;
		params.polyFlags = m_pmesh->flags;
		params.polyCount = m_pmesh->npolys;
		params.nvp = m_pmesh->nvp;
		params.detailMeshes = m_dmesh->meshes;
		params.detailVerts = m_dmesh->verts;
		params.detailVertsCount = m_dmesh->nverts;
		params.detailTris = m_dmesh->tris;
		params.detailTriCount = m_dmesh->ntris;
		//params.offMeshConVerts = m_geom->getOffMeshConnectionVerts();
		//params.offMeshConRad = m_geom->getOffMeshConnectionRads();
		//params.offMeshConDir = m_geom->getOffMeshConnectionDirs();
		//params.offMeshConAreas = m_geom->getOffMeshConnectionAreas();
		//params.offMeshConFlags = m_geom->getOffMeshConnectionFlags();
		//params.offMeshConUserID = m_geom->getOffMeshConnectionId();
		//params.offMeshConCount = m_geom->getOffMeshConnectionCount();
		params.walkableHeight = agentHeight;
		params.walkableRadius = agentRadius;
		params.walkableClimb = agentMaxClimb;
		params.tileX = tx;
		params.tileY = ty;
		params.tileLayer = 0;
		rcVcopy(params.bmin, m_pmesh->bmin);
		rcVcopy(params.bmax, m_pmesh->bmax);
		params.cs = cfg.cs;
		params.ch = cfg.ch;
		params.buildBvTree = true;

		if (!dtCreateNavMeshData(&params, &navData, &navDataSize))
		{
			LOG_ERROR(ELogChannel::AI, "Could not build Detour navmesh.");
			return 0;
		}

		LOG_INFO(ELogChannel::AI, "buildNavigation: Success?");
		return navData;
	}

	void CTiledNavMesh ::shutdown()
	{
		delete m_navMesh;
		delete m_navQuery;
		delete m_crowd;
		delete m_dmesh;
		delete m_pmesh;
	}


	void CTiledNavMesh ::update(float dt)
	{
		if (da::core::CScene* scene = da::core::CSceneManager::getScene()) {

			for (const da::core::CEntity* e : scene->getEntities()) {
				if (e->getTag() == HASHSTR("Character")) {
					setAgentPosition(e->getTransform().position());
				}
			}
		}

		m_crowd->update(dt, 0);
	}

#ifdef DA_REVIEW
	void CTiledNavMesh ::debugRender()
	{
		// Tiling grid.
		int gw = 0, gh = 0;
		float bmin[3] = { -100, 0, -100 };
		float bmax[3] = { 100, 0, 100 };
		rcCalcGridSize(bmin, bmax, m_cellSize, &gw, &gh);
		const int tw = (gw + (int)m_tileSize - 1) / (int)m_tileSize;
		const int th = (gh + (int)m_tileSize - 1) / (int)m_tileSize;
		const float s = m_tileSize * m_cellSize;
		//duDebugDrawGridXZ(&CNavMeshManager::getDrawer(), bmin[0], bmin[1], bmin[2], tw, th, s, duRGBA(0, 0, 0, 64), 1.0f);
		duDebugDrawNavMeshWithClosedList(&CNavMeshManager::getDrawer(), *m_navMesh, *m_navQuery, 3);

		if (m_idx != -1) {
			const dtCrowdAgent* agent = m_crowd->getAgent(m_idx);
			const dtPolyRef* path = agent->corridor.getPath();
			const int npath = agent->corridor.getPathCount();
			for (int j = 0; j < npath; ++j)
				duDebugDrawNavMeshPoly(&CNavMeshManager::getDrawer(), *m_navMesh, path[j], duRGBA(0, 255, 0, 255));

			glm::vec3 pos = { agent->npos[0], agent->npos[2], agent->npos[1] + .5f };
			da::graphics::CDebugRender::drawCapsule(pos, glm::quat(1.f, 0.f, 0.f, 0.f), { 1.f, 1.f, 1.f }, { 0.f, 1.f, 0.f, 1.f }, false, true);
		}
	}
#endif

	void CTiledNavMesh ::setupMesh(da::graphics::CStaticMesh* mesh)
	{
		m_bmax[0] = -1e9;
		m_bmax[1] = -1e9;
		m_bmax[2] = -1e9;
		m_bmin[0] = 1e9;
		m_bmin[1] = 1e9;
		m_bmin[2] = 1e9;

		m_nverts = mesh->getMeshes()[0].Vertices.size();
		m_verts = new float[m_nverts * 3];

		for (int i = 0; i < m_nverts; i++) {
			const da::graphics::FVertexBase& v = mesh->getMeshes()[0].Vertices[i];
			m_verts[i * 3 + 0] = v.Pos.x;
			m_verts[i * 3 + 1] = v.Pos.z;
			m_verts[i * 3 + 2] = v.Pos.y;

			if (v.Pos.x < m_bmin[0]) {
				m_bmin[0] = v.Pos.x;
			}

			if (v.Pos.y < m_bmin[2])
			{
				m_bmin[2] = v.Pos.y;
			}

			if (v.Pos.z < m_bmin[1]) {
				m_bmin[1] = v.Pos.z;
			}


			if (v.Pos.x > m_bmax[0]) {
				m_bmax[0] = v.Pos.x;
			}

			if (v.Pos.y > m_bmax[2])
			{
				m_bmax[2] = v.Pos.y;
			}

			if (v.Pos.z > m_bmax[1]) {
				m_bmax[1] = v.Pos.z;
			}
		}


		m_ntris = mesh->getMeshes()[0].Indices.size() / 3;
		m_tris = (uint32_t*)mesh->getMeshes()[0].Indices.data();
	}

}