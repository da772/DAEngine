#include "dapch.h"
#include "input.h"

#include "core/window/window.h"
#include "core/events/event_handler.h"

namespace da::core {

	std::unordered_map<class CWindow*, da::core::FWindowInputData> CInput::s_inputs;

	void CInput::registerWindow(class CWindow* window)
	{
		LOG_ASSERT(window, ELogChannel::Core, "Attempting to register NULL window");

		const auto& it = s_inputs.find(window);

		if (it != s_inputs.end())
		{
			LOG_ASSERT(window, ELogChannel::Core, "Attempting to register window thats already registered");
			return;
		}

		FWindowInputData data;
		data.Window = window;
		data.Func = [window](const core::events::CEvent& e) {
			handleInput(window, e);
			};

		window->getEventHandler().registerCallback(events::EEventType::InputKeyboard, data.Func);

		s_inputs[window] = std::move(data);
	}

	void CInput::unregisterWindow(class CWindow* window)
	{
		LOG_ASSERT(window, ELogChannel::Core, "Attempting to unregister NULL window");

		const auto& it = s_inputs.find(window);

		if (it == s_inputs.end())
		{
			LOG_ASSERT(window, ELogChannel::Core, "Attempting to unregister window thats not registered");
			return;
		}

		it->second.Window->getEventHandler().unregisterCallback(events::EEventType::InputKeyboard, it->second.Func);
		s_inputs.erase(it);
	}

	bool CInput::inputPressed(int input)
	{
		for (const std::pair<CWindow*, FWindowInputData>& it : s_inputs) {
			if (std::find(it.second.Inputs.begin(), it.second.Inputs.end(), input) != it.second.Inputs.end()) return true;
		}

		return false;
	}

	void CInput::handleInput(class CWindow* window, const events::CEvent& e)
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

}