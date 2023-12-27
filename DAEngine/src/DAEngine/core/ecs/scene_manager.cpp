#include "dapch.h"

#include "scene_manager.h"

#ifdef DA_DEBUG
#include <imgui.h>
#include "entity.h"
#include "debug/debug_menu_bar.h"
#endif


namespace da::core {

	CScene* CSceneManager::s_scene;
#ifdef DA_DEBUG
	bool CSceneManager::s_showDebug = false;
#endif

	void CSceneManager::initialize()
	{
#ifdef DA_DEBUG
		da::debug::CDebugMenuBar::register_debug(HASHSTR("ECS"), HASHSTR("CSceneManager"), &s_showDebug, renderDebug);
#endif
	}

	CScene* CSceneManager::getScene()
	{
		return s_scene;
	}

	void CSceneManager::setScene(CScene* scene)
	{
		s_scene = scene;
	}
#ifdef DA_DEBUG
	void CSceneManager::renderDebug()
	{
		if (CScene* scene = getScene()) {

			if (ImGui::Begin("Scene Debug", &s_showDebug)) {

				for (CEntity* e : scene->getEntities()) {
					if (ImGui::CollapsingHeader(e->getId().c_str())) {
						ImGui::Indent();
						glm::vec3 p = e->getTransform().getPosition();
						float pos[] = { p.x, p.y, p.z };
						
						if (ImGui::InputFloat3((std::string("Position##") + std::string(e->getId().c_str())).c_str(), pos, "%.3f")) {
							p = { pos[0], pos[1], pos[2] };
							e->getTransform().setPosition(p);
						}

						
						e->debugRender();
						ImGui::Unindent();
					}
					
				}
			}

			ImGui::End();


		}
	}

	void CSceneManager::shutdown()
	{
#ifdef DA_DEBUG
		da::debug::CDebugMenuBar::unregister_debug(HASHSTR("ECS"), HASHSTR("CSceneManager"));
#endif
	}

#endif

}


