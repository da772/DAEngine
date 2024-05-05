#include "dapch.h"
#include "script_native_core.h"

#include "core/input/input.h"
#include "core/graphics/camera.h"
#include <core/ecs/scene_manager.h>
#include <core/ecs/entity.h>
#include <core/ecs/scene.h>
#include <imgui.h>
#include "DAEngine/physics/physics_rigid_body.h"
#include <core/ecs/rigid_body_component.h>
#include "script/types/script_types.h"

extern "C" {
#include <lua/lua.h>
#include <lua/lualib.h>
#include <lua/lauxlib.h>
}

#include <sol/sol.hpp>
#include <script/script_engine.h>
#include <core/time.h>
#include <core/graphics/factory/factory_graphics_texture2d.h>

namespace da::script::core
{

	static sol::table lua_camera_get_up(sol::this_state state) {

		glm::vec3 v(da::core::CCamera::getCamera()->up());
		sol::state_view lua(state);
		return CREATE_SCRIPT_TYPE(&lua, glm::vec3, &v);
	}

	static sol::table lua_camera_get_forward(sol::this_state state) {

		glm::vec3 v(da::core::CCamera::getCamera()->forward());
		sol::state_view lua(state);
		return CREATE_SCRIPT_TYPE(&lua, glm::vec3, &v);
	}

	static sol::table lua_camera_get_right(sol::this_state state) {

		glm::vec3 v(da::core::CCamera::getCamera()->right());
		sol::state_view lua(state);
		return CREATE_SCRIPT_TYPE(&lua, glm::vec3, &v);
	}

	static sol::table lua_camera_get_pos(sol::this_state state) {

		glm::vec3 v = da::core::CCamera::getCamera()->position();
		sol::state_view lua(state);
		return CREATE_SCRIPT_TYPE(&lua, glm::vec3, &v);
	}

	static void lua_camera_move(sol::this_state state, sol::object _, sol::object pos) {
		glm::vec3 v = GET_SCRIPT_TYPE(glm::vec3, pos);
		da::core::CCamera::getCamera()->offsetPosition(v);
	}

	static void lua_camera_rotate(sol::this_state state, sol::object _, sol::object rot) {

		glm::vec2 v = GET_SCRIPT_TYPE(glm::vec2, rot);
		da::core::CCamera::getCamera()->offsetRotation({ v.x, 0.f, v.y });
	}

	static sol::table lua_entity_get_pos(sol::this_state state, sol::object _, sol::object entId) {

		da::core::CGuid guid(GET_SCRIPT_TYPE(FScriptGuid, entId).Index.c_str());
		LOG_SASSERT(guid.isValid(), state.L, "Failed to convert param 1 to Guid");

		const da::core::CEntity* entity = nullptr;

		if (da::core::CScene* scene = da::core::CSceneManager::getScene()) {
			for (const da::core::CEntity* e : scene->getEntities()) {
				if (e->getId() == guid) {
					entity = e;
					break;
				}
			}
		}

		LOG_SASSERT(entity, state.L, "Failed to find entity with guid: %s", guid.c_str());

		if (!entity) return {};

		glm::vec3 pos = entity->getTransform().position();
		sol::state_view lua(state);
		return CREATE_SCRIPT_TYPE(&lua, glm::vec3, &pos);
	}

	static void lua_entity_set_pos(sol::this_state state, sol::object _, sol::object entId, sol::object scrVec) {

		da::core::CGuid guid(GET_SCRIPT_TYPE(FScriptGuid, entId).Index.c_str());
		LOG_SASSERT(guid.isValid(), state.L, "Failed to convert param 1 to Guid");
		glm::vec3 vec = GET_SCRIPT_TYPE(glm::vec3, scrVec);

		da::core::CEntity* entity = nullptr;

		if (da::core::CScene* scene = da::core::CSceneManager::getScene()) {
			for (da::core::CEntity* e : scene->getEntities()) {
				if (e->getId() == guid) {
					entity = e;
					break;
				}
			}
		}

		LOG_SASSERT(entity, state.L, "Failed to find entity with guid: %s", guid.c_str());
		if (!entity) return;
		
		entity->getTransform().setPosition(vec);
	}

