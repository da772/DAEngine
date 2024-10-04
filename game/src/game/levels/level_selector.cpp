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
		da::CDebugMenuBar::unregister_debug(HASHSTR("Level"), HASHSTR("End"));
#endif
	}

	m_level = id;
	m_levelInit = false;

#ifdef DA_REVIEW
	da::CDebugMenuBar::register_debug(HASHSTR("Level"), HASHSTR("End"), &m_levelDebug, [&] { m_levelDebug = false; startLevel(INVALID_LEVEL_ID); });
#endif
}

void CLevelSelector::update(float dt)
{
	if (!m_showLoadScreen && m_level != INVALID_LEVEL_ID)
	{

		if (!m_levelInit)
		{
			m_levels[m_level]->initialize();
			m_levelInit = true;
			return;
		}

		m_levels[m_level]->update(dt);
	}


	if (m_showLoadScreen && m_showLoadScreen < 10)
	{
		loadScreen();
		m_showLoadScreen++;
	}
	else
	{
		m_showLoadScreen = 0;
	}

#ifdef DA_REVIEW
	if (m_level == INVALID_LEVEL_ID) {
		const float width = m_window.getWindow()->getWindowData().Width;
		const float height = m_window.getWindow()->getWindowData().Height;
		const float windowWidth = 300.f;
		const float windowHeight = 350.f;
		ImGui::SetNextWindowSize({ width*1.2f, height*1.2f});
		ImGui::SetNextWindowPos({ width / 2.f - width * 1.2f / 2.f, height / 2.f - height * 1.2f / 2.f });
		ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.02f,.15f,.15f,1.f });
		ImGui::Begin("bg", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);
		ImGui::End();
		ImGui::PopStyleColor();


		ImGui::SetNextWindowSize({ windowWidth, windowHeight });
		ImGui::SetNextWindowPos({ width / 2.f - windowWidth/2.f, height / 2.f - windowHeight/2.f});
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
				ILevel* level = m_levels[i];
				ImVec2 size = { 250.f, 30.f };
				ImGui::SetCursorPosX(ImGui::GetWindowSize().x / 2.f - size.x / 2.f);
				std::string buttonId = std::string(level->getName().c_str()) + std::string("###levelSelector:") + std::to_string(i);
				if (ImGui::Button(buttonId.c_str(), size)) {
					startLevel(i);
					m_showLoadScreen = 1;
					break;
				}
			}

			ImGui::EndChild();
		}

		ImGui::End();
	}
#endif
}


void CLevelSelector::lateUpdate(float dt)
{
	if (m_level == INVALID_LEVEL_ID || !m_levelInit) return;

	m_levels[m_level]->lateUpdate(dt);
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
			da::CDebugMenuBar::unregister_debug(HASHSTR("Level"), HASHSTR("End"));
#endif
		}

		delete m_levels[i];
	}
	m_levels = {};
	m_level = INVALID_LEVEL_ID;
}

void CLevelSelector::loadScreen()
{
#ifdef DA_REVIEW
	const float width = m_window.getWindow()->getWindowData().Width;
	const float height = m_window.getWindow()->getWindowData().Height;
	const float windowWidth = 300.f;
	const float windowHeight = 350.f;
	ImGui::SetNextWindowSize({ width * 1.2f, height * 1.2f });
	ImGui::SetNextWindowPos({ width / 2.f - width * 1.2f / 2.f, height / 2.f - height * 1.2f / 2.f });
	ImGui::PushStyleColor(ImGuiCol_WindowBg, { 0.02f,.15f,.15f,1.f });
	ImGui::Begin("bg", nullptr, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoBringToFrontOnFocus);

	const char text[] = "Loading...";
	float textWidth = ImGui::CalcTextSize(text).x;
	ImGui::SetCursorPos({ width / 2.f + (textWidth/2.f), height / 2.f});
	ImGui::PushLargeFont();
	ImGui::Text(text);
	ImGui::PopFont();
	ImGui::End();
	ImGui::PopStyleColor();
	
	
#endif
}
