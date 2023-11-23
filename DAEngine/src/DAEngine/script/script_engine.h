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
		static void* loadScript(const char* path);
		static int getScript(const char* path, bool cacheResult);
		static bool hasScript(const CHashString& hash);
		static void unloadScript(const CHashString& hash);
        static void clearAll();


	private:
		static void registerFunctions();
		static void readTable(int idx);

	private:
		lua_State* m_state;


	private:
		static CScriptEngine* s_instance;
		static std::unordered_map<uint32_t, int> s_scriptMap;

	};

}
