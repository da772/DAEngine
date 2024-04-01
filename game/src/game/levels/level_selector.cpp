#include "level_selector.h"
#include <ImGui.h>
#include <DAEngine/debug.h>

uint32_t CLevelSelector::addLevel(ILevel* level)
{
	m_levels.push_back(level);
	return m_levels.size() - 1;
}

void CLevelSelector::startLevel(uint32_t id)
{
	if (m_level != INVALID_LEVEL_ID) {
		m_levels[m_level]->shutdown();
#ifdef DA_REVIEW
		da::debug::CDebugMenuBar::unregister_debug(HASHSTR("Level"), HASHSTR("End"));
#endif
	}

	m_level = id;

	if (m_level == INVALID_LEVEL_ID) return;
	m_levels[m_level]->initialize();

#ifdef DA_REVIEW
	da::debug::CDebugMenuBar::register_debug(HASHSTR("Level"), HASHSTR("End"), &m_levelDebug, [&] { m_levelDebug = false; startLevel(INVALID_LEVEL_ID); });
#endif
}

void CLevelSelector::update(float dt)
{
#ifdef DA_REVIEW
	if (m_level == INVALID_LEVEL_ID) {
		ImGui::SetNextWindowSize({ 600, 300 });
		ImGui::SetNextWindowPos({ m_window.getWindow()->getWindowData().Width / 2.f - 300.f, m_window.getWindow()->getWindowData().Height / 2.f - 150.f });
		if (ImGui::Begin("Level Selector", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize)) {
			{
				const char* title = "Level Selector";
				ImVec2 size = ImGui::CalcTextSize(title);
				ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2.f - size.x / 2.f);
				ImGui::Text(title);
				ImGui::Separator();
			}

			ImGui::BeginChild("LevelSelectorChild", { 0.f,0.f }, false);

			for (size_t i = 0; i < m_levels.size(); i++) {
				ILevel* level = m_levels[0];
				ImVec2 size = { 250.f, 30.f };
				ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2.f - size.x / 2.f);
				//ImGui::SetCursorPosY(ImGui::GetWindowPos().y / 2.f - size.y / 2.f);
				std::string buttonId = std::string(level->getName().c_str()) + std::string("###levelSelector:") + std::to_string(i);
				if (ImGui::Button(buttonId.c_str(), size)) {
					startLevel(i);
					break;
				}
			}

			ImGui::EndChild();
		}

		ImGui::End();
	}
#endif

	if (m_level == INVALID_LEVEL_ID) return;

	m_levels[m_level]->update(dt);
}


const std::vector<ILevel*> CLevelSelector::getLevels() const
{
	return m_levels;
}


void CLevelSelector::shutdown()
{
	for (size_t i = 0; i < m_levels.size(); i++) {

		if (i == m_level) {
			m_levels[i]->shutdown();
#ifdef DA_REVIEW
			da::debug::CDebugMenuBar::unregister_debug(HASHSTR("Level"), HASHSTR("End"));
#endif
		}

		delete m_levels[i];
	}
	m_levels = {};
	m_level = INVALID_LEVEL_ID;
}
