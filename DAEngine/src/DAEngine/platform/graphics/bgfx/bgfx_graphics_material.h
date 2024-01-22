#pragma once

#include "DAEngine/core/graphics/graphics_material.h"
#ifdef DA_GRAPHICS_BGFX

namespace da::platform {
	class CBgfxGraphicsMaterial : public graphics::CMaterial
	{
	public:
		CBgfxGraphicsMaterial();
		CBgfxGraphicsMaterial(const std::string& vsPath, const std::string& fsPath);
		CBgfxGraphicsMaterial(const std::string& csPath);
		virtual void initialize() override;
		virtual void update(int frame) override;
		virtual void shutdown() override;
		uint16_t getHandle() const;

	private:
		void getPlatformPath(std::string& s);

	private:								 
		std::string m_vsPath, m_fsPath, m_csPath;
		uint16_t m_program;
	};

}

#endif