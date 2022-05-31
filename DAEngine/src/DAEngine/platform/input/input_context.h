#pragma once

#include "platform/window/window.h"

namespace da::platform
{
	class CInputContext {
	public:
		CInputContext(const CWindow& nativeWindow);

		inline virtual void initialize() {};
		inline virtual void update() {};
		inline virtual void shutdown() {};

	protected:
		const CWindow& m_nativeWindow;

	};
}
