#include "dapch.h"
#include "script_native.h"

#include "script_native_imgui.h"
#include "script_native_core.h"
#include "script_native_debug.h"

namespace da::script
{
	void registerNatives(lua_State* L, void* stateView)
	{
		imgui::registerNatives(L);
		core::registerNatives(L, stateView);
#ifdef DA_REVIEW
		debug::registerNatives(L);
#endif
	}
}