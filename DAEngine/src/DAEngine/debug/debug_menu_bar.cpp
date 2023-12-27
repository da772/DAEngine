#include "dapch.h"
#include "debug_menu_bar.h"

#ifdef DA_DEBUG
#include <imgui.h>

namespace da::debug
{
	std::unordered_map<CHashString, std::unordered_map<CHashString, std::pair<bool*, std::function<void()>>>> CDebugMenuBar::s_refs;

	void CDebugMenuBar::initialize()
	{

	}

	void CDebugMenuBar::update()
	{

		if (ImGui::BeginMainMenuBar()) {

			for (const std::pair<CHashString, std::unordered_map<CHashString, std::pair<bool*, std::function<void()>>>>& map : s_refs)
			{
				if (ImGui::BeginMenu(map.first.c_str())) {
					for (const std::pair<CHashString, std::pair<bool*, std::function<void()>>>& kv : map.second)
					{
						ImGui::MenuItem(kv.first.c_str(), "", kv.second.first);
					}
					ImGui::EndMenu();
				}
			}

			for (const std::pair<CHashString,  std::unordered_map<CHashString, std::pair<bool*, std::function<void()>>>>& map : s_refs)
			{
				for (const std::pair<CHashString, std::pair<bool*, std::function<void()>>>& kv : map.second)
				{
					if (*kv.second.first) {
						kv.second.second();
					}
				}
			}

			ImGui::EndMainMenuBar();
		}
	}

	void CDebugMenuBar::shutdown()
	{
		s_refs = {};
	}

	void CDebugMenuBar::register_debug(CHashString group, CHashString id, bool* b, std::function<void()> func)
	{
		s_refs[group][id] = { b, func };
	}

	void CDebugMenuBar::unregister_debug(CHashString group, CHashString id)
	{
		const auto& it1 = s_refs.find(group);

		if (it1 == s_refs.end())
		{
			return;
		}
		

		const std::unordered_map< CHashString, std::pair<bool*, std::function<void()>>>::iterator& it2 = it1->second.find(group);

		if (it2 == it1->second.end())
		{
			return;
		}

		it1->second.erase(it2);

		if (it1->second.empty())
		{
			s_refs.erase(it1);
		}
	}


}

#endif