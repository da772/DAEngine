#pragma once

extern "C" {
	typedef struct lua_State lua_State;
}

namespace da::core {

	void registerNatives(lua_State* L, void* stateView);
}