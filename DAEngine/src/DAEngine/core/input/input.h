#pragma once

#include "core/events/event.h"
#include "dastd.h"

namespace da {

	struct FWindowInputData
	{
		class CWindow* Window;
		std::vector<int> Inputs;
		std::function<void(const CEvent&)> Func;
	};

	struct FWindowMouseData
	{
		class CWindow* Window;
		float XPos, YPos;
		float XScroll, YScroll;
		std::vector<int> Inputs;
		std::function<void(const CEvent&)> Func;
	};

	struct FInputContextData
	{
		CHashString Context;
		uint32_t Priority;

		inline bool operator<(const FInputContextData& rhs) const
		{
			return Priority < rhs.Priority;
		}

		inline bool operator==(const CHashString& rhs) const
		{
			return Context == rhs;
		}

	};

	class CInput
	{
	public:
		static bool inputPressed(int input);
		static bool mouseInputPressed(int input);
		static double getCursorX();
		static double getCursorY();
		static double getScrollX();
		static double getScrollY();
		static void registerWindow(class CWindow* window);
		static void unregisterWindow(class CWindow* window);
		static void pushInputContext(CHashString ctx, uint32_t priority);
		static void popInputContext(CHashString ctx);
		static bool inputContextAvailable();

	private:
		static void handleKeyInput(class CWindow* window, const CEvent& e);
		static void handleMouseInput(class CWindow* window, const CEvent& e);
		static void registerKeyboardInput(CWindow* window);
		static void registerMouseInput(CWindow* window);
		static void unregisterMouseInput(CWindow* window);
		static void unregisterKeyboardInput(CWindow* window);

	private:
		static std::unordered_map<class CWindow*, FWindowInputData> s_inputs;
		static std::unordered_map<class CWindow*, FWindowMouseData> s_mouse;
		static std::set<FInputContextData> s_inputContext;
		static std::vector<FInputContextData> s_inputContextList;

	};

}