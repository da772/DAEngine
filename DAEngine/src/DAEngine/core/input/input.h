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

	class CInput
	{
	public:
		static bool inputPressed(int input);
		static void registerWindow(class CWindow* window);
		static void unregisterWindow(class CWindow* window);

	private:
		static void handleInput(class CWindow* window, const events::CEvent& e);

	private:
		static std::unordered_map<class CWindow*, FWindowInputData> s_inputs;

	};

}