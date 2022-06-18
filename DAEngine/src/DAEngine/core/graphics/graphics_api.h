#pragma once
#include "core/window/window.h"

namespace da::core
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