#include "dapch.h"
#include "script_class.h"

#include "script_engine.h"

extern "C" {
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}


namespace da::script
{

	CScriptClass::CScriptClass(const std::string& path, const std::string& classType, const std::string& objName) 
		: m_path(path), m_objName(objName), m_classType(classType)
	{

	}

	CScriptClass::CScriptClass()
	{

	}

	void CScriptClass::setup()
	{
		m_state = CScriptEngine::load_script(m_path.c_str());
		lua_State* L = (lua_State*)m_state;

		int i = lua_gettop(L);

		if (lua_istable(L, -1)) {

			m_baseRef = luaL_ref(L, LUA_REGISTRYINDEX);

			lua_rawgeti(L, LUA_REGISTRYINDEX, m_baseRef);
			i = lua_gettop(L);

			// Get Class Type
			lua_pushstring(L, m_classType.c_str());
			lua_gettable(L, -2);
			int type = lua_type(L, -1);
			m_classRef = luaL_ref(L, LUA_REGISTRYINDEX);
			lua_rawgeti(L, LUA_REGISTRYINDEX, m_classRef);

			// Get Class Functions
			lua_pushstring(L, "prototype");
			lua_gettable(L, -2);
			type = lua_type(L, -1);
			m_funcRef = luaL_ref(L, LUA_REGISTRYINDEX);

			// Get Object
			lua_rawgeti(L, LUA_REGISTRYINDEX, m_baseRef);
			lua_pushstring(L, m_objName.c_str());
			lua_gettable(L, -2);
			type = lua_type(L, -1);
			m_objRef = luaL_ref(L, LUA_REGISTRYINDEX);

			// Init func
			lua_rawgeti(L, LUA_REGISTRYINDEX, m_funcRef);
			lua_pushstring(L, "initialize");
			lua_gettable(L, -2);
			type = lua_type(L, -1);
			m_initRef = luaL_ref(L, LUA_REGISTRYINDEX);

			// Update func
			lua_rawgeti(L, LUA_REGISTRYINDEX, m_funcRef);
			lua_pushstring(L, "update");
			lua_gettable(L, -2);
			type = lua_type(L, -1);
			m_updateRef = luaL_ref(L, LUA_REGISTRYINDEX);

			// Shutdown func
			lua_rawgeti(L, LUA_REGISTRYINDEX, m_funcRef);
			lua_pushstring(L, "shutdown");
			lua_gettable(L, -2);
			type = lua_type(L, -1);
			m_shutdownRef = luaL_ref(L, LUA_REGISTRYINDEX);


		

			lua_pop(L, lua_gettop(L));
		}

	}

	void CScriptClass::cleanup()
	{
		if (!m_state) return;

		lua_State* L = (lua_State*)m_state;

		luaL_unref(L, LUA_REGISTRYINDEX, m_baseRef);
		luaL_unref(L, LUA_REGISTRYINDEX, m_baseRef);
		luaL_unref(L, LUA_REGISTRYINDEX, m_funcRef);
		luaL_unref(L, LUA_REGISTRYINDEX, m_objRef);
		luaL_unref(L, LUA_REGISTRYINDEX, m_initRef);
		luaL_unref(L, LUA_REGISTRYINDEX, m_updateRef);
		luaL_unref(L, LUA_REGISTRYINDEX, m_shutdownRef);
	}

	void CScriptClass::classInitialize()
	{
		if (!m_state) return;
		lua_State* L = (lua_State*)m_state;

		lua_rawgeti(L, LUA_REGISTRYINDEX, m_initRef);
		lua_rawgeti(L, LUA_REGISTRYINDEX, m_objRef);
		lua_pcall(L, 1, 0, 0);
		lua_pop(L, lua_gettop(L));
	}

	void CScriptClass::classUpdate(float dt)
	{
		if (!m_state) return;
		lua_State* L = (lua_State*)m_state;

		lua_rawgeti(L, LUA_REGISTRYINDEX, m_updateRef);
		lua_rawgeti(L, LUA_REGISTRYINDEX, m_objRef);
		lua_pushnumber(L, dt);
		lua_pcall(L, 2, 0, 0);
		lua_pop(L, lua_gettop(L));
	}

	void CScriptClass::classShutdown()
	{
		if (!m_state) return;
		lua_State* L = (lua_State*)m_state;

		lua_rawgeti(L, LUA_REGISTRYINDEX, m_shutdownRef);
		lua_rawgeti(L, LUA_REGISTRYINDEX, m_objRef);
		lua_pcall(L, 1, 0, 0);
		lua_pop(L, lua_gettop(L));
	}

}