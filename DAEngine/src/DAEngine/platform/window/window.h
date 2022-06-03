#pragma once
#include "DAEngine/core/containers.h"
#include "DAEngine/core/events/event_handler.h"

namespace da::platform{
	
	using namespace da::core::events;
	using u16 = uint16_t;
	using u32 = uint32_t;

	namespace window {
		enum class EWindowType : u32 {
			INVALID = 0x00,
			WINDOWS = 0x01,
			MACOSX = 0x02,
			LINUX = 0x04,
			ANDROID = 0x08,
			IOS = 0x10
		};

		enum class EWindowDeco : u32 {
			NONE = 0x00,
			WINDOWED = 0x01,
			FULLSCREEN = 0x02,
		};

		struct FWindowData {
			CString Title;
			u32 Width;
			u32 Height;
			u32 XPos;
			u32 YPos;
			u16 RefreshRate;
			EWindowDeco WindowDeco;
			CEventHandler EventHandler;;
		};
	}

	class CWindow
	{
	public:
		inline CWindow(const window::FWindowData& windowData) :m_windowData(windowData) {};
		virtual void initalize() = 0;
		virtual void update() = 0;
		virtual void shutdown() = 0;

		virtual void* getNativeWindow() const = 0;

		inline CEventHandler& getEventHandler() {
			return m_windowData.EventHandler;
		}

	protected:
		window::FWindowData m_windowData;
	};
}
