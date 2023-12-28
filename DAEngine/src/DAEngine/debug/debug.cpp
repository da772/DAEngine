#include "dapch.h"
#ifdef DA_DEBUG
#include "debug.h"
#include "debug_menu_bar.h"
#include "debug_stats_window.h"

namespace da::debug
{

	void CDebug::initialize()
	{
		CDebugMenuBar::initialize();
		CDebugStatsWindow::initialize();
	}

	void CDebug::update(float dt)
	{
		CDebugMenuBar::update();
	}

	void CDebug::shutdown()
	{
		CDebugStatsWindow::shutdown();
		CDebugMenuBar::shutdown();
	}

}
#endif