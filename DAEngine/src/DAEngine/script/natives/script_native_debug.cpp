#include "dapch.h"
#include "script_native_debug.h"
extern "C" {
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#ifdef DA_REVIEW

#include <imgui.h>
#include "debug/debug_menu_bar.h"



struct FHashIdentifier {
	CHashString Group;
	CHashString Id;

	bool operator==(const FHashIdentifier& other) const
	{
		return (Group == other.Group
			&& Id == other.Id);
	}

	size_t hash() const {
		return (Group.hash()
			^ (Id.hash() << 1) >> 1);
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

		return k.hash();
	}
};

static std::unordered_map<size_t, std::function<void()>> s_debugMenu;


extern "C" static int lua_register_debug_menu(lua_State * L)
{
	const char* group = luaL_checkstring(L, -4);
	const char* id = luaL_checkstring(L, -3);

	CHashString idHash = HASHSTR(id), groupHash = HASHSTR(group);

	size_t hashId = FHashIdentifier(idHash, groupHash).hash();

	if (s_debugMenu.find(hashId) != s_debugMenu.end())
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

	s_debugMenu[hashId] = [L, funcRef, objRef, b] {
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

	size_t hashId = FHashIdentifier(idHash, groupHash).hash();

	const auto& it = s_debugMenu.find(hashId);

	if (it == s_debugMenu.end())
	{
		return 0;
	}

	da::debug::CDebugMenuBar::unregister_debug(groupHash, idHash);
	s_debugMenu.erase(it);

	return 0;
}
#else

extern "C" static int lua_register_debug_menu(lua_State * L)
{
	return 0;
}

extern "C" static int lua_unregister_debug_menu(lua_State * L)
{
	return 0;
}
#endif
namespace da::script::debug
{
	void registerNatives(lua_State* L)
	{
		lua_register(L, "native_debug_register_debug_menu", lua_register_debug_menu);
		lua_register(L, "native_debug_unregister_debug_menu", lua_unregister_debug_menu);
	}
}
