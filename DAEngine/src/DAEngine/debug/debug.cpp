#include "dapch.h"
#ifdef DA_DEBUG
#include "debug.h"
#include "debug_menu_bar.h"

namespace da::debug
{

	void CDebug::initialize()
	{
		CDebugMenuBar::initialize();
	}

	void CDebug::update(float dt)
	{
		CDebugMenuBar::update();
	}

	void CDebug::shutdown()
	{
		CDebugMenuBar::shutdown();
	}

}
#endif