#include "dapch.h"
#include "GLFW_window.h"
#ifdef DA_WINDOW_GLFW

namespace da::platform::window {
	bool CGLFW_Window::s_initialized = 0;

	CGLFW_Window::CGLFW_Window(const FWindowData& windowData) : CWindow(windowData), m_Window(nullptr)
	{
		if (s_initialized)
		{
			return;
		}

		int success = glfwInit();

		ASSERT(success);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		s_initialized = true;
	}

	void CGLFW_Window::initalize()
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();

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
	}

	void CGLFW_Window::update()
	{
		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	void CGLFW_Window::shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void* CGLFW_Window::getNativeWindow()
	{
		return m_Window;
	}
}

#endif
