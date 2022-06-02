#pragma once
#include "platform/platform.h"

#ifdef DA_WINDOW_GLFW
#include "window.h"
#include <GLFW/glfw3.h>

namespace da::platform::window {

	class CGLFW_Window : public CWindow
	{
	public:
		CGLFW_Window(const FWindowData& windowData);

		void initalize() override;
		void update() override;
		void shutdown() override;
		void* getNativeWindow() const override;

	private:
		GLFWwindow* m_Window;

	private:
		static bool s_initialized;

	};



}

#endif
