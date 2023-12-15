#include "dapch.h"
#include "bgfx_graphics_material.h"

#ifdef DA_GRAPHICS_BGFX
#include <bgfx/bgfx.h>

#include "DAEngine/asset/asset.h"
#include "DAEngine/platform/graphics/bgfx/bgfx_graphics_api.h"

namespace da::platform {



	CBgfxGraphicsMaterial::CBgfxGraphicsMaterial(const std::string& vsPath, const std::string& fsPath) : m_vsPath(vsPath), m_fsPath(fsPath)
	{
		getPlatformPath(m_vsPath);
		getPlatformPath(m_fsPath);
	}

	CBgfxGraphicsMaterial::CBgfxGraphicsMaterial(const std::string& csPath) : m_csPath(csPath)
	{
		getPlatformPath(m_csPath);
	}

	CBgfxGraphicsMaterial::CBgfxGraphicsMaterial()
	{

	}

	void CBgfxGraphicsMaterial::initialize()
	{
		if (!m_csPath.empty()) {
			CAsset cs(m_csPath);

			const ::bgfx::Memory* csM = ::bgfx::copy(cs.data(), (uint32_t)(cs.size() + 1ull));

			::bgfx::ShaderHandle handle1 = ::bgfx::createShader(csM);
			::bgfx::setName(handle1, m_csPath.c_str());
			m_program = ::bgfx::createProgram(handle1, true).idx;
			return;
		}

		CAsset vs(m_vsPath.c_str()), fs(m_fsPath.c_str());

		const ::bgfx::Memory* vsM = ::bgfx::copy(vs.data(), (uint32_t)(vs.size() + 1ull));
		const ::bgfx::Memory* fsM = ::bgfx::copy(fs.data(), (uint32_t)(fs.size() + 1ull));

		::bgfx::ShaderHandle handle1 = ::bgfx::createShader(vsM);
		::bgfx::setName(handle1, m_vsPath.c_str());

		::bgfx::ShaderHandle handle2 = ::bgfx::createShader(fsM);
		::bgfx::setName(handle2, m_fsPath.c_str());

		m_program = ::bgfx::createProgram(handle1, handle2, true).idx;
	}

	void CBgfxGraphicsMaterial::update(int frame)
	{
		
	}

	void CBgfxGraphicsMaterial::shutdown()
	{
        ::bgfx::ProgramHandle handle = {m_program};
		::bgfx::destroy(handle);
	}

	void CBgfxGraphicsMaterial::getPlatformPath(std::string& s)
	{
		std::string platform;
#ifdef DA_PLATFORM_WINDOWS
		platform = "windows";
#elif defined(DA_PLATFORM_MACOSX)
		platform = "macosx";
#elif defined (DA_PLATFORM_LINUX)
		platform = "linux";
#endif

		switch (CbgfxGraphicsApi::getRendererApi())
		{
		case ERenderApis::D3D9:
		case ERenderApis::D3D11:
		case ERenderApis::D3D12:
			s.append(".dx");
			break;
		case ERenderApis::Metal:
			s.append(".mt");
			break;
		case ERenderApis::Vulkan:
			s.append(".vk");
			break;
		case ERenderApis::OpenGL:
			s.append(".gl");
			break;
		}

		for (size_t i = s.size() - 1; i >= 0; i--) {
			if (s[i] == '\\' || s[i] == '/') {
				s.insert(i + 1, platform + "/");
				break;
			}
		}
	}


	uint16_t CBgfxGraphicsMaterial::getHandle() const
	{
		return m_program;
	}

}

#endif
