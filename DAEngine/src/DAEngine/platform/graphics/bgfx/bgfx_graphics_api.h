#pragma once
#include "daengine/core/core.h"
#ifdef DA_GRAPHICS_BGFX
#include "daengine/core/graphics/graphics_api.h"
#include "daengine/platform/graphics/bgfx/renderer/bgfx_graphics_cluster_render.h"

class Renderer;

namespace da::platform {

	enum class ERenderApis : uint8_t {
		NOOP,
		AGC,
		D3D9,
		D3D11,
		D3D12,
		Gnm,
		Metal,
		Nvm,
		OpenGLES,
		OpenGL,
		Vulkan,
		Web,
	};

	class CbgfxGraphicsApi : public da::core::CGraphicsApi
	{
	public:
		CbgfxGraphicsApi(core::CWindow* windowModule);
		~CbgfxGraphicsApi();
		virtual void initalize() override;
		virtual void update() override;
		virtual void lateUpdate() override;
		virtual void shutdown() override;
		static ERenderApis getRendererApi();
		static const char* renderApiToString(ERenderApis api);

	public:
		virtual void setClearColor(uint32_t target, da::core::EGraphicsClear clear, Vector4u8 color) override;
		virtual void submitPipeline(da::core::CGraphicsPipeline* pipeline);
	private:
		void windowResize(const da::core::events::CEvent& event);
#ifdef DA_DEBUG
		static void renderDebugTitle();
		static bool s_showDebugTitle;
#endif

	private:
		Renderer* m_renderer;
		void* m_allocator = nullptr;
		void* m_callbacks = nullptr;
		bool m_initialized = false;
        bool m_dirtyWindow = false;
		static ERenderApis s_renderer;
	};

}
#endif
