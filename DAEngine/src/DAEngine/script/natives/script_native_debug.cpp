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

struct FHashIdentifier {
	CHashString Group;
	CHashString Id;

	bool operator==(const FHashIdentifier& other) const
	{
		return (Group == other.Group
			&& Id == other.Id);
	}
};

template <>
struct std::hash<FHashIdentifier>
{
	std::size_t operator()(const FHashIdentifier& k) const
	{
		// Compute individual hash values for first,
		// second and third and combine them using XOR
		// and bit shifting:

		return (k.Group.hash()
			^ (k.Id.hash() << 1) >> 1);
	}
};

static std::unordered_map<FHashIdentifier, std::function<void()>> s_debugMenu;


extern "C" static int lua_register_debug_menu(lua_State * L)
{
	const char* group = luaL_checkstring(L, -4);
	const char* id = luaL_checkstring(L, -3);

	CHashString idHash = HASHSTR(id), groupHash = HASHSTR(group);


	if (s_debugMenu.find({ idHash, groupHash }) != s_debugMenu.end())
	{
		return 0;
	}
	
	if (!id || !lua_istable(L, -2) || !lua_isfunction(L, -1)) return 0;

	int funcRef = luaL_ref(L, LUA_REGISTRYINDEX);
	int objRef = luaL_ref(L, LUA_REGISTRYINDEX);
	
	bool* b = new bool;
	*b = false;
	da::debug::CDebugMenuBar::register_debug(groupHash, id, b, [&, L,funcRef,objRef] {
		lua_rawgeti(L, LUA_REGISTRYINDEX, funcRef);
		lua_rawgeti(L, LUA_REGISTRYINDEX, objRef);
		lua_pcall(L, 1, 0, 0);
	});

	s_debugMenu[{groupHash, idHash}] = [L, funcRef, objRef, b] {
		delete b;
		luaL_unref(L, LUA_REGISTRYINDEX, funcRef);
		luaL_unref(L, LUA_REGISTRYINDEX, objRef);
	};
	return 0;
}

extern "C" static int lua_unregister_debug_menu(lua_State * L)
{
	const char* group = luaL_checkstring(L, -2);
	const char* id = luaL_checkstring(L, -1);

	CHashString idHash = HASHSTR(id), groupHash = HASHSTR(group);
	
	if (!id || !group) return 0;

	CHashString hash = HASHSTR(id);

	const auto& it = s_debugMenu.find({idHash, groupHash});

	if (it == s_debugMenu.end())
	{
		return 0;
	}

	it->second();
	da::debug::CDebugMenuBar::unregister_debug(groupHash, idHash);

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