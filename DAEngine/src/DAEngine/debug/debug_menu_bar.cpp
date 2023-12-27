#include "dapch.h"
#include "debug_menu_bar.h"

#ifdef DA_DEBUG
#include <imgui.h>

namespace da::debug
{

	std::unordered_map<CHashString, std::pair<bool*, std::function<void()>>> CDebugMenuBar::s_refs;

	void CDebugMenuBar::initialize()
	{

	}

	void CDebugMenuBar::update()
	{
		if (ImGui::BeginMainMenuBar()) {

			if (ImGui::BeginMenu("Debug")) {

				
				for (const std::pair<CHashString, std::pair<bool*, std::function<void()>>>& kv : s_refs)
				{
					ImGui::MenuItem(kv.first.c_str(), "", kv.second.first);
				}

				
				ImGui::EndMenu();
			}

			for (const std::pair<CHashString, std::pair<bool*, std::function<void()>>>& kv : s_refs)
			{
				if (*kv.second.first) {
					kv.second.second();
				}
			}

			ImGui::EndMainMenuBar();
		}
	}

	void CDebugMenuBar::shutdown()
	{
		s_refs = {};
	}

	void CDebugMenuBar::register_debug(CHashString id, bool* b, std::function<void()> func)
	{
		s_refs[id] = { b, func };
	}

	void CDebugMenuBar::unregister_debug(CHashString id)
	{
		const std::unordered_map< CHashString, std::pair<bool*, std::function<void()>>>::iterator& it = s_refs.find(id);

		if (it == s_refs.end())
		{
			return;
		}

		s_refs.erase(it);
	}
	

}

#endif