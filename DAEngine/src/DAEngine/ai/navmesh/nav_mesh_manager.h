#pragma once
#include "nav_mesh.h"

namespace da::ai
{
	class CNavMeshManager
	{
	public:
#ifdef DA_REVIEW
		static CNavDrawer& getDrawer();
#endif
		static CNavCtx& getCtx();
		static void addNavMesh(INavMesh& navMesh);
		static INavMesh* getNavMesh(uint32_t index = 0);

		static void initialize();
		static void update(float dt);
		static void shutdown();
		

	private:
		static CNavCtx s_ctx;
		static std::vector<INavMesh*> s_navMeshes;
		static bool s_initialized;
#ifdef DA_REVIEW
		static CNavDrawer s_drawer;
		static bool s_debugRender;
#endif
	};
}