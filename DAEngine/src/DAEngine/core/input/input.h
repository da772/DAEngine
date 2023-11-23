#pragma once

#include "daengine/core/events/event.h"
#include <functional>

namespace da::core {

	struct FWindowInputData
	{
		class CWindow* Window;
		std::vector<int> Inputs;
		std::function<void(const events::CEvent&)> Func;
	};

	struct FWindowMouseData
	{
		class CWindow* Window;
		float XPos, YPos;
		std::function<void(const events::CEvent&)> Func;
	};

	class CInput
	{
	public:
		static bool inputPressed(int input);
		static double getCursorX();
		static double getCursorY();
		static void registerWindow(class CWindow* window);
		static void unregisterWindow(class CWindow* window);

	private:
		static void handleKeyInput(class CWindow* window, const events::CEvent& e);
		static void handleMouseMove(class CWindow* window, const events::CEvent& e);
		static void registerKeyboardInput(CWindow* window);
		static void registerMouseMove(CWindow* window);
		static void unregisterMouseMove(CWindow* window);
		static void unregisterKeyboardInput(CWindow* window);

	private:
		static std::unordered_map<class CWindow*, FWindowInputData> s_inputs;
		static std::unordered_map<class CWindow*, FWindowMouseData> s_mouse;

	};

}