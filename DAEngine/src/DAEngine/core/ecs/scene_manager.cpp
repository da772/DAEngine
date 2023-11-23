#include "dapch.h"

#include "scene_manager.h"

namespace da::core {

	CScene* CSceneManager::s_scene;

	CScene* CSceneManager::getScene()
	{
		return s_scene;
	}

	void CSceneManager::setScene(CScene* scene)
	{
		s_scene = scene;
	}
}


