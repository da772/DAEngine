#include "dapch.h"
#include "nav_mesh_manager.h"

#include "debug/debug_menu_bar.h"

namespace da::ai
{

#ifdef DA_REVIEW
	da::ai::CNavDrawer CNavMeshManager::s_drawer;
	bool CNavMeshManager::s_debugRender;
#endif
	
	std::vector<INavMesh*> CNavMeshManager::s_navMeshes;
	da::ai::CNavCtx CNavMeshManager::s_ctx;
	bool CNavMeshManager::s_initialized;
	
#ifdef DA_REVIEW
	da::ai::CNavDrawer& CNavMeshManager::getDrawer()
	{
		return s_drawer;
	}
#endif

	void CNavMeshManager::initialize()
	{
		s_initialized = true;
		
#ifdef DA_REVIEW
		da::debug::CDebugMenuBar::register_debug(HASHSTR("Ai"), HASHSTR("NavMesh"), &s_debugRender, []() { });
#endif

		for (INavMesh* m : s_navMeshes)
		{
			m->initialize();
		}
	}

	void CNavMeshManager::addNavMesh(INavMesh& navMesh)
	{
		s_navMeshes.push_back(&navMesh);
		if (s_initialized) navMesh.initialize();
	}

	void CNavMeshManager::shutdown()
	{
#ifdef DA_REVIEW
		da::debug::CDebugMenuBar::unregister_debug(HASHSTR("Ai"), HASHSTR("NavMesh"));
#endif

		for (INavMesh* m : s_navMeshes)
		{
			m->shutdown();
		}

		s_navMeshes = {};
	}

	void CNavMeshManager::update(float dt)
	{
		for (INavMesh* m : s_navMeshes)
		{
			m->update(dt);
#ifdef DA_REVIEW
			if (s_debugRender) {
				m->debugRender();
			}
#endif
		}
	}

	da::ai::CNavCtx& CNavMeshManager::getCtx()
	{
		return s_ctx;
	}

	da::ai::INavMesh* CNavMeshManager::getNavMesh(uint32_t index /*= 0*/)
	{
		ASSERT(index < s_navMeshes.size())
		return s_navMeshes[index];
	}

}