#pragma once

#include "DAEngine/core/graphics/graphics_material.h"
#ifdef DA_GRAPHICS_BGFX

namespace da::platform {

	class CBgfxGraphicsMaterial : public core::CMaterial
	{
	public:
		CBgfxGraphicsMaterial(const CString& vsPath, const CString& fsPath);
		virtual void initialize() override;
		virtual void update(int frame) override;
		virtual void shutdown() override;
		uint16_t getHandle() const;

	private:
		CString m_vsPath, m_fsPath;
		uint16_t m_program;
	};

}

#endif