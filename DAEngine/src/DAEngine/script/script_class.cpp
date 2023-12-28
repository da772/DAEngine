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

	void CScriptClass::setup(const da::core::CGuid& parent, const da::core::CGuid& guid)
	{
		m_state = CScriptEngine::loadScript(m_path.c_str());
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
            if (m_objRef == 0) {
                lua_rawgeti(L, LUA_REGISTRYINDEX, m_baseRef);
                lua_pushstring(L, m_objName.c_str());
                lua_gettable(L, -2);
                type = lua_type(L, -1);
                m_objRef = luaL_ref(L, LUA_REGISTRYINDEX);
            }

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

			// set Id 
			lua_rawgeti(L, LUA_REGISTRYINDEX, m_funcRef);
			lua_pushstring(L, "__native_set_id__");
			lua_gettable(L, -2);
			type = lua_type(L, -1);
			m_setIdRef = luaL_ref(L, LUA_REGISTRYINDEX);

			// get Id
			lua_rawgeti(L, LUA_REGISTRYINDEX, m_funcRef);
			lua_pushstring(L, "__native_get_id__");
			lua_gettable(L, -2);
			type = lua_type(L, -1);
			m_getIdRef = luaL_ref(L, LUA_REGISTRYINDEX);

			// set entity
			lua_rawgeti(L, LUA_REGISTRYINDEX, m_funcRef);
			lua_pushstring(L, "__native_set_entity__");
			lua_gettable(L, -2);
			type = lua_type(L, -1);
			m_setEntityRef = luaL_ref(L, LUA_REGISTRYINDEX);
			
			// set entity
			lua_rawgeti(L, LUA_REGISTRYINDEX, m_funcRef);
			lua_pushstring(L, "__native_get_entity__");
			lua_gettable(L, -2);
			type = lua_type(L, -1);
			m_getEntityRef = luaL_ref(L, LUA_REGISTRYINDEX);
		
			{
				// Set id
				lua_rawgeti(L, LUA_REGISTRYINDEX, m_setEntityRef);
				lua_rawgeti(L, LUA_REGISTRYINDEX, m_objRef);
				uint64_t d1, d2;

				memcpy(&d1, &parent.data()[0], sizeof(uint64_t));
				memcpy(&d2, &parent.data()[sizeof(uint64_t)], sizeof(uint64_t));

				lua_pushinteger(L, d1);
				lua_pushinteger(L, d2);
				lua_pcall(L, 3, 0, 0);
			}

			{
				// get Id
				lua_rawgeti(L, LUA_REGISTRYINDEX, m_getEntityRef);
				lua_rawgeti(L, LUA_REGISTRYINDEX, m_objRef);
				lua_pcall(L, 1, 2, 0);

				type = lua_type(L, -1);

				uint64_t d1 = lua_tointeger(L, -2);
				uint64_t d2 = lua_tointeger(L, -1);
			}

			{
				// Set id
				lua_rawgeti(L, LUA_REGISTRYINDEX, m_setIdRef);
				lua_rawgeti(L, LUA_REGISTRYINDEX, m_objRef);
				uint64_t d1, d2;

				memcpy(&d1, &guid.data()[0], sizeof(uint64_t));
				memcpy(&d2, &guid.data()[sizeof(uint64_t)], sizeof(uint64_t));

				lua_pushinteger(L, d1);
				lua_pushinteger(L, d2);
				lua_pcall(L, 3, 0, 0);
			}

			{
				// get Id
				lua_rawgeti(L, LUA_REGISTRYINDEX, m_getIdRef);
				lua_rawgeti(L, LUA_REGISTRYINDEX, m_objRef);
				lua_pcall(L, 1, 2, 0);

				type = lua_type(L, -1);

				uint64_t d1 = lua_tointeger(L, -2);
				uint64_t d2 = lua_tointeger(L, -1);
			}


			lua_pop(L, lua_gettop(L));
		}

	}

	void CScriptClass::cleanup(bool keepObj)
	{
		if (!m_state) return;

		lua_State* L = (lua_State*)m_state;

		luaL_unref(L, LUA_REGISTRYINDEX, m_baseRef);
		luaL_unref(L, LUA_REGISTRYINDEX, m_funcRef);
        if (!keepObj) luaL_unref(L, LUA_REGISTRYINDEX, m_objRef);
		luaL_unref(L, LUA_REGISTRYINDEX, m_initRef);
		luaL_unref(L, LUA_REGISTRYINDEX, m_updateRef);
		luaL_unref(L, LUA_REGISTRYINDEX, m_shutdownRef);

		luaL_unref(L, LUA_REGISTRYINDEX, m_getIdRef);
		luaL_unref(L, LUA_REGISTRYINDEX, m_setIdRef);
		luaL_unref(L, LUA_REGISTRYINDEX, m_setEntityRef);
		luaL_unref(L, LUA_REGISTRYINDEX, m_getEntityRef);
        
        m_baseRef = 0, m_funcRef = 0, m_initRef = 0, m_updateRef = 0, m_shutdownRef = 0;
        if (!keepObj) m_objRef = 0;
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
