#pragma once

#include <bgfx/bgfx.h>
#include "core/graphics/graphics_smesh.h"

namespace da::platform {

	class CBgfxStaticMesh : public da::graphics::CStaticMesh
	{
	public:

		CBgfxStaticMesh(const std::string& path, bool inverseNormals);
		virtual ~CBgfxStaticMesh();

		void setBuffers(size_t index, uint8_t stream);
		inline size_t getMeshCount() const { return m_meshes.size(); };

		virtual const void* getNativeVBIndex(size_t index) const override;
		virtual const void* getNativeIBIndex(size_t index) const override;

	private:
		std::vector<::bgfx::VertexBufferHandle> m_vbh;
		std::vector<::bgfx::IndexBufferHandle>m_ibh;

	public:
		static ::bgfx::VertexLayout getLayout();
	private:
		static ::bgfx::VertexLayout s_layout;
		static bool s_init;
	
	};

}