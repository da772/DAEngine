#include "dapch.h"
#include "bgfx_graphics_api.h"

#ifdef DA_GRAPHICS_BGFX
#include "logger.h"
#include <bgfx/bgfx.h>
#include <bx/allocator.h>

namespace da::platform {

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
	};

	CbgfxGraphicsApi::CbgfxGraphicsApi(core::CWindow* windowModule) : CGraphicsApi(windowModule)
	{	
		da::memory::CMemoryScope scope(memory::EMemoryLayer::Graphics);
		m_allocator = new FDAllocator();
		m_callbacks = new FDACallbacks();
	}

	CbgfxGraphicsApi::~CbgfxGraphicsApi()
	{
		da::memory::CMemoryScope scope(memory::EMemoryLayer::Graphics);
		delete m_allocator;
		delete m_callbacks;
	}

	void CbgfxGraphicsApi::initalize()
	{
		bgfx::Init init;
#ifdef DA_PLATFORM_WINDOWS
		init.type = bgfx::RendererType::Enum::Direct3D12;
#elif defined(DA_PLATFORM_MAC) || defined (DA_PLATFORM_IOS)
		init.type = bgfx::RendererType::Enum::Metal;
#else
		init.type = bgfx::RendererType::Enum::Vulkan;
#endif
		bgfx::PlatformData pd;
		pd.nwh = m_nativeWindow->getPlatformWindow();
#ifdef DA_PLATFORM_WINDOWS
		// Set window display data for other platforms
		pd.ndt = NULL;
#endif
		init.platformData = pd;
		const da::core::FWindowData& data = m_nativeWindow->getWindowData();
		init.resolution.width = data.Width;
		init.resolution.height = data.Height;
		init.resolution.reset = data.RefreshRate;
		// issue with alloator on mulithreads
		init.allocator = (bx::AllocatorI*)m_allocator;
		init.callback = (bgfx::CallbackI*)m_callbacks;

		if (!bgfx::init(init))
		{
			da::CLogger::LogError(da::ELogChannel::Graphics, "Failed to create bgfx");
			return;
		}

		m_initialized = true;

		m_nativeWindow->getEventHandler().registerCallback(da::core::events::EEventType::WindowResize, BIND_EVENT_FN(CbgfxGraphicsApi, windowResize));

		// Enable debug text.
		bgfx::setDebug(BGFX_DEBUG_TEXT);

		bgfx::setViewClear(0
			, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
			, 0x303030ff
			, 1.0f
			, 0
		);
		bgfx::setViewRect(0, 0, 0, data.Width, data.Height);
	}

	void CbgfxGraphicsApi::update()
	{
		bgfx::touch(0);
		bgfx::dbgTextClear();
		bgfx::dbgTextPrintf(0, 1, 0x0f, "Color can be changed with ANSI \x1b[9;me\x1b[10;ms\x1b[11;mc\x1b[12;ma\x1b[13;mp\x1b[14;me\x1b[0m code too.");
		bgfx::dbgTextPrintf(80, 1, 0x0f, "\x1b[;0m    \x1b[;1m    \x1b[; 2m    \x1b[; 3m    \x1b[; 4m    \x1b[; 5m    \x1b[; 6m    \x1b[; 7m    \x1b[0m");
		bgfx::dbgTextPrintf(80, 2, 0x0f, "\x1b[;8m    \x1b[;9m    \x1b[;10m    \x1b[;11m    \x1b[;12m    \x1b[;13m    \x1b[;14m    \x1b[;15m    \x1b[0m");
		bgfx::frame();
	}

	void CbgfxGraphicsApi::shutdown()
	{
		bgfx::shutdown();
		m_initialized = false;
	}

	void CbgfxGraphicsApi::submitPipeline(da::core::CGraphicsPipeline* pipeline)
	{

	}

	void CbgfxGraphicsApi::windowResize(const da::core::events::CEvent& event)
	{
		const da::core::events::CWindowResizeEvent* data = static_cast<const da::core::events::CWindowResizeEvent*>(&event);
		bgfx::setViewRect(0, 0, 0, data->getWidth(), data->getHeight());
		bgfx::reset(data->getWidth(), data->getHeight());
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