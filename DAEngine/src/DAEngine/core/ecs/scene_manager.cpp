

#include "scene_manager.h"
#include "entity.h"
#include <format> 


namespace da {

	CScene* CSceneManager::s_scene;
	bool CSceneManager::s_initialized = false;

	void CSceneManager::initialize()
	{
		s_initialized = true;
	}

	CScene* CSceneManager::getScene()
	{
		return s_scene;
	}

	void CSceneManager::setScene(CScene* scene)
	{
		if (s_scene) {
			if (s_initialized) s_scene->shutdown();
			delete s_scene;
			s_scene = nullptr;
		}

		s_scene = scene;
		if (s_initialized && s_scene) s_scene->initialize();
	}

	void CSceneManager::shutdown()
	{
		if (s_scene) {
			if (s_initialized) s_scene->shutdown();
			delete s_scene;
		}

		s_initialized = false;
	}

}


