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
#include <DetourNavMesh.h>
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
		if (!m_crowd) {
			m_crowd = new dtCrowd();
			m_crowd->init(128, m_agentRadius, m_navMesh);
		}

		const float pos[3] = { -11.f, 7.11f,-16.2f };

		dtCrowdAgentParams ap;
		memset(&ap, 0, sizeof(ap));
		ap.radius = m_agentRadius;
		ap.height = m_agentHeight;
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
		/*
		if (false) {

			int idx = m_crowd->addAgent(pos, &ap);

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
		*/
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
		rcConfig cfg;

		// Rasterization
		cfg.cs = .3f;
		cfg.ch = .2f;
		// Agent
		cfg.walkableSlopeAngle = 45.f;
		cfg.walkableHeight = (int)ceilf(m_agentHeight / cfg.ch);
		cfg.walkableClimb = (int)floorf(m_agentMaxClimb / cfg.ch);
		cfg.walkableRadius = (int)ceilf(m_agentRadius / cfg.cs);
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
			LOG_WARN(ELogChannel::AI, "Could not create chunky mesh for tile: %d, %d", tx ,ty);
			return 0;
		}

		rcHeightfield* solid = rcAllocHeightfield();

		if (!rcCreateHeightfield(&CNavMeshManager::getCtx(), *solid, cfg.width, cfg.height, cfg.bmin, cfg.bmax, cfg.cs, cfg.ch))
		{
			LOG_ERROR(ELogChannel::AI, "Could not create solid heightfield.");
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
			LOG_WARN(ELogChannel::AI, "Could not create chunky mesh for tile: %d, %d", tx, ty);
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
			LOG_WARN(ELogChannel::AI, "Could not create contours for tile: %d, %d", tx, ty);
			return 0;
		}

		if (cset->nconts == 0)
		{
			LOG_WARN(ELogChannel::AI, "Could not create contours for tile: %d, %d", tx, ty);
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
		params.walkableHeight = m_agentHeight;
		params.walkableRadius = m_agentRadius;
		params.walkableClimb = m_agentMaxClimb;
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
			LOG_WARN(ELogChannel::AI, "Could not build navmesh data for tile: %d,%d", tx,ty);
			return 0;
		}

		LOG_INFO(ELogChannel::AI, "NavMesh built for tile: %d,%d", tx,ty);
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
	
	static int fixupShortcuts(dtPolyRef* path, int npath, dtNavMeshQuery* navQuery)
	{
		if (npath < 3)
			return npath;

		// Get connected polygons
		static const int maxNeis = 16;
		dtPolyRef neis[maxNeis];
		int nneis = 0;

		const dtMeshTile* tile = 0;
		const dtPoly* poly = 0;
		if (dtStatusFailed(navQuery->getAttachedNavMesh()->getTileAndPolyByRef(path[0], &tile, &poly)))
			return npath;

		for (unsigned int k = poly->firstLink; k != DT_NULL_LINK; k = tile->links[k].next)
		{
			const dtLink* link = &tile->links[k];
			if (link->ref != 0)
			{
				if (nneis < maxNeis)
					neis[nneis++] = link->ref;
			}
		}

		// If any of the neighbour polygons is within the next few polygons
		// in the path, short cut to that polygon directly.
		static const int maxLookAhead = 6;
		int cut = 0;
		for (int i = std::min(maxLookAhead, npath) - 1; i > 1 && cut == 0; i--) {
			for (int j = 0; j < nneis; j++)
			{
				if (path[i] == neis[j]) {
					cut = i;
					break;
				}
			}
		}
		if (cut > 1)
		{
			int offset = cut - 1;
			npath -= offset;
			for (int i = 1; i < npath; i++)
				path[i] = path[i + offset];
		}

		return npath;
	}

	static void dtVsub(float* dest, const float* v1, const float* v2)
	{
		dest[0] = v1[0] - v2[0];
		dest[1] = v1[1] - v2[1];
		dest[2] = v1[2] - v2[2];
	}

	static float dtVlen(const float* v)
	{
		return std::sqrtf(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
	}

	static float dtVdot(const float* v1, const float* v2)
	{
		return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
	}

	static void dtVmad(float* dest, const float* v1, const float* v2, const float s)
	{
		dest[0] = v1[0] + v2[0] * s;
		dest[1] = v1[1] + v2[1] * s;
		dest[2] = v1[2] + v2[2] * s;
	}

#define dtVcopy(x,y) memcpy(x, y, sizeof(float)*3)

	std::vector<glm::vec3> CTiledNavMesh::findPath(const glm::vec3& startPos, const glm::vec3& endPos)
	{

		/*
		enum SamplePolyFlags
		{
			SAMPLE_POLYFLAGS_WALK		= 0x01,		// Ability to walk (ground, grass, road)
			SAMPLE_POLYFLAGS_SWIM		= 0x02,		// Ability to swim (water).
			SAMPLE_POLYFLAGS_DOOR		= 0x04,		// Ability to move through doors.
			SAMPLE_POLYFLAGS_JUMP		= 0x08,		// Ability to jump.
			SAMPLE_POLYFLAGS_DISABLED	= 0x10,		// Disabled polygon
			SAMPLE_POLYFLAGS_ALL		= 0xffff	// All abilities.
		};*/
		dtQueryFilter filter;


		filter.setIncludeFlags(0xffff ^ 0x10);
		filter.setExcludeFlags(0);

		const float halfExtent[3] = { 2,4,2 };

		const float startP[3] = { startPos.x, startPos.z, startPos.y };
		dtPolyRef startRef;
		m_navQuery->findNearestPoly(startP, halfExtent, &filter, &startRef, 0);

		const float endP[3] = { endPos.x, endPos.z, endPos.y };
		dtPolyRef endRef;
		m_navQuery->findNearestPoly(endP, halfExtent, &filter, &endRef, 0);

		if (!startRef || !endRef) {
			return {};
		}

		constexpr uint32_t MAX_POLYS = 256;
		constexpr int MAX_SMOOTH = 2048;

		dtPolyRef m_polys[MAX_POLYS];
		dtPolyRef m_parent[MAX_POLYS];
		float m_smoothPath[MAX_SMOOTH * 3];

		int m_npolys = 0;

		m_navQuery->findPath(startRef, endRef, startP, endP, &filter, m_polys, &m_npolys, MAX_POLYS);

		if (!m_npolys) {
			return {};
		}

		// Iterate over the path to find smooth path on the detail mesh surface.
		dtPolyRef polys[MAX_POLYS];
		memcpy(polys, m_polys, sizeof(dtPolyRef) * m_npolys);
		int npolys = m_npolys;

		float iterPos[3], targetPos[3];
		m_navQuery->closestPointOnPoly(startRef, startP, iterPos, 0);
		m_navQuery->closestPointOnPoly(polys[npolys - 1], endP, targetPos, 0);

		static const float STEP_SIZE = 0.5f;
		static const float SLOP = 0.01f;

		int m_nsmoothPath = 0;

		dtVcopy(&m_smoothPath[m_nsmoothPath * 3], iterPos);
		m_nsmoothPath++;

		// Move towards target a small advancement at a time until target reached or
		// when ran out of memory to store the path.
		while (npolys && m_nsmoothPath < MAX_SMOOTH)
		{
			// Find location to steer towards.
			float steerPos[3];
			unsigned char steerPosFlag;
			dtPolyRef steerPosRef;

			if (!getSteerTarget(m_navQuery, iterPos, targetPos, SLOP,
				polys, npolys, steerPos, steerPosFlag, steerPosRef))
				break;

			bool endOfPath = (steerPosFlag & DT_STRAIGHTPATH_END) ? true : false;
			bool offMeshConnection = (steerPosFlag & DT_STRAIGHTPATH_OFFMESH_CONNECTION) ? true : false;

			// Find movement delta.
			float delta[3], len;
			dtVsub(delta, steerPos, iterPos);
			len = sqrtf(dtVdot(delta, delta));
			// If the steer target is end of path or off-mesh link, do not move past the location.
			if ((endOfPath || offMeshConnection) && len < STEP_SIZE)
				len = 1;
			else
				len = STEP_SIZE / len;
			float moveTgt[3];
			dtVmad(moveTgt, iterPos, delta, len);

			// Move
			float result[3];
			dtPolyRef visited[16];
			int nvisited = 0;
			m_navQuery->moveAlongSurface(polys[0], iterPos, moveTgt, &filter,
				result, visited, &nvisited, 16);

			npolys = dtMergeCorridorStartMoved(polys, npolys, MAX_POLYS, visited, nvisited);
			npolys = fixupShortcuts(polys, npolys, m_navQuery);

			float h = 0;
			m_navQuery->getPolyHeight(polys[0], result, &h);
			result[1] = h;
			dtVcopy(iterPos, result);

			// Handle end of path and off-mesh links when close enough.
			if (endOfPath && inRange(iterPos, steerPos, SLOP, 1.0f))
			{
				// Reached end of path.
				dtVcopy(iterPos, targetPos);
				if (m_nsmoothPath < MAX_SMOOTH)
				{
					dtVcopy(&m_smoothPath[m_nsmoothPath * 3], iterPos);
					m_nsmoothPath++;
				}
				break;
			}
			else if (offMeshConnection && inRange(iterPos, steerPos, SLOP, 1.0f))
			{
				// Reached off-mesh connection.
				float startPos[3], endPos[3];

				// Advance the path up to and over the off-mesh connection.
				dtPolyRef prevRef = 0, polyRef = polys[0];
				int npos = 0;
				while (npos < npolys && polyRef != steerPosRef)
				{
					prevRef = polyRef;
					polyRef = polys[npos];
					npos++;
				}
				for (int i = npos; i < npolys; ++i)
					polys[i - npos] = polys[i];
				npolys -= npos;

				// Handle the connection.
				dtStatus status = m_navMesh->getOffMeshConnectionPolyEndPoints(prevRef, polyRef, startPos, endPos);
				if (dtStatusSucceed(status))
				{
					if (m_nsmoothPath < MAX_SMOOTH)
					{
						dtVcopy(&m_smoothPath[m_nsmoothPath * 3], startPos);
						m_nsmoothPath++;
						// Hack to make the dotted path not visible during off-mesh connection.
						if (m_nsmoothPath & 1)
						{
							dtVcopy(&m_smoothPath[m_nsmoothPath * 3], startPos);
							m_nsmoothPath++;
						}
					}
					// Move position at the other side of the off-mesh link.
					dtVcopy(iterPos, endPos);
					float eh = 0.0f;
					m_navQuery->getPolyHeight(polys[0], iterPos, &eh);
					iterPos[1] = eh;
				}
			}

			// Store results.
			if (m_nsmoothPath < MAX_SMOOTH)
			{
				dtVcopy(&m_smoothPath[m_nsmoothPath * 3], iterPos);
				m_nsmoothPath++;
			}
		}
		std::vector<glm::vec3> result;

		for (int i = 0; i < m_nsmoothPath*3; i+= 3) {
			result.push_back({ m_smoothPath[i + 0],m_smoothPath[i + 2] ,m_smoothPath[i + 1] });
		}

		return result;
	}

	bool CTiledNavMesh::getSteerTarget(dtNavMeshQuery* navQuery, const float* startPos, const float* endPos, const float minTargetDist, const dtPolyRef* path, const int pathSize, float* steerPos, unsigned char& steerPosFlag, dtPolyRef& steerPosRef, float* outPoints /*= 0*/, int* outPointCount /*= 0*/)
	{
		// Find steer target.
		static const int MAX_STEER_POINTS = 3;
		float steerPath[MAX_STEER_POINTS * 3];
		unsigned char steerPathFlags[MAX_STEER_POINTS];
		dtPolyRef steerPathPolys[MAX_STEER_POINTS];
		int nsteerPath = 0;
		navQuery->findStraightPath(startPos, endPos, path, pathSize,
			steerPath, steerPathFlags, steerPathPolys, &nsteerPath, MAX_STEER_POINTS);
		if (!nsteerPath)
			return false;

		if (outPoints && outPointCount)
		{
			*outPointCount = nsteerPath;
			for (int i = 0; i < nsteerPath; ++i) {
				memcpy(&outPoints[i * 3], &steerPath[i * 3], sizeof(float) * 3);
			}
		}


		// Find vertex far enough to steer to.
		int ns = 0;
		while (ns < nsteerPath)
		{
			// Stop at Off-Mesh link or when point is further than slop away.
			if ((steerPathFlags[ns] & DT_STRAIGHTPATH_OFFMESH_CONNECTION) ||
				!inRange(&steerPath[ns * 3], startPos, minTargetDist, 1000.0f))
				break;
			ns++;
		}
		// Failed to find good point to steer to.
		if (ns >= nsteerPath)
			return false;

		memcpy(steerPos, &steerPath[ns * 3], sizeof(float) * 3);
		steerPos[1] = startPos[1];
		steerPosFlag = steerPathFlags[ns];
		steerPosRef = steerPathPolys[ns];

		return true;
	}
}