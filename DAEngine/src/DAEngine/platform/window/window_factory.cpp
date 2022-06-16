#include "dapch.h"
#include "platform/platform.h"
#include "window_factory.h"
#include "logger.h"

#ifdef DA_WINDOW_GLFW
#include "platform/window/GLFW_window.h"
#endif

namespace da::platform {

	da::platform::CWindow* CWindowFactory::CreateWindow(const FWindowData& windowData)
	{
#ifdef DA_WINDOW_GLFW
		return new CGLFW_Window(windowData);
#endif
		return nullptr;
	}

}