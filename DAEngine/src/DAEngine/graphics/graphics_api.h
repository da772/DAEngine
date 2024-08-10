#pragma once
#include "window/window.h"
#include "daglm.h"
#include "dacommon.h"

namespace da::graphics
{
	class CGraphicsPipeline;

	enum class EGraphicsClear : uint16_t
	{
		None = 0x00,
		Color = 0x01,
		Depth = 0x02,
		Stencil = 0x04
	};

	inline EGraphicsClear operator|(EGraphicsClear a, EGraphicsClear b)
	{
		return static_cast<EGraphicsClear>(static_cast<int>(a) | static_cast<int>(b));
	}


	class CGraphicsApi
	{
	public:
		CGraphicsApi(da::core::CWindow* nativeWindow);
		virtual ~CGraphicsApi();

		inline virtual void initialize() {};
		inline virtual void update() {};
		inline virtual void lateUpdate() {};
		inline virtual void shutdown() {};
		inline virtual glm::vec2 worldPosToScreenSpace(const glm::vec3& pos) const { return {}; }

	public:
		inline virtual void setClearColor(uint32_t target, EGraphicsClear clear, Vector4u8 color) {};
		virtual void submitPipeline(CGraphicsPipeline* pipeline) = 0;

	public:
		inline da::core::CWindow* getWindow() const { return m_nativeWindow; }

	protected:
		da::core::CWindow* m_nativeWindow;

	};
}