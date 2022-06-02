#include "dapch.h"
#include "GLFW_window.h"
#include "logger.h"
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

		CLogger::LogInfo(ELogChannel::Window, "[%s] GLFW Initialized: %d", NAMEOF(CGLFW_Window::CGLFW_Window), success);

		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

		glfwSetErrorCallback([](int i, const char* e) {
			CLogger::LogAssert(false, ELogChannel::Window, "[%s] %d: %s", NAMEOF(CGLFW_Window::glfwSetErrorCallback), i, e);
		});

		s_initialized = true;
	}

	void CGLFW_Window::initalize()
	{
		GLFWmonitor* monitor = glfwGetPrimaryMonitor();

		CLogger::LogInfo(ELogChannel::Platform, "[%s] Initializing Window: \"%s\" %d, %d", NAMEOF(CWindow::initalize), m_windowData.Title.cstr(), m_windowData.Width, m_windowData.Height);

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
		//glfwSwapBuffers(m_Window);
	}

	void CGLFW_Window::shutdown()
	{
		glfwDestroyWindow(m_Window);
	}

	void* CGLFW_Window::getNativeWindow() const
	{
		return m_Window;
	}
}

#endif
