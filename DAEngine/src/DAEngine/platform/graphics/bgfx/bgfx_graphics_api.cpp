#include "dapch.h"
#include "bgfx_graphics_api.h"

#ifdef DA_GRAPHICS_BGFX
#include "logger.h"
#include <bgfx/bgfx.h>
#include <bx/allocator.h>
#include "DAEngine/core/arg_handler.h"
#include "cluster/bgfx_type_renderer.h"
#include "cluster/bgfx_clustered_renderer.h"

#ifdef DA_REVIEW
#include <imgui.h>
#include "DAEngine/debug/debug_stats_window.h"
#include "DAEngine/debug/debug_menu_bar.h"
#include <src/bgfx_p.h>
#endif
#include <bgfx/platform.h>

//#define ENABLE_VSYNC BGFX_RESET_VSYNC
#define ENABLE_VSYNC 0

#define RESET_FLAGS BGFX_RESET_MSAA_X8 | ENABLE_VSYNC | BGFX_RESET_DEPTH_CLAMP

namespace da::platform {


	ERenderApis CbgfxGraphicsApi::s_renderer = ERenderApis::NOOP;
#ifdef DA_REVIEW
	bool CbgfxGraphicsApi::s_showDebugTitle = true;
#endif


	const char* s_bgfxRenderers[] = {
		"No-Op",
		"Agc",
		"D3D9",
		"D3D11",
		"D3D12",
		"Gnm",
		"Metal",
		"Nvm",
		"OpenGLES",
		"OpenGL",
		"Vulkan",
		"Web",
	};

	struct FDAllocator : public bx::AllocatorI
	{
		/// Allocates, resizes, or frees memory block.
		///
		/// @remark
		///  - Allocate memory block: _ptr == NULL && size > 0
		///  -   Resize memory block: _ptr != NULL && size > 0
		///  -     Free memory block: _ptr != NULL && size == 0
		///
		/// @param[in] _ptr If _ptr is NULL new block will be allocated. If _ptr is not-NULL, and
		///   _size is not 0, memory block will be resized.
		/// @param[in] _size If _ptr is set, and _size is 0, memory will be freed.
		/// @param[in] _align Alignment.
		/// @param[in] _file Debug file path info.
		/// @param[in] _line Debug file line info.
		///
		virtual void* realloc(
			void* _ptr
			, size_t _size
			, size_t _align
			, const char* _file
			, uint32_t _line
		) override {
			if (_ptr == NULL && _size > 0) {
				return malloc(_size);
			}

			if (_ptr != NULL && _size > 0) {
				return ::realloc(_ptr, _size);
			}

			if (_ptr != NULL && _size == 0) {
				free(_ptr);
				return nullptr;
			}

			return nullptr;
		}

	};

	struct FDACallbacks : public ::bgfx::CallbackI
	{
		virtual void fatal(
			const char* _filePath
			, uint16_t _line
			, ::bgfx::Fatal::Enum _code
			, const char* _str
		) override {
			LOG_ASSERT(false, da::ELogChannel::Graphics, "[bgfx] [Code: %d] %s at LINE %d %s", (int)_code, _str, (int)_line, _filePath);
		}

		virtual void traceVargs(
			const char* _filePath
			, uint16_t _line
			, const char* _format
			, va_list _argList
		) override {
			if (!m_trace) return;
			char buffer[512];
			std::string str = _format;
			std::remove(str.begin(), str.end(), '\n');
			vsnprintf(buffer, 512, str.c_str(), _argList);
			LOG_INFO(ELogChannel::Graphics, buffer);
		}

		virtual void profilerBegin(
			const char* _name
			, uint32_t _abgr
			, const char* _filePath
			, uint16_t _line
		) override {};

		virtual void profilerBeginLiteral(
			const char* _name
			, uint32_t _abgr
			, const char* _filePath
			, uint16_t _line
		) override {};

		virtual void profilerEnd() override {};

