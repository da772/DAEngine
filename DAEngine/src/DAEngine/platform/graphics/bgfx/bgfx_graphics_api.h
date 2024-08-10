#pragma once
#include "core.h"
#ifdef DA_GRAPHICS_BGFX
#include "graphics/graphics_api.h"



namespace da::platform {

	class CBgfxTypeRenderer;

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

	class CbgfxGraphicsApi : public da::graphics::CGraphicsApi
	{
	public:
		CbgfxGraphicsApi(core::CWindow* windowModule);
		~CbgfxGraphicsApi();
		virtual void initialize() override;
		virtual void update() override;
		virtual void lateUpdate() override;
		virtual void shutdown() override;
		static ERenderApis getRendererApi();
		static const char* renderApiToString(ERenderApis api);
		virtual glm::vec2 worldPosToScreenSpace(const glm::vec3& pos) const override;

	public:
		virtual void setClearColor(uint32_t target, da::graphics::EGraphicsClear clear, Vector4u8 color) override;
		virtual void submitPipeline(da::graphics::CGraphicsPipeline* pipeline);
	private:
		void windowResize(const da::core::events::CEvent& event);
#ifdef DA_REVIEW
		static void renderDebugTitle();
		static bool s_showDebugTitle;
#endif

	private:
		CBgfxTypeRenderer* m_renderer;
		void* m_allocator = nullptr;
		void* m_callbacks = nullptr;
		bool m_initialized = false;
        bool m_dirtyWindow = false;
		static ERenderApis s_renderer;
	};

}
#endif