	static void lua_entity_apply_vel(sol::this_state state, sol::object _, sol::object entId, sol::object scrVec) {

		da::core::CGuid guid(GET_SCRIPT_TYPE(FScriptGuid, entId).Index.c_str());
		LOG_SASSERT(guid.isValid(), state.L, "Failed to convert param 1 to Guid");
		glm::vec3 vec = GET_SCRIPT_TYPE(glm::vec3, scrVec);

		da::core::CEntity* entity = nullptr;

		if (da::core::CScene* scene = da::core::CSceneManager::getScene()) {
			for (da::core::CEntity* e : scene->getEntities()) {
				if (e->getId() == guid) {
					entity = e;
					break;
				}
			}
		}

		LOG_SASSERT(entity, state.L, "Failed to find entity with guid: %s", guid.c_str());
		if (!entity) return;

		da::core::FComponentRef<da::core::CRigidBodyComponent> c = entity->getComponent<da::core::CRigidBodyComponent>();

		if (!c.isValid()) return;
		c->getPhysicsBody()->applyImpulse(vec);
	}

	static sol::table lua_entity_get_forward(sol::this_state state, sol::object _, sol::object entId) {

		da::core::CGuid guid(GET_SCRIPT_TYPE(FScriptGuid, entId).Index.c_str());
		LOG_SASSERT(guid.isValid(), state.L, "Failed to convert param 1 to Guid");

		const da::core::CEntity* entity = nullptr;

		if (da::core::CScene* scene = da::core::CSceneManager::getScene()) {
			for (const da::core::CEntity* e : scene->getEntities()) {
				if (e->getId() == guid) {
					entity = e;
					break;
				}
			}
		}

		LOG_SASSERT(entity, state.L, "Failed to find entity with guid: %s", guid.c_str());
		if (!entity) return {};
		glm::vec3 pos = entity->getTransform().forward();
		sol::state_view lua(state);
		return CREATE_SCRIPT_TYPE(&lua, glm::vec3, &pos);
	}

	static sol::table lua_entity_get_right(sol::this_state state, sol::object _, sol::object entId) {

		da::core::CGuid guid(GET_SCRIPT_TYPE(FScriptGuid, entId).Index.c_str());
		LOG_SASSERT(guid.isValid(), state.L, "Failed to convert param 1 to Guid");

		const da::core::CEntity* entity = nullptr;

		if (da::core::CScene* scene = da::core::CSceneManager::getScene()) {
			for (const da::core::CEntity* e : scene->getEntities()) {
				if (e->getId() == guid) {
					entity = e;
					break;
				}
			}
		}

		LOG_SASSERT(entity, state.L, "Failed to find entity with guid: %s", guid.c_str());
		if (!entity) return {};
		glm::vec3 pos = entity->getTransform().right();
		sol::state_view lua(state);
		return CREATE_SCRIPT_TYPE(&lua, glm::vec3, &pos);
	}

	static sol::table lua_entity_get_up(sol::this_state state, sol::object _, sol::object entId) {

		da::core::CGuid guid(GET_SCRIPT_TYPE(FScriptGuid, entId).Index.c_str());
		LOG_SASSERT(guid.isValid(), state.L, "Failed to convert param 1 to Guid");

		const da::core::CEntity* entity = nullptr;

		if (da::core::CScene* scene = da::core::CSceneManager::getScene()) {
			for (const da::core::CEntity* e : scene->getEntities()) {
				if (e->getId() == guid) {
					entity = e;
					break;
				}
			}
		}

		LOG_SASSERT(entity, state.L, "Failed to find entity with guid: %s", guid.c_str());
		if (!entity) return {};
		glm::vec3 pos = entity->getTransform().up();
		sol::state_view lua(state);
		return CREATE_SCRIPT_TYPE(&lua, glm::vec3, &pos);
	}

	static bool lua_input_pressed(sol::this_state state, sol::object _, int key)
	{
		return da::core::CInput::inputPressed(key);
	}

