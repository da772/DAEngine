#pragma once

#include "daengine/core/events/event_handler.h"
#include "daengine/modules/module.h"
#include "daengine/platform/window/window.h"

using namespace da::core::events;

namespace da::modules {
	class CWindowModule : public IModule
	{
	public:
		CWindowModule(const platform::window::FWindowData& windowData);

		virtual void update() override;
		virtual void lateUpdate() override;
		virtual void initalize() override;
		virtual void shutdown() override;
		virtual void lateShutdown() override;

		CEventHandler& getEventHandler();

		const platform::CWindow& getWindow() const;

	private:
		platform::CWindow* m_Window;

	};
}
