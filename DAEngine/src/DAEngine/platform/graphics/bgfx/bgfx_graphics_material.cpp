#include "dapch.h"
#include "bgfx_graphics_material.h"

#ifdef DA_GRAPHICS_BGFX
#include <bgfx/bgfx.h>

#include "DAEngine/asset/asset.h"
#include "DAEngine/platform/graphics/bgfx/bgfx_graphics_api.h"

namespace da::platform {



	CBgfxGraphicsMaterial::CBgfxGraphicsMaterial(const std::string& vsPath, const std::string& fsPath) : m_vsPath(vsPath), m_fsPath(fsPath)
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
				m_vsPath.append(".dx");
				m_fsPath.append(".dx");
				break;
			case ERenderApis::Metal:
				m_vsPath.append(".mt");
				m_fsPath.append(".mt");
				break;
			case ERenderApis::Vulkan:
				m_vsPath.append(".vk");
				m_fsPath.append(".vk");
				break;
			case ERenderApis::OpenGL:
				m_vsPath.append(".gl");
				m_fsPath.append(".gl");
				break;
		}

		for (size_t i = m_vsPath.size() - 1; i >= 0; i--) {
			if (m_vsPath[i] == '\\' || m_vsPath[i] == '/') {
				m_vsPath.insert(i + 1, platform + "/");
				break;
			}
		}

		for (size_t i = m_fsPath.size() - 1; i >= 0; i--) {
			if (m_fsPath[i] == '\\' || m_fsPath[i] == '/') {
				m_fsPath.insert(i + 1, platform + "/");
				break;
			}
		}

	}

	void CBgfxGraphicsMaterial::initialize()
	{
		CAsset vs(m_vsPath.c_str()), fs(m_fsPath.c_str());

		const bgfx::Memory* vsM = bgfx::copy(vs.data(), (uint32_t)(vs.size() + 1ull));
		const bgfx::Memory* fsM = bgfx::copy(fs.data(), (uint32_t)(fs.size() + 1ull));

		bgfx::ShaderHandle handle1 = bgfx::createShader(vsM);
		bgfx::setName(handle1, m_vsPath.c_str());

		bgfx::ShaderHandle handle2 = bgfx::createShader(fsM);
		bgfx::setName(handle2, m_fsPath.c_str());

		m_program = bgfx::createProgram(handle1, handle2, true).idx;
	}

	void CBgfxGraphicsMaterial::update(int frame)
	{

	}

	void CBgfxGraphicsMaterial::shutdown()
	{
        bgfx::ProgramHandle handle = {m_program};
		bgfx::destroy(handle);
	}

	uint16_t CBgfxGraphicsMaterial::getHandle() const
	{
		return m_program;
	}

}

#endif
