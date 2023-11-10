#pragma once

#include <unordered_map>
#include "daengine/asset/asset.h"

extern "C" {
	typedef struct lua_State lua_State;
}

namespace da::script
{

	class CScriptEngine {

	public:
		static void initialize();
		static void shutdown();
		static void* load_script(const char* path);
		static int get_script(const char* path, bool cacheResult);
		static bool has_script(const CHashString& hash);
		static void unload_script(const CHashString& hash);


	private:
		static void register_functions();
		static void read_table(int idx);

	private:
		lua_State* m_state;


	private:
		static CScriptEngine* s_instance;
		static std::unordered_map<uint32_t, int> s_scriptMap;

	};

}
