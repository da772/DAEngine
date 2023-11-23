#include "dapch.h"
#include "script_native_core.h"

#include "core/input/input.h"
#include "core/graphics/camera.h"

extern "C" {
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

extern "C" static int lua_input_pressed(lua_State * L)
{
	int key = luaL_checkint(L, 2);

	bool result = da::core::CInput::inputPressed(key);

	lua_pushboolean(L, result);
	return 1;
}

extern "C" static int lua_input_cursor_pos(lua_State * L)
{
	double x = da::core::CInput::getCursorX();
	double y = da::core::CInput::getCursorY();

	lua_newtable(L);
	lua_pushnumber(L, x);
	lua_rawseti(L, -2, 1);
	lua_pushnumber(L, y);
	lua_rawseti(L, -2, 2);
	
	return 1;
}

extern "C" static int lua_camera_move(lua_State * L)
{
	float x = luaL_checknumber(L, 2);
	float y = luaL_checknumber(L, 3);
	float z = luaL_checknumber(L, 4);

	da::core::CCamera::getCamera()->move({x,y,z});
	return 0;
}

extern "C" static int lua_camera_rotate(lua_State * L)
{
	float x = luaL_checknumber(L, 2);
	float y = luaL_checknumber(L, 3);

	da::core::CCamera::getCamera()->rotate({ x,y });
	return 0;
}


extern "C" static int lua_camera_get_forward(lua_State * L)
{
	glm::vec3 v = da::core::CCamera::getCamera()->forward();

	lua_newtable(L);
	lua_pushnumber(L, v.x);
	lua_rawseti(L, -2, 1);
	lua_pushnumber(L, v.y);
	lua_rawseti(L, -2, 2);
	lua_pushnumber(L, v.z);
	lua_rawseti(L, -2, 3);

	return 1;
}

extern "C" static int lua_camera_get_up(lua_State * L)
{
	glm::vec3 v = da::core::CCamera::getCamera()->up();

	lua_newtable(L);
	lua_pushnumber(L, v.x);
	lua_rawseti(L, -2, 1);
	lua_pushnumber(L, v.y);
	lua_rawseti(L, -2, 2);
	lua_pushnumber(L, v.z);
	lua_rawseti(L, -2, 3);

	return 1;
}

extern "C" static int lua_camera_get_right(lua_State * L)
{
	glm::vec3 v = da::core::CCamera::getCamera()->right();

	lua_newtable(L);
	lua_newtable(L);
	lua_pushnumber(L, v.x);
	lua_rawseti(L, -2, 1);
	lua_pushnumber(L, v.y);
	lua_rawseti(L, -2, 2);
	lua_pushnumber(L, v.z);
	lua_rawseti(L, -2, 3);

	return 1;
}

extern "C" static int lua_camera_get_pos(lua_State * L)
{
	glm::vec3 v = da::core::CCamera::getCamera()->position();

	lua_newtable(L);
	lua_newtable(L);
	lua_pushnumber(L, v.x);
	lua_rawseti(L, -2, 1);
	lua_pushnumber(L, v.y);
	lua_rawseti(L, -2, 2);
	lua_pushnumber(L, v.z);
	lua_rawseti(L, -2, 3);

	return 1;
}

namespace da::script::core
{

	void registerNatives(lua_State* L)
	{
		// Input
		lua_register(L, "native_input_key_pressed", lua_input_pressed);
		lua_register(L, "native_input_cursor_pos", lua_input_cursor_pos);

		// Camera
		lua_register(L, "native_camera_move", lua_camera_move);
		lua_register(L, "native_camera_rotate", lua_camera_rotate);
		lua_register(L, "native_camera_get_forward", lua_camera_get_forward);
		lua_register(L, "native_camera_get_up", lua_camera_get_up);
		lua_register(L, "native_camera_get_right", lua_camera_get_right);
		lua_register(L, "native_camera_get_position", lua_camera_get_pos);
	}

}