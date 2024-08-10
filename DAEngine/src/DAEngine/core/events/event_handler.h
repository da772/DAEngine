#pragma once
#include "event.h"
#include "window_event.h"
#include "input_event.h"
#include <functional>
#include "dastd.h"

namespace da::core::events
{
#define BIND_EVENT_FN(x,y) std::bind(&x::y, this, std::placeholders::_1)
#define BIND_EVENT_FN_2(x,y) std::bind(&x::y, this, std::placeholders::_1, std::placeholders::_2)
#define BIND_EVENT_FN_3(x,y) std::bind(&x::y, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)
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
			FEventCallback* cb = nullptr;
			{
				cb = new FEventCallback(callback, type, EEventCategory::None);
			}
			m_callbacks.push_back(cb);
		}

		void registerCallback(EEventCategory category, const std::function<void(const CEvent&)>& callback) {
			FEventCallback* cb = nullptr;
			{
				cb = new FEventCallback(callback, EEventType::None, category);
			}
			m_callbacks.push_back(cb);
		}

		void unregisterCallback(EEventCategory category, const std::function<void(const CEvent&)>& callback) {
			const std::vector<FEventCallback*>::iterator it = std::find_if(m_callbacks.begin(), m_callbacks.end(), ([callback, category](FEventCallback* cb) {
				return compareFunc(cb->callback_ptr, callback) && category == cb->event_category && cb->event_type == EEventType::None;
				}));
			if (it != m_callbacks.end())
			{
				{
					delete *it;
				}
				m_callbacks.erase(it);
			}
		}

		void unregisterCallback(EEventType type, const std::function<void(const CEvent&)>& callback) {
			const std::vector<FEventCallback*>::iterator it = std::find_if(m_callbacks.begin(), m_callbacks.end(), [callback, type](FEventCallback* cb) {
				return compareFunc(cb->callback_ptr, callback) && EEventCategory::None == cb->event_category && cb->event_type == type;
				});
			if (it != m_callbacks.end())
			{
				{
					delete *it;
				}
				m_callbacks.erase(it);
			}
		}
	private:
		// Std::function doesnt like to be moved ??
		// So we'll just use a pointer instead :/
		std::vector<FEventCallback*> m_callbacks;

		static bool compareFunc(const std::function<void(const CEvent&)>& lhs, const std::function<void(const CEvent&)>& rhs) {
			if (lhs.target_type() == rhs.target_type())
			{
				if (lhs.target<void(const CEvent&)>() == rhs.target<void(const CEvent&)>()) {
					return true;
				}
			}
			return false;
		}

	};

}
