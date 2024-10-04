#pragma once
#include "nav_mesh.h"

namespace da
{
	class CNavDrawer;
	class CNavCtx;

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
		static std::vector<INavMesh*> s_navMeshes;
		static bool s_initialized;
#ifdef DA_REVIEW
		static bool s_debugRender;
#endif
	};
}