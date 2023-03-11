#include "dapch.h"
#include "bgfx_graphics_material.h"

#ifdef DA_GRAPHICS_BGFX
#include <bgfx/bgfx.h>

#include "DAEngine/asset/asset.h"

namespace da::platform {

	CBgfxGraphicsMaterial::CBgfxGraphicsMaterial(const CString& vsPath, const CString& fsPath) : m_vsPath(vsPath), m_fsPath(fsPath)
	{

	}

	void CBgfxGraphicsMaterial::initialize()
	{
		CAsset vs(m_vsPath), fs(m_fsPath);

		const bgfx::Memory* vsM = bgfx::copy(vs.data(), (uint32_t)(vs.size() + 1ull));
		const bgfx::Memory* fsM = bgfx::copy(fs.data(), (uint32_t)(fs.size() + 1ull));

		bgfx::ShaderHandle handle1 = bgfx::createShader(vsM);
		bgfx::setName(handle1, m_vsPath.cstr());

		bgfx::ShaderHandle handle2 = bgfx::createShader(fsM);
		bgfx::setName(handle2, m_fsPath.cstr());

		m_program = bgfx::createProgram(handle1, handle2, true).idx;
	}

	void CBgfxGraphicsMaterial::update(int frame)
	{

	}

	void CBgfxGraphicsMaterial::shutdown()
	{
		bgfx::destroy(bgfx::ProgramHandle(m_program ));
	}

	uint16_t CBgfxGraphicsMaterial::getHandle() const
	{
		return m_program;
	}

}

#endif