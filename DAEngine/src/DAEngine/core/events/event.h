#pragma once
#include "daengine/core/containers.h"

namespace da::core::events {

	enum class EEventType : uint32_t {
		None,
		WindowResize, WindowClose, WindowMove
	};

	enum class EEventCategory : uint32_t {
		None,
		Window,
	};

	class CEvent
	{
	public:
		inline CEvent(const EEventType type, const EEventCategory category) : m_type(type), m_categoryType(category) {
		};

		inline const EEventType getType() const { return m_type; }
		inline const EEventCategory getCategory() const { return m_categoryType; }

	protected:
		EEventType m_type;
		EEventCategory m_categoryType;

	};
}