#pragma once
#include "core/events/event.h"

namespace da::core::events
{

	class CInputEvent : public CEvent {
	public:
		inline CInputEvent(EEventType type) : CEvent(type, EEventCategory::Input) {}

	};

	class CInputCursorMoveEvent : public CInputEvent
	{
	public:
		inline CInputCursorMoveEvent(double x, double y) : CInputEvent(EEventType::InputCursorMove), m_x(x), m_y(y) {}

		inline const double getX() const { return m_x; }
		inline const double getY() const { return m_y; }

	private:
		double m_x, m_y;
	};

	enum class EInputType : int {
		RELEASED,
		PRESSED,
		REPEAT
	};

	class CInputMouseButtonEvent : public CInputEvent
	{
	public:
		inline CInputMouseButtonEvent(int btn, EInputType type, int mod) : CInputEvent(EEventType::InputMouseButton), m_btn(btn), m_type(type), m_mod(mod) {}

		inline const int getBtn() const { return m_btn; }
		inline const EInputType getType() const { return m_type; }
		inline const int getModifiers() const { return m_mod; }

	private:
		int m_btn, m_mod;
		EInputType m_type;
	};

	class CInputKeyboardEvent : public CInputEvent
	{
	public:
		inline CInputKeyboardEvent(int btn, EInputType type, int scanCode, int mod) : CInputEvent(EEventType::InputKeyboard), m_btn(btn), m_scanCode(scanCode), m_type(type), m_mod(mod) {}

		inline const int getBtn() const { return m_btn; }
		inline const EInputType getType() const { return m_type; }
		inline const int getModifiers() const { return m_mod; }
		inline const int getScanCode() const { return m_scanCode; }

	private:
		int m_btn, m_mod, m_scanCode;
		EInputType m_type;
	};

	class CInputScrollEvent : public CInputEvent
	{
	public:
		inline CInputScrollEvent(double xOffset, double yOffset) : CInputEvent(EEventType::InputMouseScroll), m_xOffset(xOffset), m_yOffset(yOffset) {}

		inline const double getXOffset() const { return m_xOffset; }
		inline const double getYOffset() const { return m_yOffset; }

	private:
		double m_xOffset, m_yOffset;
	};



}
