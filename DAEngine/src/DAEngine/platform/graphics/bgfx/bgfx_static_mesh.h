#pragma once

#ifndef DA_TEST
#include <bgfx/bgfx.h>
#include "core/graphics/graphics_smesh.h"

namespace da::platform {

	class CBgfxStaticMesh : public da::core::CStaticMesh
	{
	public:

		CBgfxStaticMesh(const std::string& path);
		virtual ~CBgfxStaticMesh();
		inline virtual void* getNativeVB() const override;
		inline virtual void* getNativeIB() const override;

		void setBuffers(uint8_t stream);

	private:
		::bgfx::VertexBufferHandle m_vbh;
		::bgfx::IndexBufferHandle m_ibh;

	public:
		static ::bgfx::VertexLayout getLayout();
	private:
		static ::bgfx::VertexLayout s_layout;
		static bool s_init;
	
	};

}
#endif