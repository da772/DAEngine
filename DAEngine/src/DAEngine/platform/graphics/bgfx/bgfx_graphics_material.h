#pragma once

#include "graphics/graphics_material.h"
#include "graphics/factory/factory_graphics_material.h"

#ifdef DA_GRAPHICS_BGFX

namespace da {

	struct FShaderData {
		uint16_t Handle;
		uint16_t Count;
	};

	class CBgfxGraphicsMaterial : public da::CMaterial
	{
	public:
		virtual uint16_t getHandle() const override;

	protected:
		virtual void initialize() override;
		virtual void shutdown() override;

	protected:
		CBgfxGraphicsMaterial();
		CBgfxGraphicsMaterial(const std::string& vsPath, const std::string& fsPath, CHashString hash);
		CBgfxGraphicsMaterial(const std::string& csPath, CHashString hash);
		virtual ~CBgfxGraphicsMaterial();
		void getPlatformPath(std::string& s);
		uint16_t createShader(const std::string& s, CHashString hash);

	private:								 
		std::string m_vsPath, m_fsPath, m_csPath;
		uint16_t m_program;
		static std::unordered_map<CHashString, FShaderData> ms_shaderHandles;

		friend class da::CMaterialFactory;
	};

}

#endif