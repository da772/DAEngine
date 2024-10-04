#pragma once

#include "core/events/event_handler.h"
#include "platform/app/modules/module.h"
#include "graphics/window/window.h"

using namespace da;

namespace da {
	class CWindowModule : public IModule
	{
	public:
		CWindowModule(const FWindowData& windowData);
		virtual ~CWindowModule();
		virtual void update() override;
		virtual void lateUpdate() override;
		virtual void initialize() override;
		virtual void shutdown() override;
		virtual void lateShutdown() override;

		CEventHandler& getEventHandler();

		CWindow* getWindow() const;

	private:
		CWindow* m_Window;

	};
}
