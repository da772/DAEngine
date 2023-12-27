#pragma once


#ifdef DA_DEBUG

#include <functional>

namespace da::debug
{
	class CDebugMenuBar
	{
	public:
		static void initialize();
		static void update();
		static void shutdown();

		static void register_debug(CHashString group, CHashString id, bool* b, std::function<void()> func);
		static void unregister_debug(CHashString group, CHashString id);


	private:
		static std::unordered_map<CHashString, std::unordered_map<CHashString, std::pair<bool*, std::function<void()>>>> s_refs;

	};
}

#endif