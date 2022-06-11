#pragma once
#include "platform/window/window.h"

namespace da::platform
{
	class CGraphicsApi
	{
	public:
		CGraphicsApi(const CWindow& nativeWindow);

		inline virtual void initalize() {};
		inline virtual void update() {};
		inline virtual void shutdown() {};

		inline const CWindow& getWindow() const { return m_nativeWindow; }

	protected:
		const CWindow& m_nativeWindow;

	};
}