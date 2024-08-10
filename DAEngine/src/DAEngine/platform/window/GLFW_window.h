#pragma once
#include "core.h"

#ifdef DA_WINDOW_GLFW
#include "graphics/window/window.h"
#include <GLFW/glfw3.h>

namespace da::platform {

	class CGLFW_Window : public core::CWindow
	{
	public:
		CGLFW_Window(const core::FWindowData& windowData);

		void initialize() override;
		void update() override;
		void shutdown() override;
		void* getNativeWindow() const override;
		void* getPlatformWindow() const override;
        void* getPlatformDisplay() const override;
		void lockCursor(bool lock) override;
        
	private:
		GLFWwindow* m_Window;

	private:
		static bool s_initialized;

	};



}

#endif
