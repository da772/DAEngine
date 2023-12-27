#include "dapch.h"
#ifdef DA_DEBUG
#include "script_native_debug.h"

#include <imgui.h>
#include "debug/debug_menu_bar.h"

extern "C" {
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

static std::unordered_map<CHashString, std::function<void()>> s_debugMenu;

extern "C" static int lua_register_debug_menu(lua_State * L)
{
	const char* id = luaL_checkstring(L, -3);

	CHashString hash = HASHSTR(id);

	if (s_debugMenu.find(HASHSTR(id)) != s_debugMenu.end())
	{
		return 0;
	}
	
	if (!id || !lua_istable(L, -2) || !lua_isfunction(L, -1)) return 0;

	int funcRef = luaL_ref(L, LUA_REGISTRYINDEX);
	int objRef = luaL_ref(L, LUA_REGISTRYINDEX);
	
	bool* b = new bool;
	*b = false;
	da::debug::CDebugMenuBar::register_debug(id, b, [&, L,funcRef,objRef] {
		lua_rawgeti(L, LUA_REGISTRYINDEX, funcRef);
		lua_rawgeti(L, LUA_REGISTRYINDEX, objRef);
		lua_pcall(L, 1, 0, 0);
	});

	s_debugMenu[hash] = [L, funcRef, objRef, b] {
		delete b;
		luaL_unref(L, LUA_REGISTRYINDEX, funcRef);
		luaL_unref(L, LUA_REGISTRYINDEX, objRef);
	};
	return 0;
}

extern "C" static int lua_unregister_debug_menu(lua_State * L)
{
	const char* id = luaL_checkstring(L, 2);
	
	if (!id) return 0;

	CHashString hash = HASHSTR(id);

	const auto& it = s_debugMenu.find(HASHSTR(id));

	if (it == s_debugMenu.end())
	{
		return 0;
	}

	it->second();
	da::debug::CDebugMenuBar::unregister_debug(id);

	return 0;
}

namespace da::script::debug
{
	void registerNatives(lua_State* L)
	{
		lua_register(L, "native_debug_register_debug_menu", lua_register_debug_menu);
		lua_register(L, "native_debug_unregister_debug_menu", lua_unregister_debug_menu);
	}
}
#endif