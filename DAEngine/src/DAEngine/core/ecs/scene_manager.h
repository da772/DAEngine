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
		static void initialize();
		static void shutdown();
#ifdef DA_REVIEW
		static void renderDebug();
		static void renderCameraDebug();
#endif

	private:
		static CScene* s_scene;
#ifdef DA_REVIEW
		static bool s_showDebug;
		static bool s_showCameraDebug;
#endif

	};

}



