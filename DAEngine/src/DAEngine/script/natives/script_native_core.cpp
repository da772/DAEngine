#include "dapch.h"
#include "script_native_core.h"

#include "core/input/input.h"
#include "core/graphics/camera.h"
#include <core/ecs/scene_manager.h>
#include <core/ecs/entity.h>
#include <core/ecs/scene.h>
#include <imgui.h>

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

	da::core::CCamera::getCamera()->offsetPosition({x,y,z});
	return 0;
}

extern "C" static int lua_camera_rotate(lua_State * L)
{
	float x = luaL_checknumber(L, 2);
	float y = luaL_checknumber(L, 3);

	da::core::CCamera::getCamera()->offsetRotation({ x,0.f, -y });
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

extern "C" static int lua_entity_set_pos(lua_State * L)
{
	uint64_t id1 = lua_tointeger(L, 2);
	uint64_t id2 = lua_tointeger(L, 3);

	uint8_t ch[16];
	memcpy(&ch[0], &id1, sizeof(id1));
	memcpy(&ch[sizeof(id2)], &id2, sizeof(id2));

	da::core::CGuid guid(ch);

	if (!guid.isValid()) return 0;

	da::core::CEntity* entity = nullptr;

	if (da::core::CScene* scene = da::core::CSceneManager::getScene()) {
		for (da::core::CEntity* e : scene->getEntities()) {
			if (e->getId() == guid) {
				entity = e;
				break;
			}
		}
	}

	if (!entity) return 0;

	float x = luaL_checknumber(L, 4);
	float y = luaL_checknumber(L, 5);
	float z = luaL_checknumber(L, 6);

	entity->getTransform().setPosition({ x,y,z });

	return 0;
}

extern "C" static int lua_entity_get_pos(lua_State * L)
{
	uint64_t id1 = lua_tointeger(L, 2);
	uint64_t id2 = lua_tointeger(L, 3);

	uint8_t ch[16];
	memcpy(&ch[0], &id1, sizeof(id1));
	memcpy(&ch[sizeof(id2)], &id2, sizeof(id2));

	da::core::CGuid guid(ch);

	if (!guid.isValid()) return 0;

	const da::core::CEntity* entity = nullptr;

	if (da::core::CScene* scene = da::core::CSceneManager::getScene()) {
		for (const da::core::CEntity* e : scene->getEntities()) {
			if (e->getId() == guid) {
				entity = e;
				break;
			}
		}
	}

	if (!entity) return 0;

	lua_newtable(L);
	lua_newtable(L);
	lua_pushnumber(L, entity->getTransform().position().x);
	lua_rawseti(L, -2, 1);
	lua_pushnumber(L, entity->getTransform().position().y);
	lua_rawseti(L, -2, 2);
	lua_pushnumber(L, entity->getTransform().position().z);
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

		// Entity
		lua_register(L, "native_entity_get_position", lua_entity_get_pos);
		lua_register(L, "native_entity_set_position", lua_entity_set_pos);
	}

}