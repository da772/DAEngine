#pragma once

#include "DaEngine/core/events/event_handler.h"
#include "DaEngine/modules/module.h"
#include "DaEngine/core/window/window.h"

using namespace da::core::events;

namespace da::modules {
	class CWindowModule : public IModule
	{
	public:
		CWindowModule(const core::FWindowData& windowData);
		virtual ~CWindowModule();
		virtual void update() override;
		virtual void lateUpdate() override;
		virtual void initalize() override;
		virtual void shutdown() override;
		virtual void lateShutdown() override;

		CEventHandler& getEventHandler();

		core::CWindow& getWindow() const;

	private:
		core::CWindow* m_Window;

	};
}
