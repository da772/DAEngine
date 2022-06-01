#pragma once

#include "daengine/modules/module.h"
#include "daengine/platform/window/window.h"

namespace da::modules {
	class CWindowModule : public IModule
	{
	public:
		CWindowModule(const platform::window::FWindowData& windowData);

		virtual void update() override;
		virtual void lateUpdate() override;
		virtual void initalize() override;
		virtual void shutdown() override;

		const platform::CWindow& getWindow() const;

	private:
		platform::CWindow* m_Window;

	};
}
