#pragma once
#include "event.h"
#include "window_event.h"
#include "daengine/core/containers.h"
#include <functional>

namespace da::core::events
{
#define BIND_EVENT_FN(x,y) std::bind(&x::y, this, std::placeholders::_1)
#define EVENT_HANDLER_REGISTER_CALLBACK(x)

	struct FEventCallback {
		FEventCallback(const std::function<void(const CEvent&)>& callback, EEventType type, EEventCategory category) : callback_ptr(callback), event_type(type), event_category(category) {};
		std::function<void(const CEvent&)> callback_ptr;
		EEventType event_type;
		EEventCategory event_category;
	};

	class CEventHandler {
	public:
		inline CEventHandler() {}

		inline ~CEventHandler() {
			for (FEventCallback* ptr : m_callbacks) {
				delete ptr;
			}
			m_callbacks.clear();
		}

		void eventCallback(const CEvent& event) {
			for (const FEventCallback* e : m_callbacks) {
				if (e->event_type != event.getType() && e->event_category != event.getCategory()) continue;

				e->callback_ptr(event);
			}
		}

	public:
		void registerCallback(EEventType type, const std::function<void(const CEvent&)>& callback) {
			m_callbacks.push(new FEventCallback(callback, type, EEventCategory::None));
		}

		void registerCallback(EEventCategory category, const std::function<void(const CEvent&)>& callback) {
			m_callbacks.push(new FEventCallback(callback, EEventType::None, category));
		}
	private:
		// Std::function doesnt like to be moved ??
		// So we'll just use a pointer instead :/
		TList<FEventCallback*> m_callbacks;

	};

}
