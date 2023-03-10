#pragma once
#include "event.h"
#include "window_event.h"
#include "input_event.h"
#include "daengine/core/containers.h"
#include "daengine/core/memory/memory_scope.h"
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
			FEventCallback* cb = nullptr;
			{
				da::memory::CMemoryScope scope(da::memory::EMemoryLayer::Core);
				cb = new FEventCallback(callback, type, EEventCategory::None);
			}
			m_callbacks.push(cb);
		}

		void registerCallback(EEventCategory category, const std::function<void(const CEvent&)>& callback) {
			FEventCallback* cb = nullptr;
			{
				da::memory::CMemoryScope scope(da::memory::EMemoryLayer::Core);
				cb = new FEventCallback(callback, EEventType::None, category);
			}
			m_callbacks.push(cb);
		}

		void unregisterCallback(EEventCategory category, const std::function<void(const CEvent&)>& callback) {
			TEnumerator<FEventCallback*> it = m_callbacks.find([callback, category](FEventCallback* cb) {
				return cb->callback_ptr.target<void(const CEvent&)>() == callback.target<void(const CEvent&)>() && category == cb->event_category && cb->event_type == EEventType::None;
				});
			if (it != m_callbacks.end())
			{
				{
					da::memory::CMemoryScope scope(da::memory::EMemoryLayer::Core);
					delete* it.get();
				}
				m_callbacks.remove(it);
			}
		}

		void unregisterCallback(EEventType type, const std::function<void(const CEvent&)>& callback) {
			TEnumerator<FEventCallback*> it = m_callbacks.find([callback, type](FEventCallback* cb) {
				return cb->callback_ptr.target<void(const CEvent&)>() == callback.target<void(const CEvent&)>() && EEventCategory::None == cb->event_category && cb->event_type == type;
				});
			if (it != m_callbacks.end())
			{
				{
					da::memory::CMemoryScope scope(da::memory::EMemoryLayer::Core);
					delete* it.get();
				}
				m_callbacks.remove(it);
			}
		}
	private:
		// Std::function doesnt like to be moved ??
		// So we'll just use a pointer instead :/
		TList < FEventCallback*, da::memory::CCoreAllocator> m_callbacks;

	};

}
