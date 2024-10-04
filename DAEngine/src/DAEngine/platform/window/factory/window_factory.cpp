
#include "platform/platform.h"
#include "window_factory.h"

#ifdef DA_WINDOW_GLFW
#include "platform/window/GLFW_window.h"
#endif



da::CWindow* da::CWindowFactory::CreateWindow(const da::FWindowData& windowData)
{
#ifdef DA_WINDOW_GLFW
	return new CGLFW_Window(windowData);
#endif
	return nullptr;
}
