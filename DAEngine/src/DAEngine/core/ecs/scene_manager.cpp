

#include "scene_manager.h"
#include "entity.h"

#ifdef DA_REVIEW
#include <imgui.h>
//#include "debug/debug_menu_bar.h"
#endif
#include <format> 


namespace da::core {

	CScene* CSceneManager::s_scene;
	bool CSceneManager::s_initialized = false;
#ifdef DA_REVIEW
	bool CSceneManager::s_showDebug = false;
	bool CSceneManager::s_showECSDebug = false;
	bool CSceneManager::s_showCameraDebug = false;
#endif

	void CSceneManager::initialize()
	{
#ifdef DA_REVIEW
		//da::debug::CDebugMenuBar::register_debug(HASHSTR("ECS"), HASHSTR("CSceneManager"), &s_showDebug, renderDebug);
		//da::debug::CDebugMenuBar::register_debug(HASHSTR("ECS"), HASHSTR("Component View"), &s_showECSDebug, renderECSDebug);
#endif
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
#ifdef DA_REVIEW
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
							glm::vec3 r = e->getTransform().rotationEuler();
							float rot[] = { r.x ,r.y, r.z };
							ImGui::Text("Rotation");
							ImGui::SameLine();
							if (ImGui::InputFloat3((std::string("##Rotation") + std::string(e->getId().c_str())).c_str(), rot, "%.3f")) {
								r = { rot[0], rot[1], rot[2] };
								e->getTransform().setRotation(r);
							}
						}
						

						{
							glm::vec3 s = e->getTransform().scale();
							float scale[] = { s.x ,s.y, s.z };
							ImGui::Text("Scale");
							ImGui::SameLine();
							if (ImGui::InputFloat3((std::string("##scale") + std::string(e->getId().c_str())).c_str(), scale, "%.3f")) {
								s = { scale[0], scale[1], scale[2] };
								e->getTransform().setScale(s);
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


	void CSceneManager::renderECSDebug()
	{
		if (CScene* scene = getScene()) {

			if (ImGui::Begin("ECS Debug", &s_showECSDebug)) {

				for (const std::pair<CHashString, FComponentContainer>& kv : scene->getComponentContainers())
				{
					constexpr int colSize = 5;
					if (ImGui::CollapsingHeader(kv.first.c_str()))
					{
						if (kv.second.getCount())
						{
							ImGui::BeginTable(kv.first.c_str(), colSize, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg);
							for (size_t i = 0; i < kv.second.getCount() * kv.second.getComponentSize(); i+= 2) {
								if (i % colSize == 0) {
									ImGui::TableNextRow();
								}
								ImGui::TableSetColumnIndex(i % colSize);
								if (i % kv.second.getComponentSize() == 0) {
									ImU32 cell_bg_color = ImGui::GetColorU32(ImVec4(0.3f, 0.3f, 0.7f, 0.65f));
									ImGui::TableSetBgColor(ImGuiTableBgTarget_CellBg, cell_bg_color);
								}
								ImGui::Text("0x%04x", *((uint16_t*)kv.second.getComponentAtIndexUnscaled(i)));
							}

							ImGui::EndTable();
						}
					}
				}

			}

			ImGui::End();
		}
	}

#endif

	void CSceneManager::shutdown()
	{
#ifdef DA_REVIEW
		//da::debug::CDebugMenuBar::unregister_debug(HASHSTR("ECS"), HASHSTR("CSceneManager"));
		//da::debug::CDebugMenuBar::unregister_debug(HASHSTR("Renderer"), HASHSTR("Camera"));
#endif
		if (s_scene) {
			if (s_initialized) s_scene->shutdown();
			delete s_scene;
		}

		s_initialized = false;
	}

}


