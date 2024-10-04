#include "graphics/factory/factory_graphics_texture2d.h"

#ifdef DA_GRAPHICS_VULKAN
#include "platform/graphics/vulkan/vulkan_graphics_texture2d.h"
#endif

#ifdef DA_GRAPHICS_BGFX
#include "platform/graphics/bgfx/bgfx_texture2d.h"
#endif

namespace da
{
	CTexture2DFactory CTexture2DFactory::ms_factory;

	CGraphicsTexture2D* CTexture2DFactory::Create(const std::string& path, CGraphicsApi& graphicsApi)
	{
		CHashString hash(path.c_str(), path.size());

#ifdef DA_GRAPHICS_BGFX
		return ms_factory.addInstance(hash, [path] {return new da::CBgfxTexture2D(path); });
#elif defined(DA_GRAPHICS_VULKAN)
		return ms_factory.addInstance(hash, [path] {return new da::CVulkanGraphicsTexture2D(path, graphicsApi); });
#endif
	}

	da::CGraphicsTexture2D* CTexture2DFactory::Create(const std::string& path)
	{
		CHashString hash(path.c_str(), path.size());

#ifdef DA_GRAPHICS_BGFX
		return ms_factory.addInstance(hash, [path] {return new da::CBgfxTexture2D(path); });
#endif
	}

	da::CGraphicsTexture2D* CTexture2DFactory::Create(const std::string& name, size_t width, size_t height, char* data)
	{
		CHashString hash(name.c_str(), name.size());
#ifdef DA_GRAPHICS_BGFX
		return ms_factory.addInstance(hash, [name, width, height, data] {return new da::CBgfxTexture2D(name, width, height, data); });;
#endif
	}

	void CTexture2DFactory::Remove(CGraphicsTexture2D* texture)
	{
		// store hash in texture pls
		CHashString hash(texture->getPath().c_str(), texture->getPath().size());
		ms_factory.removeInstance(hash);
	}

}