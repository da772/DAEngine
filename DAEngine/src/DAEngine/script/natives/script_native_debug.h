#pragma once
extern "C" {
	typedef struct lua_State lua_State;
}

namespace da::script::debug {

	void registerNatives(lua_State* L);
}