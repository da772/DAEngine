#pragma once

#include <unordered_map>
#include "daengine/asset/asset.h"

extern "C" {
	typedef struct lua_State lua_State;
}

namespace sol {
	class state_view;
}

#define LOG_SASSERT(cond, L, msg, ...) LOG_ASSERT(cond, ELogChannel::Script, "%s - Printing Script callstack: %s", msg, ##__VA_ARGS__, !cond ? CScriptEngine::getCallstack(L).c_str() : "")

namespace da::script
{
	class CScriptEngine {

	public:
		static void initialize();
#ifdef DA_REVIEW
		static void update();
#endif
		static void shutdown();
		static void* loadScript(const char* path);
		static int getScript(const char* path, bool cacheResult);
		static bool hasScript(const CHashString& hash);
		static void unloadScript(const CHashString& hash);
        static void clearAll();
		static lua_State* getState();
		static sol::state_view* getStateView();

		static std::string getCallstack(lua_State* L);

	private:
		static void registerFunctions();
		static void readTable(int idx);

	private:
		lua_State* m_state;
		sol::state_view* m_stateView;


	private:
		static CScriptEngine* s_instance;
		static std::unordered_map<uint32_t, int> s_scriptMap;

	};

}
