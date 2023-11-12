#include "dapch.h"
#ifdef DA_GRAPHICS_BGFX

#include "bgfx_texture2d.h"
#include <stb_image.h>
#include <bgfx/bgfx.h>

namespace da::platform::bgfx {

	#define INVALID_HANDLE UINT16_MAX 


	CBgfxTexture2D::CBgfxTexture2D(const std::string& s) 
		: da::core::CGraphicsTexture2D(s), m_fileAsset(s)
	{
		stbi_uc* pixels = stbi_load_from_memory((const stbi_uc*)m_fileAsset.data(), m_fileAsset.size(), (int*)&m_width, (int*)&m_height, (int*)&m_channels, STBI_rgb_alpha);
		m_channels = 4;
		const ::bgfx::Memory * mem = ::bgfx::copy(pixels, m_width * m_height * m_channels * sizeof(char));
		m_handle = ::bgfx::createTexture2D(m_width, m_height, false, 1, ::bgfx::TextureFormat::Enum::RGBA8, 0, mem).idx;
		m_textureNative = &m_handle;
		LOG_ASSERT(m_handle != INVALID_HANDLE, ELogChannel::Graphics, "Failed to create texture %s", m_path.c_str());
		stbi_image_free(pixels);
	}

	CBgfxTexture2D::CBgfxTexture2D(const std::string& name, uint32_t width, uint32_t height) 
		: da::core::CGraphicsTexture2D(name, width, height),m_fileAsset(name, width*height*4)
	{
		memset((void*)m_fileAsset.data(), 0xff, width * height * 4);
		m_channels = 4;
		const ::bgfx::Memory* mem = ::bgfx::copy(m_fileAsset.data(), width * height * m_channels * sizeof(char));
		m_handle = ::bgfx::createTexture2D(width, height, false, 1, ::bgfx::TextureFormat::Enum::RGBA8, 0, mem).idx;
		m_textureNative = &m_handle;
		LOG_ASSERT(m_handle != INVALID_HANDLE, ELogChannel::Graphics, "Failed to create texture %s", name.c_str());
		m_fileAsset = {};
	}

	CBgfxTexture2D::CBgfxTexture2D() : CGraphicsTexture2D("")
	{

	}

	void CBgfxTexture2D::destroy()
	{
		if (m_handle == INVALID_HANDLE) return;

		::bgfx::TextureHandle handle{ m_handle };
		::bgfx::destroy(handle);
		m_handle = INVALID_HANDLE;
	}

}
#endif
