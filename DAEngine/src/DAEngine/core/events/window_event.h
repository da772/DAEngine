#pragma once
#include "core/events/event.h"

namespace da {

	class CWindowEvent : public CEvent {
	public:
		inline CWindowEvent(EEventType type) : CEvent(type, EEventCategory::Window) {}
		
	};

	class CWindowResizeEvent : public CWindowEvent
	{
	public:
		inline CWindowResizeEvent(int width, int height) : CWindowEvent(EEventType::WindowResize), m_width(width), m_height(height) {}

		inline const int getWidth() const { return m_width; }
		inline const int getHeight() const { return m_height; }

	private:
		int m_width, m_height;
	};

	class CWindowCloseEvent : public CWindowEvent
	{
	public:
		inline CWindowCloseEvent() : CWindowEvent(EEventType::WindowClose) {}
	};

	class CWindowMoveEvent : public CWindowEvent
	{
	public:
		inline CWindowMoveEvent(int x, int y) : CWindowEvent(EEventType::WindowMove), m_x(x), m_y(y) {}

		inline const int getX() const { return m_x; }
		inline const int getY() const { return m_y; }

	private:
		int m_x, m_y;
	};

}
