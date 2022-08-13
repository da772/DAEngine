#pragma once
#include "daengine/core/window/window.h"


namespace da::core
{
	class CGraphicsPipeline;

	class CGraphicsApi
	{
	public:
		CGraphicsApi(const CWindow& nativeWindow);
		virtual ~CGraphicsApi();

		inline virtual void initalize() {};
		inline virtual void update() {};
		inline virtual void shutdown() {};

		virtual void submitPipeline(CGraphicsPipeline* pipeline) = 0;

		inline const CWindow& getWindow() const { return m_nativeWindow; }

	protected:
		const CWindow& m_nativeWindow;

	};
}