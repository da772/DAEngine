// daengine
#include <daengine/script.h>

// game
#include "game_script_native.h"
#include "vehicle_script_native.h"

void CGameScriptNative::registerNatives()
{
	sol::state_view lua(da::CScriptEngine::getState());
	CVehicleScriptNative::registerNatives(&lua);
}
