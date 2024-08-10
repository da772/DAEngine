
#include "bgfx_texture2d.h"
#ifdef DA_GRAPHICS_BGFX

#include <stb_image.h>
#include <bgfx/bgfx.h>
#include "core/threading/worker_pool.h"
#include <bimg/decode.h>
#include <bx/allocator.h>
#include <bx/error.h>
#include "bgfx_allocator.h"

namespace da::platform::bgfx {

	#define INVALID_HANDLE UINT16_MAX 


	CBgfxTexture2D::CBgfxTexture2D(const std::string& s) 
		: da::graphics::CGraphicsTexture2D(s), m_fileAsset(s), m_mutex(new std::mutex())
	{
		da::core::CWorkerPool::addJob([this] {
			PROFILE_TAG("Texture: ", m_fileAsset.path().c_str())
			std::lock_guard<std::mutex> guard(*m_mutex);

			if (m_path[m_path.size()-1] == 's' && m_path[m_path.size() - 2] == 'd' && m_path[m_path.size() - 3] == 'd')
			{
				bx::Error err;
				::bimg::ImageContainer* imageContainer = bimg::imageParse(DefaultAllocator::Get(), (const void*)m_fileAsset.data(), (uint32_t)m_fileAsset.size());

				m_width = imageContainer->m_width;
				m_height = imageContainer->m_height;

				if (::bgfx::isTextureValid(0, false, imageContainer->m_numLayers, ::bgfx::TextureFormat::Enum(imageContainer->m_format), 0))
				{

					const ::bgfx::Memory* mem = ::bgfx::copy(
						imageContainer->m_data
						, imageContainer->m_size
					);
					m_handle = ::bgfx::createTexture2D(
						uint16_t(imageContainer->m_width)
						, uint16_t(imageContainer->m_height)
						, 1 < imageContainer->m_numMips
						, imageContainer->m_numLayers
						, ::bgfx::TextureFormat::Enum(imageContainer->m_format)
						, 0
						, mem
					).idx;

					m_textureNative = &m_handle;
					bimg::imageFree(imageContainer);
				}

				LOG_ASSERT(m_handle != INVALID_HANDLE, ELogChannel::Graphics, "Failed to create texture %s", m_path.c_str());
				return;
			}

			stbi_uc* pixels = stbi_load_from_memory((const stbi_uc*)m_fileAsset.data(), m_fileAsset.size(), (int*)&m_width, (int*)&m_height, (int*)&m_channels, STBI_rgb_alpha);
			m_channels = 4;
			const ::bgfx::Memory* mem = ::bgfx::copy(pixels, m_width * m_height * m_channels * sizeof(char));
			m_handle = ::bgfx::createTexture2D(m_width, m_height, false, 1, ::bgfx::TextureFormat::Enum::RGBA8, 0, mem).idx;
			m_textureNative = &m_handle;
			LOG_ASSERT(m_handle != INVALID_HANDLE, ELogChannel::Graphics, "Failed to create texture %s", m_path.c_str());
			stbi_image_free(pixels);
		});
		
	}

	CBgfxTexture2D::CBgfxTexture2D(const std::string& name, uint32_t width, uint32_t height) 
		: da::graphics::CGraphicsTexture2D(name, width, height),m_fileAsset(name, width*height*4), m_mutex(new std::mutex())
	{
		memset((void*)m_fileAsset.data(), 0xff, width * height * 4);
		m_channels = 4;
		const ::bgfx::Memory* mem = ::bgfx::copy(m_fileAsset.data(), width * height * m_channels * sizeof(char));
		m_handle = ::bgfx::createTexture2D(width, height, false, 1, ::bgfx::TextureFormat::Enum::RGBA8, 0, mem).idx;
		m_textureNative = &m_handle;
		LOG_ASSERT(m_handle != INVALID_HANDLE, ELogChannel::Graphics, "Failed to create texture %s", name.c_str());
		m_fileAsset = {};
	}

	CBgfxTexture2D::CBgfxTexture2D() : CGraphicsTexture2D(""), m_mutex(new std::mutex())
	{

	}

	CBgfxTexture2D::CBgfxTexture2D(const std::string& name, uint32_t width, uint32_t height, char* data) : CGraphicsTexture2D(name), m_mutex(new std::mutex())
	{
		char* d = (char*)malloc(width);
		memcpy(d, data, width);
		da::core::CWorkerPool::addJob([this, width, height, d] {
			PROFILE_NAME("da::platform::bgfx::CBgfxTexture2D")
			PROFILE_TAG("Texture: ", m_fileAsset.path().c_str())
			std::lock_guard<std::mutex> guard(*m_mutex);
			stbi_uc* pixels = stbi_load_from_memory((const stbi_uc*)d, width, (int*)&m_width, (int*)&m_height, (int*)&m_channels, STBI_rgb_alpha);
			m_channels = 4;
			const ::bgfx::Memory* mem = ::bgfx::copy(pixels, m_width * m_height * m_channels * sizeof(char));
			m_handle = ::bgfx::createTexture2D(m_width, m_height, false, 1, ::bgfx::TextureFormat::Enum::RGBA8, 0, mem).idx;
			m_textureNative = &m_handle;
			LOG_ASSERT(m_handle != INVALID_HANDLE, ELogChannel::Graphics, "Failed to create texture %s", m_path.c_str());
			stbi_image_free(pixels);
		});
	}

	void CBgfxTexture2D::destroy()
	{
		{
			std::lock_guard<std::mutex> guard(*m_mutex);
		}

		delete m_mutex;
		m_mutex = nullptr;
		if (m_handle == INVALID_HANDLE) return;

		::bgfx::TextureHandle handle{ m_handle };
		::bgfx::destroy(handle);
		m_handle = INVALID_HANDLE;
	}

	CBgfxTexture2D::~CBgfxTexture2D()
	{
		destroy();
	}

}
#endif
