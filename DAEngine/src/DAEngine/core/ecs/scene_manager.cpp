#include "dapch.h"

#include "scene_manager.h"

#ifdef DA_DEBUG
#include <imgui.h>
#include "entity.h"
#include "debug/debug_menu_bar.h"
#include "daengine/core/graphics/camera.h"
#endif
#include <format> 


namespace da::core {

	CScene* CSceneManager::s_scene;
#ifdef DA_DEBUG
	bool CSceneManager::s_showDebug = false;
	bool CSceneManager::s_showCameraDebug = false;
#endif

	void CSceneManager::initialize()
	{
#ifdef DA_DEBUG
		da::debug::CDebugMenuBar::register_debug(HASHSTR("ECS"), HASHSTR("CSceneManager"), &s_showDebug, renderDebug);
		da::debug::CDebugMenuBar::register_debug(HASHSTR("Renderer"), HASHSTR("Camera"), &s_showCameraDebug, renderCameraDebug);
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
					char nameBuffer[1024];
					sprintf_s(nameBuffer, sizeof(nameBuffer), "%s (%s)", e->getTag().c_str(), e->getId().c_str());
					if (ImGui::CollapsingHeader(nameBuffer)) {
						ImGui::Indent();

						{
							glm::vec3 p = e->getTransform().position();
							float pos[] = { p.x, p.y, p.z };
							ImGui::Text("Position: ");
							ImGui::SameLine();
							if (ImGui::InputFloat3((std::string("##Position") + std::string(e->getId().c_str())).c_str(), pos, "%.3f")) {
								p = { pos[0], pos[1], pos[2] };
								e->getTransform().setPosition(p);
							}
						}

						{
							glm::vec3 r = e->getTransform().rotation();
							float rot[] = { r.x ,r.y, r.z };
							ImGui::Text("Rotation");
							ImGui::SameLine();
							if (ImGui::InputFloat3((std::string("##Rotation") + std::string(e->getId().c_str())).c_str(), rot, "%.3f")) {
								r = { rot[0], rot[1], rot[2] };
								e->getTransform().setRotation(r);
							}
						}

						e->debugRender();
						ImGui::Unindent();
					}
					
				}
			}

			ImGui::End();


		}
	}

	void CSceneManager::renderCameraDebug()
	{
		if (da::core::CCamera* camera = CCamera::getCamera())
		{
			if (ImGui::Begin("Camera Debug", &s_showCameraDebug)) {
				glm::vec3 pos = camera->position();
				float pPos[] = { pos.x, pos.y, pos.z };
				if (ImGui::InputFloat3("Position", pPos)) {
					glm::vec3 newPos = { pPos[0], pPos[1], pPos[2] };
					camera->setPosition(newPos);
				}
				ImGui::InputFloat("Exposure", &camera->exposure, .1f, 1.f);
				ImGui::InputFloat("FOV", &camera->fov, 1.f);
			}

			ImGui::End();
		}
	}
#endif

	void CSceneManager::shutdown()
	{
#ifdef DA_DEBUG
		da::debug::CDebugMenuBar::unregister_debug(HASHSTR("ECS"), HASHSTR("CSceneManager"));
		da::debug::CDebugMenuBar::unregister_debug(HASHSTR("Renderer"), HASHSTR("Camera"));
#endif
	}

}


