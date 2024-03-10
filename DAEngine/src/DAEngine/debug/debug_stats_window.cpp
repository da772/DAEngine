#include "dapch.h"
#include "debug_stats_window.h"

#ifdef DA_REVIEW

#include <imgui.h>
#include "debug_menu_bar.h"
#include <core/time.h>

double da::debug::CDebugStatsWindow::s_gpuTime;
double da::debug::CDebugStatsWindow::s_cpuTime;
uint64_t da::debug::CDebugStatsWindow::s_drawCalls;
double da::debug::CDebugStatsWindow::s_waitTime;
uint64_t da::debug::CDebugStatsWindow::s_gpuMemMax;
uint64_t da::debug::CDebugStatsWindow::s_gpuMem;
std::unordered_map<CHashString, double> da::debug::CDebugStatsWindow::s_viewTimes;
bool da::debug::CDebugStatsWindow::s_showing = true;
bool da::debug::CDebugStatsWindow::s_showViewTimes = false;


void da::debug::CDebugStatsWindow::initialize()
{
	da::debug::CDebugMenuBar::register_debug(HASHSTR("Renderer"), HASHSTR("Toggle Stats"), &s_showing, update);
}

void da::debug::CDebugStatsWindow::update()
{
	ImGui::SetNextWindowBgAlpha(.5f);
	ImGui::SetNextWindowPos({ 0.f, 24.5f });
	ImGui::SetNextWindowSize({ -1, -1 });
	if (ImGui::Begin("Debug Stats", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus)) {

		ImGui::Text("Debug Stats");
		ImGui::Separator();
		ImGui::Text("Time Stats | Show View Stats");
		ImGui::SameLine();
		ImGui::Checkbox("##viewStates", &s_showViewTimes);
		ImGui::Separator();
		ImGui::Text("%d FPS", da::core::CTime::getFps());
		ImGui::Text("App   Time:    %f ms", 1000.0/(double)da::core::CTime::getFps());
		ImGui::Text("Cpu   Time:    %f ms", std::abs(1000.0 / (double)da::core::CTime::getFps() - s_gpuTime / 1000.0));
		ImGui::Text("Gpu   Time:    %f ms", s_gpuTime/1000.0);
		ImGui::Text("Wait  Time:    %f ms", s_waitTime/1000.0);

		if (s_showViewTimes) {
			for (const std::pair<CHashString, double>& kv : s_viewTimes) {
				ImGui::Text("%s:", kv.first.c_str());
				ImGui::SameLine(150);
				ImGui::Text("%f ms", kv.second);
			}
		}

		ImGui::Separator();
		ImGui::Text("Renderer Stats");
		ImGui::Separator();
		ImGui::Text("Draw Calls:   %d", s_drawCalls);
		ImGui::Text("Gpu Memory:   %.2f/%.2f GB (%.2f%%)", s_gpuMem / (double)1e6, s_gpuMemMax / (double)1e6, (double)s_gpuMemMax/(double)s_gpuMem);
	}

	ImGui::End();
}

void da::debug::CDebugStatsWindow::shutdown()
{
	da::debug::CDebugMenuBar::unregister_debug(HASHSTR("Renderer"), HASHSTR("Toggle Stats"));
}



#endif