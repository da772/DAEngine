
#include "platform/platform.h"
#include "window_factory.h"

#ifdef DA_WINDOW_GLFW
#include "platform/window/GLFW_window.h"
#endif



da::core::CWindow* da::factory::CWindowFactory::CreateWindow(const da::core::FWindowData& windowData)
{
#ifdef DA_WINDOW_GLFW
	return new platform::CGLFW_Window(windowData);
#endif
	return nullptr;
}
