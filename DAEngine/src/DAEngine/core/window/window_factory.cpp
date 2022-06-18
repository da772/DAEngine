#include "dapch.h"
#include "platform/platform.h"
#include "window_factory.h"
#include "logger.h"

#ifdef DA_WINDOW_GLFW
#include "platform/window/GLFW_window.h"
#endif

namespace da::core {

	da::core::CWindow* CWindowFactory::CreateWindow(const FWindowData& windowData)
	{
#ifdef DA_WINDOW_GLFW
		return new platform::CGLFW_Window(windowData);
#endif
		return nullptr;
	}

}