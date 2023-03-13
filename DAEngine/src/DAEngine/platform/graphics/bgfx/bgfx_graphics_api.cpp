#include "dapch.h"
#include "bgfx_graphics_api.h"

#ifdef DA_GRAPHICS_BGFX
#include "logger.h"
#include <bgfx/bgfx.h>
#include <bx/allocator.h>
#include "DAEngine/core/arg_handler.h"
#include "bgfx_graphics_test.h"

namespace da::platform {

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
				return m_allocator.allocate(_size);
			}

			if (_ptr != NULL && _size > 0) {
				return m_allocator.reallocate(_ptr, _size);
			}

			if (_ptr != NULL && _size == 0) {
				m_allocator.deallocate(_ptr);
				return nullptr;
			}

			return nullptr;
		}

		da::memory::CGraphicsAllocator m_allocator;
	};

	struct FDACallbacks : public bgfx::CallbackI
	{
		virtual void fatal(
			const char* _filePath
			, uint16_t _line
			, bgfx::Fatal::Enum _code
			, const char* _str
		) override {
			LOG_ERROR(da::ELogChannel::Graphics, "[bgfx] [Code: %d] %s at LINE %d %s", (int)_code, _str, (int)_line, _filePath);
		}

		virtual void traceVargs(
			const char* _filePath
			, uint16_t _line
			, const char* _format
			, va_list _argList
		) override {
			if (!m_trace) return;
			char buffer[512];
			CString str = _format;
			str.remove('\n');
			vsnprintf(buffer, 512, str.cstr(), _argList);
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
			, bgfx::TextureFormat::Enum _format
			, bool _yflip
		) override {};

		virtual void captureEnd() override {};

		virtual void captureFrame(const void* _data, uint32_t _size) {};

		bool m_trace = false;
	};

	CBgfxGraphicsTest* s_test;

	CbgfxGraphicsApi::CbgfxGraphicsApi(core::CWindow* windowModule) : CGraphicsApi(windowModule)
	{	
		da::memory::CMemoryScope scope(memory::EMemoryLayer::Graphics);
		m_allocator = new FDAllocator();
		m_callbacks = new FDACallbacks();
		s_test = new CBgfxGraphicsTest();
	}

	CbgfxGraphicsApi::~CbgfxGraphicsApi()
	{
		da::memory::CMemoryScope scope(memory::EMemoryLayer::Graphics);
		delete m_allocator;
		delete m_callbacks;
		delete s_test;
	}

	void CbgfxGraphicsApi::initalize()
	{
		bgfx::Init init;
#ifdef DA_PLATFORM_WINDOWS
		m_renderer = (uint8_t)bgfx::RendererType::Enum::Direct3D12;
#elif defined(DA_PLATFORM_MACOSX) || defined (DA_PLATFORM_IOS)
		m_renderer = (uint8_t)bgfx::RendererType::Enum::Vulkan;
#else
		m_renderer = (uint8_t)bgfx::RendererType::Enum::Vulkan;
#endif

		init.type = (bgfx::RendererType::Enum)m_renderer;
        
		bgfx::PlatformData pd;
		pd.nwh = m_nativeWindow->getPlatformWindow();
        pd.ndt = m_nativeWindow->getPlatformDisplay();
        
		init.platformData = pd;
		const da::core::FWindowData& data = m_nativeWindow->getWindowData();
		init.resolution.width = data.Width;
		init.resolution.height = data.Height;
		init.resolution.reset = data.RefreshRate;
		// Issues with allocator, MACOSX Release
		//init.allocator = (bx::AllocatorI*)m_allocator;
		#ifdef DA_DEBUG
		((FDACallbacks*)m_callbacks)->m_trace = da::core::CArgHandler::contains("debugGpu");
		init.callback = (bgfx::CallbackI*)m_callbacks;
		#endif
		LOG_INFO(ELogChannel::Graphics, "Initialzing BGFX with renderer: %s", s_bgfxRenderers[(int)init.type]);
		if (!bgfx::init(init))
		{
			da::CLogger::LogError(da::ELogChannel::Graphics, "Failed to create bgfx");
			return;
		}

		m_initialized = true;

		m_nativeWindow->getEventHandler().registerCallback(da::core::events::EEventType::WindowResize, BIND_EVENT_FN(CbgfxGraphicsApi, windowResize));

		// Enable debug text.
		bgfx::setDebug(BGFX_DEBUG_TEXT);
		bgfx::reset(data.Width, data.Height);
		bgfx::setViewClear(0
			, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
			, 0x0c0c0cff
			, 1.0f
			, 0
		);
		bgfx::setViewRect(0, 0, 0, data.Width, data.Height);

		s_test->Initialize(m_nativeWindow);
	}

	void CbgfxGraphicsApi::update()
	{
        if (m_dirtyWindow) {
            uint32_t w = m_nativeWindow->getWindowData().Width;
            uint32_t h = m_nativeWindow->getWindowData().Height;
            bgfx::reset(w, h);
            bgfx::setViewRect(0, 0, 0, w, h);
            m_dirtyWindow = false;
        }

		bgfx::touch(0);
		s_test->Render();

		bgfx::dbgTextClear();
		bgfx::dbgTextPrintf(0, 0, 0x0f, "DAv%s - %s", DA_VERSION, s_bgfxRenderers[m_renderer]);
 
		bgfx::frame();
	}

	void CbgfxGraphicsApi::shutdown()
	{
		s_test->Shutdown();
		bgfx::shutdown();
		m_initialized = false;
	}

	void CbgfxGraphicsApi::submitPipeline(da::core::CGraphicsPipeline* pipeline)
	{

	}

	void CbgfxGraphicsApi::windowResize(const da::core::events::CEvent& event)
	{
        m_dirtyWindow = true;
	}

	void CbgfxGraphicsApi::setClearColor(uint32_t target, da::core::EGraphicsClear clear, Vector4u8 color)
	{
		ASSERT(m_initialized);
		uint32_t c = 0;
		c |= ((uint32_t)color.x) << 0x18;
		c |= ((uint32_t)color.y) << 0x10;
		c |= ((uint32_t)color.z) << 0x8;
		c |= color.w;

		bgfx::setViewClear(target, (uint16_t)clear, c	, 1.0f, 0);
	}

}

#endif
