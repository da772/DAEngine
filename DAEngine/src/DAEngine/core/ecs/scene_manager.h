#pragma once
#include "scene_manager.h"

namespace da::core
{
	class CScene;

	class CSceneManager
	{
	public:
		static CScene* getScene();
		static void setScene(CScene* scene);

	private:
		static CScene* s_scene;

	};

}



