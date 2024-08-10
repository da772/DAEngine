#pragma once
#include "core.h"
#include "dastd.h"

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

	private:
		static CScene* s_scene;
		static bool s_initialized;

	};

}



