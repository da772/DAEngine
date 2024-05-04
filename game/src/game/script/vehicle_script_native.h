#pragma once
#include <DAEngine/script.h>

class CVehicleScriptNative
{
public:
	static void registerNatives(sol::state_view* view);

private:
	static void registerTypes();
};