		virtual uint32_t cacheReadSize(uint64_t _id) override { return 0; };

		virtual bool cacheRead(uint64_t _id, void* _data, uint32_t _size) override { return true; };

		virtual void cacheWrite(uint64_t _id, const void* _data, uint32_t _size) override {};

		virtual void screenShot(
			const char* _filePath
			, uint32_t _width
			, uint32_t _height
			, uint32_t _pitch
			, const void* _data
			, uint32_t _size
			, bool _yflip
		) override {};

		virtual void captureBegin(
			uint32_t _width
			, uint32_t _height
			, uint32_t _pitch
			, ::bgfx::TextureFormat::Enum _format
			, bool _yflip
		) override {};

		virtual void captureEnd() override {};

		virtual void captureFrame(const void* _data, uint32_t _size) {};

		bool m_trace = false;
	};

	CbgfxGraphicsApi::CbgfxGraphicsApi(core::CWindow* windowModule) : CGraphicsApi(windowModule)
	{	
		m_allocator = new FDAllocator();
		m_callbacks = new FDACallbacks();
	}

	CbgfxGraphicsApi::~CbgfxGraphicsApi()
	{
		delete m_allocator;
		delete m_callbacks;
	}

	void CbgfxGraphicsApi::initialize()
	{
		::bgfx::Init init;
#ifdef DA_PLATFORM_WINDOWS
		s_renderer = (ERenderApis)::bgfx::RendererType::Enum::Direct3D11;
#elif defined(DA_PLATFORM_MACOSX) || defined (DA_PLATFORM_IOS)
		s_renderer = (ERenderApis)::bgfx::RendererType::Enum::Metal;
#else
		s_renderer = (ERenderApis)::bgfx::RendererType::Enum::Vulkan;
#endif

		init.type = (::bgfx::RendererType::Enum)s_renderer;
        
		::bgfx::PlatformData pd;
		pd.nwh = m_nativeWindow->getPlatformWindow();
        pd.ndt = m_nativeWindow->getPlatformDisplay();
        
		init.platformData = pd;
		const da::core::FWindowData& data = m_nativeWindow->getWindowData();
		init.resolution.width = data.Width;
		init.resolution.height = data.Height;
		init.resolution.reset = data.RefreshRate;
		// Issues with allocator, MACOSX Release
		#ifdef DA_DEBUG
		//init.allocator = (bx::AllocatorI*)m_allocator;
		#endif
		#ifdef DA_REVIEW
		if (da::core::CArgHandler::contains("debugGpu"))
		{
			((FDACallbacks*)m_callbacks)->m_trace = true;
			init.callback = (::bgfx::CallbackI*)m_callbacks;
		}
		else {
#ifndef DA_DEBUG
			init.profile = false;
			init.debug = false;
#endif
		}
		
		#endif
		LOG_INFO(ELogChannel::Graphics, "Initialzing BGFX with renderer: %s", s_bgfxRenderers[(int)init.type]);
		if (!::bgfx::init(init))
		{
			da::CLogger::LogError(da::ELogChannel::Graphics, "Failed to create bgfx");
			return;
		}

		m_initialized = true;

		m_nativeWindow->getEventHandler().registerCallback(da::core::events::EEventType::WindowResize, BIND_EVENT_FN(CbgfxGraphicsApi, windowResize));

		uint32_t debug = BGFX_DEBUG_PROFILER;

		// Enable debug text.
		::bgfx::setDebug(debug);// | BGFX_DEBUG_STATS);
		::bgfx::reset(data.Width, data.Height, RESET_FLAGS);
		::bgfx::setViewClear(0
			, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
			, 0x0c0c0cff
			, 1.0f
			, 0
		);
		::bgfx::setViewRect(0, 0, 0, data.Width, data.Height);

		m_renderer = new CBgfxClusteredRenderer();

		
		m_renderer->initialize();
		m_renderer->reset(data.Width, data.Height);

#ifdef DA_REVIEW
		da::debug::CDebugMenuBar::register_debug(HASHSTR("Renderer"), HASHSTR("Info"), &s_showDebugTitle, renderDebugTitle);
#endif

	}

