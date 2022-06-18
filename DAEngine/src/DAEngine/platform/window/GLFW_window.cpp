#include "dapch.h"
#include "GLFW_window.h"
#include "logger.h"
#ifdef DA_WINDOW_GLFW
#include "core/events/window_event.h"

using namespace da::core;

namespace da::platform {
	bool CGLFW_Window::s_initialized = 0;

	CGLFW_Window::CGLFW_Window(const core::FWindowData& windowData) : core::CWindow(windowData), m_Window(nullptr)
	{
		if (s_initialized)
		{
			return;
		}

		int success = glfwInit();

		ASSERT(success);

		LOG_INFO(ELogChannel::Window, "GLFW Initialized: %d", success);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		glfwSetErrorCallback([](int i, const char* e) {
			LOG_ASSERT(false, ELogChannel::Window, "%d: %s", i, e);
		});

		s_initialized = true;
	}

	void CGLFW_Window::initalize()
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();

		LOG_INFO(ELogChannel::Platform, "Initializing Window: \"%s\" %d, %d", m_windowData.Title.cstr(), m_windowData.Width, m_windowData.Height);

		m_Window = glfwCreateWindow((int)m_windowData.Width, (int)m_windowData.Height, m_windowData.Title.cstr(), nullptr, nullptr);

		const GLFWvidmode* mode = glfwGetVideoMode(monitor);
		if (!mode)
			return;

		int monitorX, monitorY;
		glfwGetMonitorPos(monitor, &monitorX, &monitorY);

		int windowWidth, windowHeight;
		glfwGetWindowSize(m_Window, &windowWidth, &windowHeight);

		glfwSetWindowPos(m_Window,
			monitorX + (mode->width - windowWidth) / 2,
			monitorY + (mode->height - windowHeight) / 2);

		glfwSetWindowUserPointer(m_Window, &m_windowData);

		// Events
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height) {
			FWindowData& data = *(FWindowData*)glfwGetWindowUserPointer(window);

			data.Width = width;
			data.Height = height;
			CWindowResizeEvent event(width, height);
			data.EventHandler.eventCallback(event);
		});

		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window) {
			FWindowData& data = *(FWindowData*)glfwGetWindowUserPointer(window);

			CWindowCloseEvent event;
			data.EventHandler.eventCallback(event);
		});

		glfwSetWindowPosCallback(m_Window, [](GLFWwindow* window, int x, int y) {
			FWindowData& data = *(FWindowData*)glfwGetWindowUserPointer(window);

			data.XPos = x;
			data.YPos = y;

			CWindowMoveEvent event(x, y);
			data.EventHandler.eventCallback(event);
		});
	}

	void CGLFW_Window::update()
	{
		glfwPollEvents();
		//glfwSwapBuffers(m_Window);
	}

	void CGLFW_Window::shutdown()
	{
		if (!m_Window) return;
		LOG_INFO(ELogChannel::Window, "Destroying Window");
		glfwDestroyWindow(m_Window);
		m_Window = nullptr;
	}

	void* CGLFW_Window::getNativeWindow() const
	{
		return m_Window;
	}
}

#endif
