#include "debug.h"
#ifdef DA_REVIEW

#include "debug_menu_bar.h"
#include "debug_stats_window.h"
#include <debug/graphics_debug_render.h>

namespace da
{

	void CDebug::initialize()
	{
		CDebugMenuBar::initialize();
		CDebugStatsWindow::initialize();
	}

	void CDebug::update(float dt)
	{
		da::CDebugRender::clear();
		CDebugMenuBar::update();
	}

	void CDebug::shutdown()
	{
		CDebugStatsWindow::shutdown();
		CDebugMenuBar::shutdown();
	}

}
#endif