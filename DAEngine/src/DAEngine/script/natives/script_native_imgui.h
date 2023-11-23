#pragma once

extern "C" {
	typedef struct lua_State lua_State;
}

namespace da::script::imgui {

	void registerNatives(lua_State* L);
}