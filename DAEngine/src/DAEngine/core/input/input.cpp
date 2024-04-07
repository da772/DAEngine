#include "dapch.h"
#include "input.h"

#include "core/window/window.h"
#include "core/events/event_handler.h"

namespace da::core {

	std::unordered_map<class CWindow*, da::core::FWindowInputData> CInput::s_inputs;
	std::unordered_map<class CWindow*, da::core::FWindowMouseData> CInput::s_mouse;

	void CInput::registerWindow(class CWindow* window)
	{
		LOG_ASSERT(window, ELogChannel::Core, "Attempting to register NULL window");

		registerKeyboardInput(window);
		registerMouseInput(window);
	}

	void CInput::registerMouseInput(CWindow* window) {

		const auto& it = CInput::s_mouse.find(window);

		if (it != CInput::s_mouse.end())
		{
			LOG_ASSERT(window, ELogChannel::Core, "Attempting to register window thats already registered");
			return;
		}

		FWindowMouseData data;
		data.Window = window;
		data.XPos = 0.0;
		data.YPos = 0.0;
		data.XScroll = 0.0;
		data.YScroll = 0.0;
		data.Func = [window](const core::events::CEvent& e) {
			handleMouseInput(window, e);
			};

		window->getEventHandler().registerCallback(events::EEventType::InputCursorMove, data.Func);
		window->getEventHandler().registerCallback(events::EEventType::InputMouseButton, data.Func);
		window->getEventHandler().registerCallback(events::EEventType::InputMouseScroll, data.Func);
		s_mouse[window] = std::move(data);
	}

	void CInput::registerKeyboardInput(CWindow* window) {

		const auto& it = CInput::s_inputs.find(window);

		if (it != CInput::s_inputs.end())
		{
			LOG_ASSERT(window, ELogChannel::Core, "Attempting to register window thats already registered");
			return;
		}

		FWindowInputData data;
		data.Window = window;
		data.Func = [window](const core::events::CEvent& e) {
			handleKeyInput(window, e);
			};

		window->getEventHandler().registerCallback(events::EEventType::InputKeyboard, data.Func);

		

		s_inputs[window] = std::move(data);
	}

	void CInput::unregisterWindow(class CWindow* window)
	{
		LOG_ASSERT(window, ELogChannel::Core, "Attempting to unregister NULL window");

		unregisterKeyboardInput(window);
		unregisterMouseInput(window);
	}

	void CInput::unregisterMouseInput(CWindow* window)
	{
		const auto& it = s_mouse.find(window);

		if (it == s_mouse.end())
		{
			LOG_ASSERT(window, ELogChannel::Core, "Attempting to unregister window thats not registered");
			return;
		}

		it->second.Window->getEventHandler().unregisterCallback(events::EEventType::InputCursorMove, it->second.Func);
		it->second.Window->getEventHandler().unregisterCallback(events::EEventType::InputMouseButton, it->second.Func);
		it->second.Window->getEventHandler().unregisterCallback(events::EEventType::InputMouseScroll, it->second.Func);
		s_mouse.erase(it);
	}

	void CInput::unregisterKeyboardInput(CWindow* window)
	{
		const auto& it = s_inputs.find(window);

		if (it == s_inputs.end())
		{
			LOG_ASSERT(window, ELogChannel::Core, "Attempting to unregister window thats not registered");
			return;
		}

		it->second.Window->getEventHandler().unregisterCallback(events::EEventType::InputKeyboard, it->second.Func);
		s_inputs.erase(it);
	}

	double CInput::getCursorX()
	{
		for (const std::pair<CWindow*, FWindowMouseData>& pair : s_mouse) {
			return pair.second.XPos;
		}

		return 0.0;
	}

	double CInput::getCursorY()
	{
		for (const std::pair<CWindow*, FWindowMouseData>& pair : s_mouse) {
			return pair.second.YPos;
		}

		return 0.0;
	}

	bool CInput::inputPressed(int input)
	{
		for (const std::pair<CWindow*, FWindowInputData>& it : s_inputs) {
			if (std::find(it.second.Inputs.begin(), it.second.Inputs.end(), input) != it.second.Inputs.end()) return true;
		}

		return false;
	}

	bool CInput::mouseInputPressed(int input)
	{
		for (const std::pair<CWindow*, FWindowMouseData>& it : s_mouse) {
			if (std::find(it.second.Inputs.begin(), it.second.Inputs.end(), input) != it.second.Inputs.end()) return true;
		}

		return false;
	}

	void CInput::handleKeyInput(class CWindow* window, const events::CEvent& e)
	{
		if (e.getCategory() != EEventCategory::Input) return;

		if (e.getType() != EEventType::InputKeyboard) {
			return;
		}

		const CInputKeyboardEvent* btn = static_cast<const CInputKeyboardEvent*>(&e);
		FWindowInputData& data = s_inputs[window];

		const auto& it = std::find(data.Inputs.begin(), data.Inputs.end(), btn->getBtn());

		if (btn->getType() == da::core::EInputType::RELEASED)
		{
			if (it == data.Inputs.end())
			{
				return;
			}

			data.Inputs.erase(it);
			return;
		}

		if (it != data.Inputs.end())
		{
			return;
		}

		data.Inputs.push_back(btn->getBtn());
	}

	void CInput::handleMouseInput(class CWindow* window, const events::CEvent& e)
	{
		if (e.getCategory() != EEventCategory::Input) return;

		if (e.getType() == EEventType::InputCursorMove) {
			const CInputCursorMoveEvent* mouse = static_cast<const CInputCursorMoveEvent*>(&e);
			FWindowMouseData& data = s_mouse[window];

			data.XPos = mouse->getX();
			data.YPos = mouse->getY();
			return;
		}

		if (e.getType() == EEventType::InputMouseButton) {
			const CInputMouseButtonEvent* btn = static_cast<const CInputMouseButtonEvent*>(&e);
			FWindowMouseData& data = s_mouse[window];

			const auto& it = std::find(data.Inputs.begin(), data.Inputs.end(), btn->getBtn());

			if (btn->getType() == da::core::EInputType::RELEASED)
			{
				if (it == data.Inputs.end())
				{
					return;
				}

				data.Inputs.erase(it);
				return;
			}

			if (it != data.Inputs.end())
			{
				return;
			}

			data.Inputs.push_back(btn->getBtn());
			return;
		}

		if (e.getType() == EEventType::InputMouseButton) {
			const CInputScrollEvent* mouse = static_cast<const CInputScrollEvent*>(&e);
			FWindowMouseData& data = s_mouse[window];

			data.XScroll = mouse->getXOffset();
			data.YScroll = mouse->getYOffset();
			return;
		}
	}

}