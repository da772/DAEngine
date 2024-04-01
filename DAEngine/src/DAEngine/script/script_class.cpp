#include "dapch.h"
#include "script_class.h"

#include "script_engine.h"

extern "C" {
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#ifdef DA_PLATFORM_WINDOWS
#define SOL_LUAJIT 1
#endif
#include <sol/sol.hpp>

namespace da::script
{

	int getRef(lua_State* L, sol::object obj)
	{
		sol::reference ref(L, obj);
		int res = ref.registry_index();
		ref.abandon();
		return res;
	}

	CScriptClass::CScriptClass(const std::string& path, const std::string& classType, const std::string& objName) 
		: m_path(path), m_objName(objName), m_classType(classType)
	{

	}

	void CScriptClass::setup(const da::core::CGuid& parent, const da::core::CGuid& guid)
	{
		m_state = CScriptEngine::getState();
		ASSERT(m_state);
		lua_State* L = (lua_State*)m_state;
		int ref = CScriptEngine::getScript(m_path.c_str(), false);
		ASSERT(ref);
		sol::reference funcRef(L,(sol::ref_index)ref);		
		ASSERT(funcRef.get_type() == sol::type::function);

		sol::function func(funcRef);
		sol::function_result result = func();

		if (!result.valid()) {
			sol::error err = result;
			LOG_ASSERT(false, ELogChannel::Script, "Failed to instantiate script: %s, err:  %s", m_path, err.what());
		}

		LOG_ASSERT( (result.get_type() == sol::type::table), ELogChannel::Script, "%s did not return table type", m_classType);

		sol::table base = result.get<sol::table>();

		sol::reference baseRef(L, base);
		m_baseRef = getRef(L, base);

		sol::table classType = base[m_classType.c_str()];
		m_classRef = getRef(L, classType);

		sol::table protoType = classType["prototype"];
		m_funcRef = getRef(L, protoType);

		if (m_objRef == 0) {
			sol::table obj = base[m_objName.c_str()];
			m_objRef = getRef(L, obj);
		}
		
		sol::function initialize = protoType["initialize"];
		m_initRef = getRef(L, initialize);

		sol::function update = protoType["update"];
		m_updateRef = getRef(L, update);

		sol::function shutdown = protoType["shutdown"];
		m_shutdownRef = getRef(L, shutdown);

		sol::table guidIndex = base[m_objName.c_str()]["Id"];
		sol::table entityIndex = base[m_objName.c_str()]["Entity"]["Id"];

		da::core::guid_str compGuid, entityGuid;
		guid.c_str(compGuid);
		parent.c_str(entityGuid);

		guidIndex.set("Index", compGuid);
		entityIndex.set("Index", entityGuid);
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

		luaL_unref(L, LUA_REGISTRYINDEX, m_classRef);
        
        m_baseRef = 0, m_funcRef = 0, m_initRef = 0, m_updateRef = 0, m_shutdownRef = 0, m_classRef = 0;
        if (!keepObj) m_objRef = 0;
	}

	void CScriptClass::classInitialize()
	{
		if (!m_state) return;
		lua_State* L = (lua_State*)m_state;
		sol::state_view lua(L);

		sol::protected_function func(lua, (sol::ref_index)m_initRef);
#ifdef DA_REVIEW
		sol::protected_function protectedFunc = lua.safe_script(
			"function eh(err) \
				print(\"Error: \", err) \
				print(debug.traceback()) \
			end\
			function callFunc(func, obj, dt) \
				status, ret = xpcall(func, eh, obj)\
				return status \
			end\
			return callFunc\
			");
		sol::protected_function_result res = protectedFunc(func, sol::object(L, (sol::ref_index)m_objRef));
#else
		sol::protected_function_result res = func(sol::object(L, (sol::ref_index)m_objRef));
#endif

		if (!res.valid()) {
			sol::error err = res;
			LOG_ASSERT(res.valid(), ELogChannel::Script, "Script Failed: %s with Err: %s", m_path, err.what())
		}

#ifdef DA_REVIEW
		if (!res.get<bool>()) {
			sol::error err = res;
			LOG_ASSERT(res.valid(), ELogChannel::Script, "Script Failed: %s with Err: %s", m_path, err.what())
		}
#endif
	}

	void CScriptClass::classUpdate(float dt)
	{
		if (!m_state) return;
		lua_State* L = (lua_State*)m_state;
		sol::state_view lua(L);

		sol::protected_function func(lua, (sol::ref_index)m_updateRef);	
#ifdef DA_REVIEW
		sol::protected_function protectedFunc = lua.safe_script(
			"function eh(err) \
				print(\"Error: \", err) \
				print(debug.traceback()) \
			end\
			function callFunc(func, obj, dt) \
				status, ret = xpcall(func, eh, obj, dt)\
				return status \
			end\
			return callFunc\
			");
		sol::protected_function_result res = protectedFunc(func, sol::object(L, (sol::ref_index)m_objRef), dt);
#else
		sol::protected_function_result res = func(sol::object(L, (sol::ref_index)m_objRef), dt);
#endif

		if (!res.valid()) {
			sol::error err = res;
			LOG_ASSERT(res.valid(), ELogChannel::Script, "Script Failed: %s with Err: %s", m_path, err.what())
		}

#ifdef DA_REVIEW
		if (!res.get<bool>()) {
			sol::error err = res;
			LOG_ASSERT(res.valid(), ELogChannel::Script, "Script Failed: %s with Err: %s", m_path, err.what())
		}
#endif
	}

	void CScriptClass::classShutdown()
	{
		if (!m_state) return;
		lua_State* L = (lua_State*)m_state;
		sol::state_view lua(L);

		sol::protected_function func(lua, (sol::ref_index)m_shutdownRef);
#ifdef DA_REVIEW
		sol::protected_function protectedFunc = lua.safe_script(
			"function eh(err) \
				print(\"Error: \", err) \
				print(debug.traceback()) \
			end\
			function callFunc(func, obj, dt) \
				status, ret = xpcall(func, eh, obj)\
				return status \
			end\
			return callFunc\
			");
		sol::protected_function_result res = protectedFunc(func, sol::object(L, (sol::ref_index)m_objRef));
#else
		sol::protected_function_result res = func(sol::object(L, (sol::ref_index)m_objRef));
#endif

		if (!res.valid()) {
			sol::error err = res;
			LOG_ASSERT(res.valid(), ELogChannel::Script, "Script Failed: %s with Err: %s", m_path, err.what())
		}

#ifdef DA_REVIEW
		if (!res.get<bool>()) {
			sol::error err = res;
			LOG_ASSERT(res.valid(), ELogChannel::Script, "Script Failed: %s with Err: %s", m_path, err.what())
		}
#endif
	}

}
