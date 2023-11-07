#include "dapch.h"

#include "script_engine.h"
#include "asset/asset.h"
#include <chrono>
#include <algorithm>

extern "C" {
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

// TODO REMOVE
#include <imgui.h>

namespace da::script
{
	CScriptEngine* CScriptEngine::s_instance = nullptr;
	std::unordered_map<uint32_t, int> CScriptEngine::s_scriptMap;

	extern "C" int lua_requires(lua_State * L)
	{
		char buffer[1024] = "scripts/";
		const char* path = luaL_checkstring(L, 1);
		size_t pathLen = strlen(path);
		memcpy(&buffer[8], path, pathLen);
		char ext[] = ".lua";
		memcpy(&buffer[pathLen + 8], ext, sizeof(ext));
		buffer[pathLen + 8 + sizeof(ext)] = '\0';


		//std::string filePath = "scripts/" + std::string(path) + ".lua";

		//LOG_INFO(ELogChannel::Script, "Attempting to require script: %s", filePath.c_str());

		int ref = CScriptEngine::get_script(buffer, true);
		lua_rawgeti(L, LUA_REGISTRYINDEX, ref);
		//lua_call(L, 0, 1);
		return 1;
	}

	extern "C" int lua_print(lua_State * L)
	{
		int top = lua_gettop(L);

		std::string str;

		if (top == 0)
			return 0;

		for (unsigned index = 1; index <= top; index++)
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

	extern "C" int lua_imgui_begin(lua_State * L)
	{
		const char* title = luaL_checkstring(L, 2);
	
		bool result = ImGui::Begin(title);

		lua_pushboolean(L, result);
		return 1;
	}

	extern "C" int lua_imgui_end(lua_State * L)
	{
		ImGui::End();

		return 0;
	}

	extern "C" int lua_imgui_label_text(lua_State * L)
	{
		const char* title = luaL_checkstring(L, 2);
		const char* fmt = luaL_checkstring(L, 3);

		if (!title || !fmt) return 0;

		ImGui::LabelText(title, fmt);

		return 0;
	}

	void CScriptEngine::initialize()
	{
		if (s_instance) return;
		s_instance = new CScriptEngine();

		s_instance->m_state = lua_open();
		luaL_openlibs(s_instance->m_state);

		lua_pushcfunction(s_instance->m_state, luaopen_jit);
		lua_pushstring(s_instance->m_state, LUA_JITLIBNAME);
		lua_call(s_instance->m_state, 1, 0);

		register_functions();
	}

	void CScriptEngine::register_functions()
	{
		lua_register(s_instance->m_state, "require", lua_requires);
		lua_register(s_instance->m_state, "print", lua_print);
		lua_register(s_instance->m_state, "native_imgui_begin", lua_imgui_begin);
		lua_register(s_instance->m_state, "native_imgui_end", lua_imgui_end);
		lua_register(s_instance->m_state, "native_imgui_label_text", lua_imgui_label_text);
	}

	bool CScriptEngine::has_script(const CHashString& hash)
	{
		const std::unordered_map<uint32_t, int>::iterator it = s_scriptMap.find(hash.hash());

		return it != s_scriptMap.end();
	}

	int CScriptEngine::get_script(const char* path, bool cacheResult)
	{
		uint32_t hash = HASH(path);
		const std::unordered_map<uint32_t, int>::iterator it = s_scriptMap.find(hash);

		if (it == s_scriptMap.end()) {
			CAsset asset(path);
			int load_stat = luaL_loadbuffer(s_instance->m_state, asset.data(), asset.size(), path);

			if (cacheResult) {
				lua_call(s_instance->m_state, 0, 1);
			}

			s_scriptMap[hash] = luaL_ref(s_instance->m_state, LUA_REGISTRYINDEX);
			return s_scriptMap[hash];
		}

		return it->second;
	}

	void CScriptEngine::load_script(const char* path)
	{
		int ref = CScriptEngine::get_script(path, false);
		auto t1 = std::chrono::high_resolution_clock::now();
		lua_rawgeti(s_instance->m_state, LUA_REGISTRYINDEX, ref);
		if (lua_pcall(s_instance->m_state, 0, 0, 0) != LUA_OK) {
			LOG_ERROR(ELogChannel::Script, "%s failed: %s", path, lua_tostring(s_instance->m_state, -1));
		}

		auto t2 = std::chrono::high_resolution_clock::now();
		auto ms_int = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1);

		//LOG_INFO(ELogChannel::Script, "SCRIPT %s, EXECUTED IN %f ms", path, ms_int.count()/1000.f);
	}



	void CScriptEngine::unload_script(const CHashString& hash)
	{
		const auto& it = s_scriptMap.find(hash.hash());
		if (it == s_scriptMap.end()) return;
		luaL_unref(s_instance->m_state, LUA_REGISTRYINDEX, it->second);
		s_scriptMap.erase(it);
	}

	void CScriptEngine::shutdown()
	{
		if (!s_instance) return;

		lua_close(s_instance->m_state);
	}


}