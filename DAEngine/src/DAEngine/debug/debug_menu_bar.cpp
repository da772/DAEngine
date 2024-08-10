#include "core/core.h"
#include "debug_menu_bar.h"

#ifdef DA_REVIEW
#include <imgui.h>

namespace da::debug
{
	std::map<CHashString, std::map<CHashString, std::pair<bool*, std::function<void()>>>> CDebugMenuBar::s_refs;
	std::vector < std::pair<CHashString, CHashString>> CDebugMenuBar::s_dirty;
	bool CDebugMenuBar::s_processing = false;

	void CDebugMenuBar::initialize()
	{

	}

	void CDebugMenuBar::update()
	{
		s_processing = true;
		if (ImGui::BeginMainMenuBar()) {

			for (const std::pair<CHashString, std::map<CHashString, std::pair<bool*, std::function<void()>>>>& map : s_refs)
			{
				if (ImGui::BeginMenu(map.first.c_str())) {
					for (const std::pair<CHashString, std::pair<bool*, std::function<void()>>>& kv : map.second)
					{
						ImGui::MenuItem(kv.first.c_str(), "", kv.second.first);
					}
					ImGui::EndMenu();
				}
			}

			for (const std::pair<CHashString,  std::map<CHashString, std::pair<bool*, std::function<void()>>>>& map : s_refs)
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
		s_processing = false;

		for (size_t i = 0; i < s_dirty.size(); i++) {
			unregister_debug(s_dirty[i].first, s_dirty[i].second);
		}
		s_dirty = {};
	}

	void CDebugMenuBar::shutdown()
	{
		s_refs = {};
		s_dirty = {};
	}

	void CDebugMenuBar::register_debug(CHashString group, CHashString id, bool* b, std::function<void()> func)
	{
		s_refs[group][id] = { b, func };
	}

	void CDebugMenuBar::unregister_debug(CHashString group, CHashString id)
	{

		if (s_processing) {
			s_dirty.push_back({ group, id });
			return;
		}

		const auto& it1 = s_refs.find(group);

		if (it1 == s_refs.end())
		{
			return;
		}

		const std::map< CHashString, std::pair<bool*, std::function<void()>>>::iterator& it2 = it1->second.find(id);

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