	void CbgfxGraphicsApi::update()
	{
		if (m_dirtyWindow) {
			uint32_t w = m_nativeWindow->getWindowData().Width;
			uint32_t h = m_nativeWindow->getWindowData().Height;
			::bgfx::reset(w, h, RESET_FLAGS);
			::bgfx::setViewRect(0, 0, 0, w, h);
			m_renderer->reset(w, h);
			m_dirtyWindow = false;
		}
		
		::bgfx::touch(0);

#ifdef DA_REVIEW
		const ::bgfx::Stats* stats = ::bgfx::getStats();
		da::debug::CDebugStatsWindow::s_gpuTime = ((stats->gpuTimeEnd - stats->gpuTimeBegin)/ 1000.0);
		da::debug::CDebugStatsWindow::s_cpuTime = ((stats->cpuTimeEnd - stats->cpuTimeBegin) / 1000.0);
		da::debug::CDebugStatsWindow::s_waitTime = stats->waitRender / 1000.0;
		da::debug::CDebugStatsWindow::s_drawCalls = stats->numDraw;
		da::debug::CDebugStatsWindow::s_gpuMem = stats->gpuMemoryUsed;
		da::debug::CDebugStatsWindow::s_gpuMemMax = stats->gpuMemoryMax;
#endif
	}

	void CbgfxGraphicsApi::lateUpdate()
	{
		m_renderer->render(0.1f);
		::bgfx::frame();
	}

	void CbgfxGraphicsApi::shutdown()
	{
#ifdef DA_REVIEW
		da::debug::CDebugMenuBar::unregister_debug(HASHSTR("Renderer"), HASHSTR("Info"));
#endif
		m_renderer->shutdown();
#ifdef DA_DEBUG
		bgfx::setGraphicsDebuggerPresent(true);
#endif
		::bgfx::shutdown();
		m_initialized = false;
	}

	void CbgfxGraphicsApi::submitPipeline(da::graphics::CGraphicsPipeline* pipeline)
	{

	}

	void CbgfxGraphicsApi::windowResize(const da::core::events::CEvent& event)
	{
        m_dirtyWindow = true;
	}

	void CbgfxGraphicsApi::setClearColor(uint32_t target, da::graphics::EGraphicsClear clear, Vector4u8 color)
	{
		ASSERT(m_initialized);
		uint32_t c = 0;
		c |= ((uint32_t)color.x) << 0x18;
		c |= ((uint32_t)color.y) << 0x10;
		c |= ((uint32_t)color.z) << 0x8;
		c |= color.w;

		::bgfx::setViewClear(target, (uint16_t)clear, c	, 1.0f, 0);
	}

	da::platform::ERenderApis CbgfxGraphicsApi::getRendererApi()
	{
		return s_renderer;
	}

	const char* CbgfxGraphicsApi::renderApiToString(ERenderApis api)
	{
		return s_bgfxRenderers[(uint8_t)api];
	}

	glm::vec2 CbgfxGraphicsApi::worldPosToScreenSpace(const glm::vec3& pos) const
	{
		return m_renderer->worldPosToScreenSpace(pos);
	}

#ifdef DA_REVIEW
	void CbgfxGraphicsApi::renderDebugTitle()
	{
		const float wSize = 115.f;
		ImGui::SetNextWindowBgAlpha(.0f);
		ImGui::SetNextWindowPos({ ImGui::GetWindowWidth() - wSize , 24.5f });
		ImGui::SetNextWindowSize({ wSize , -1 });
		if (ImGui::Begin("Renderer Info", nullptr, ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus)) {

			ImGui::Text("DAv%s - %s", DA_VERSION, s_bgfxRenderers[(uint8_t)s_renderer]);
		}
		ImGui::End();
	}

#endif
	

}

#endif
