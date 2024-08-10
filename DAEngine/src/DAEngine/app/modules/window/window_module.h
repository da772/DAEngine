#pragma once

#include "core/events/event_handler.h"
#include "app/modules/module.h"
#include "graphics/window/window.h"

using namespace da::core::events;

namespace da::modules {
	class CWindowModule : public IModule
	{
	public:
		CWindowModule(const core::FWindowData& windowData);
		virtual ~CWindowModule();
		virtual void update() override;
		virtual void lateUpdate() override;
		virtual void initialize() override;
		virtual void shutdown() override;
		virtual void lateShutdown() override;

		CEventHandler& getEventHandler();

		core::CWindow* getWindow() const;

	private:
		core::CWindow* m_Window;

	};
}
