
#include "script_native.h"

#include "script_native_imgui.h"
#include "script_native_core.h"
#include "script_native_debug.h"

namespace da
{
	void registerNatives(lua_State* L, void* stateView)
	{
		imgui::registerNatives(L);
		da::core::registerNatives(L, stateView);
		da::debug::registerNatives(L);
	}
}