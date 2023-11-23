#include "dapch.h"
#include "script_native.h"

#include "script_native_imgui.h"
#include "script_native_core.h"

namespace da::script
{
	void registerNatives(lua_State* L)
	{
		imgui::registerNatives(L);
		core::registerNatives(L);
	}
}