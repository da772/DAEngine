#include "dapch.h"

#include "script_engine.h"
#include "asset/asset.h"
#include <chrono>

extern "C" {
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}


namespace da::script
{
	CScriptEngine* CScriptEngine::s_instance = nullptr;

	extern "C" int lua_requires(lua_State * L)
	{
		const char* path = luaL_checkstring(L, 1);

		std::string filePath = "scripts/" + std::string(path) + ".lua";

		LOG_INFO(ELogChannel::Script, "Attempting to require script: %s", filePath.c_str());

		CAsset asset(filePath);
		int load_stat = luaL_loadbuffer(L, asset.data(), asset.size(), path);
		if (lua_pcall(L, 0, 1, 0) != LUA_OK) {
			LOG_ERROR(ELogChannel::Script, "%s failed: %s", filePath.c_str(), lua_tostring(L, -1));
		}

		return 1;
	}

	extern "C" int lua_print(lua_State * L)
	{
		int top = lua_gettop(L);

		std::string str;

		for (unsigned index = 0; index <= top; index++)
		{
			int type = lua_type(L, index);
			switch (type)
			{
				// booleans
			case LUA_TBOOLEAN:
				str = str + (lua_toboolean(L, index) ? "true" : "false");
				break;

				// numbers
			case LUA_TNUMBER:
				str = str + std::to_string(lua_tonumber(L, index));
				break;

				// strings
			case LUA_TSTRING:
				str = str + lua_tostring(L, index);
				break;

				// other
			default:
				//str = str + lua_typename(L, type);
				break;
			}
		}

		LOG_DEBUG(ELogChannel::Script, "%s", str.c_str());
		return 0;
	}

	void CScriptEngine::initialize()
	{
		if (s_instance) return;
		s_instance = new CScriptEngine();

		s_instance->m_state = lua_open();
		luaL_openlibs(s_instance->m_state);

		register_functions();
	}

	void CScriptEngine::register_functions()
	{
		lua_register(s_instance->m_state, "require", lua_requires);
		lua_register(s_instance->m_state, "print", lua_print);
	}

	void CScriptEngine::load_script(const char* path)
	{
		CAsset asset(path);
		LOG_INFO(ELogChannel::Script, "Loading: %s", path);
		//LOG_INFO(ELogChannel::Script, "%s", asset.data());
		int load_stat = luaL_loadbuffer(s_instance->m_state, asset.data(), asset.size(), path);
		auto t1 = std::chrono::high_resolution_clock::now();
		if (lua_pcall(s_instance->m_state, 0, 0, 0) != LUA_OK) {
			LOG_ERROR(ELogChannel::Script, "%s failed: %s", path, lua_tostring(s_instance->m_state, -1));
		}
		auto t2 = std::chrono::high_resolution_clock::now();
		auto ms_int = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);

		LOG_INFO(ELogChannel::Script, "SCRIPT %s, EXECUTED IN %llums", path, ms_int.count());
	}

	void CScriptEngine::shutdown()
	{
		if (!s_instance) return;

		lua_close(s_instance->m_state);
	}

}