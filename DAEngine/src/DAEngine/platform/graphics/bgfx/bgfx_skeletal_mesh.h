#pragma once
#pragma once

#include "daengine/core/graphics/graphics_skmesh.h"

namespace bgfx {
	struct VertexBufferHandle;
	struct IndexBufferHandle;
	struct VertexLayout;
}


namespace da::platform {

	class CBgfxSkeletalMesh : public da::graphics::CSkeletalMesh
	{
	public:

		CBgfxSkeletalMesh(const std::string& path, bool inverseNormals);
		virtual ~CBgfxSkeletalMesh();

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