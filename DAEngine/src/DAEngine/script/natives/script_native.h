#pragma once

extern "C" {
	typedef struct lua_State lua_State;
}

namespace da
{
	void registerNatives(lua_State* L, void* stateView);
}