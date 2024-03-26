#pragma once

extern "C" {
	typedef struct lua_State lua_State;
}

namespace da::script::core {

	void registerNatives(lua_State* L, void* stateView);
}