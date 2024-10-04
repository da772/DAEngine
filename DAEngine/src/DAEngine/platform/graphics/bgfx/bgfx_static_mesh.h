#pragma once

#include <bgfx/bgfx.h>
#include "graphics/graphics_smesh.h"
#include "core/factory.h"

namespace da
{
	class CStaticMeshFactory;
}

namespace da {

	class CBgfxStaticMesh : public da::CStaticMesh
	{
	public:
		void setBuffers(size_t index, uint8_t stream);
		inline size_t getMeshCount() const { return m_meshes.size(); };

		virtual const void* getNativeVBIndex(size_t index) const override;
		virtual const void* getNativeIBIndex(size_t index) const override;

protected:
		std::vector<::bgfx::VertexBufferHandle> m_vbh;
		std::vector<::bgfx::IndexBufferHandle>m_ibh;
		CBgfxStaticMesh(const std::string& path, bool process);
		CBgfxStaticMesh();
		virtual ~CBgfxStaticMesh();

	public:
		static ::bgfx::VertexLayout getLayout();
	private:

		static ::bgfx::VertexLayout s_layout;
		static bool s_init;

		friend class da::CFactory<da::CStaticMesh>;
		friend class da::CStaticMeshFactory;
	
	};

}