	static bool lua_mouse_pressed(sol::this_state state, sol::object _, int key)
	{
		return da::core::CInput::mouseInputPressed(key);
	}

	static sol::table lua_input_cursor_pos(sol::this_state state)
	{
		double x = da::core::CInput::getCursorX();
		double y = da::core::CInput::getCursorY();

		glm::vec2 scrVec({ x,y });
		sol::state_view lua(state);
		return CREATE_SCRIPT_TYPE(&lua, glm::vec2, &scrVec);
	}

	static size_t lua_create_texture2d(sol::this_state state, sol::object _, std::string path)
	{
		if (path.empty()) return 0;
		return reinterpret_cast<size_t>(da::graphics::CTexture2DFactory::Create(path));
	}

	static void lua_destroy_texture2d(sol::this_state state, sol::object _, size_t ptr)
	{
		if (!ptr) return;
		delete reinterpret_cast<void*>(ptr);
	}


	static sol::table lua_timer_create(sol::this_state state, sol::object _, sol::object ms) {

		uint64_t time = ms.as<uint64_t>();

		int ref = CScriptEngine::getScript("scripts/build/daengine/promise.lua", false);

		lua_State* L = da::script::CScriptEngine::getState();

		sol::protected_function func(L, (sol::ref_index)ref);
		sol::protected_function_result result = func();

		ASSERT(result.valid());

		sol::table funcStack = result.get<sol::table>();

		sol::table promiseObj = funcStack["promise"].get<sol::table>();
		sol::table promise = promiseObj["_promise"].get<sol::table>();

		sol::function resolve = promiseObj["Resolve"];
		int resolveRef = 0;
		{
			sol::reference reference(L, resolve);
			resolveRef = reference.registry_index();
			reference.abandon();
		}

		int promiseRef = 0;
		{
			sol::reference reference(L, promiseObj);
			promiseRef = reference.registry_index();
			reference.abandon();
		}
		
		da::core::CTime::addTimerCallback(time, [resolveRef, promiseRef, ref, L]() {
			sol::table promiseObj(L, (sol::ref_index)promiseRef);
			sol::protected_function func(L, (sol::ref_index)resolveRef);
			sol::protected_function_result result = func(promiseObj, true);
			luaL_unref(L, LUA_REGISTRYINDEX, resolveRef);
			luaL_unref(L, LUA_REGISTRYINDEX, promiseRef);
			if (!result.valid()) {
				sol::error err = result;
				LOG_ERROR(ELogChannel::Script, "Failed to resolve promise %s", err.what());
			}
			luaL_unref(L, LUA_REGISTRYINDEX, ref);
			
		});

		return promise;
	}

	void registerNatives(lua_State* L, void* stateView)
	{
		sol::state_view* lua = (sol::state_view*)stateView;

		// Camera
		lua->set_function("native_camera_get_up", lua_camera_get_up);
		lua->set_function("native_camera_get_position", lua_camera_get_pos);
		lua->set_function("native_camera_move", lua_camera_move);
		lua->set_function("native_camera_get_forward", lua_camera_get_forward);
		lua->set_function("native_camera_get_right", lua_camera_get_right);
		lua->set_function("native_camera_rotate", lua_camera_rotate);
		lua->set_function("native_entity_get_position", lua_entity_get_pos);
		lua->set_function("native_entity_get_forward", lua_entity_get_forward);
		lua->set_function("native_entity_get_up", lua_entity_get_up);
		lua->set_function("native_entity_get_right", lua_entity_get_right);
		lua->set_function("native_entity_set_position", lua_entity_set_pos);

		// input
		lua->set_function("native_input_key_pressed", lua_input_pressed);
		lua->set_function("native_input_mouse_pressed", lua_mouse_pressed);
		lua->set_function("native_input_cursor_pos", lua_input_cursor_pos);

		// Physics
		lua->set_function("native_entity_apply_velocity", lua_entity_apply_vel);

		//Graphics
		lua->set_function("native_create_texture2d", lua_create_texture2d);
		lua->set_function("native_destroy_texture2d", lua_destroy_texture2d);
		

		// Timer
		lua->set_function("native_timer_create", lua_timer_create);
	}

}