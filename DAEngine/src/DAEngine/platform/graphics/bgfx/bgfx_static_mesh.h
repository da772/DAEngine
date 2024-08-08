#pragma once

#include <bgfx/bgfx.h>
#include "core/graphics/graphics_smesh.h"
#include "core/factory.h"

namespace da::graphics
{
	class CStaticMeshFactory;
}

namespace da::platform {

	class CBgfxStaticMesh : public da::graphics::CStaticMesh
	{
	public:
		void setBuffers(size_t index, uint8_t stream);
		inline size_t getMeshCount() const { return m_meshes.size(); };

		virtual const void* getNativeVBIndex(size_t index) const override;
		virtual const void* getNativeIBIndex(size_t index) const override;

protected:
		std::vector<::bgfx::VertexBufferHandle> m_vbh;
		std::vector<::bgfx::IndexBufferHandle>m_ibh;
		CBgfxStaticMesh(const std::string& path, bool inverseNormals);
		CBgfxStaticMesh();
		virtual ~CBgfxStaticMesh();

	public:
		static ::bgfx::VertexLayout getLayout();
	private:

		static ::bgfx::VertexLayout s_layout;
		static bool s_init;

		friend class da::core::CFactory<da::graphics::CStaticMesh>;
		friend class da::graphics::CStaticMeshFactory;
	
	};

}