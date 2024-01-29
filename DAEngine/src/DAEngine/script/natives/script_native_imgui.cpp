#include "dapch.h"
#include "script_native_imgui.h"

#include <imgui.h>

extern "C" {
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

extern "C" static int lua_imgui_begin(lua_State * L)
{
	const char* title = luaL_checkstring(L, 2);

	bool result = ImGui::Begin(title);

	lua_pushboolean(L, result);
	return 1;
}

extern "C" static int lua_imgui_button(lua_State * L)
{
	const char* title = luaL_checkstring(L, 2);

	bool result = ImGui::Button(title);

	lua_pushboolean(L, result);
	return 1;
}

extern "C" static int lua_imgui_end(lua_State * L)
{
	ImGui::End();

	return 0;
}

extern "C" static int lua_imgui_label_text(lua_State * L)
{
	const char* title = luaL_checkstring(L, 2);
	const char* fmt = luaL_checkstring(L, 3);

	if (!title || !fmt) return 0;

	ImGui::Text(title, fmt);
	return 0;
}


extern "C" static int lua_imgui_float_input(lua_State * L)
{
	const char* title = luaL_checkstring(L, 2);
	float n = luaL_checknumber(L, 3);

	if (!title) return 0;
	ImGui::InputFloat(title, &n);
	lua_pushnumber(L, n);
	return 1;
}

extern "C" static int lua_imgui_same_line(lua_State * L)
{
	ImGui::SameLine();
	return 0;
}

namespace da::script::imgui
{
	void registerNatives(lua_State* L)
	{
		lua_register(L, "native_imgui_begin", lua_imgui_begin);
		lua_register(L, "native_imgui_button", lua_imgui_button);
		lua_register(L, "native_imgui_end", lua_imgui_end);
		lua_register(L, "native_imgui_label_text", lua_imgui_label_text);
		lua_register(L, "native_imgui_float_input", lua_imgui_float_input);
		lua_register(L, "native_imgui_same_line", lua_imgui_same_line);
	}
}