#pragma once
#pragma once

#include "daengine/core/graphics/graphics_skmesh.h"
#include "DAEngine/core/factory.h"

namespace bgfx {
	struct VertexBufferHandle;
	struct IndexBufferHandle;
	struct VertexLayout;
}

namespace da::graphics
{
	class CSkeletalMeshFactory;
}

namespace da::platform {

	class CBgfxSkeletalMesh : public da::graphics::CSkeletalMesh
	{
	public:
		virtual void setBuffers(size_t index, uint8_t stream) override;
		inline size_t getMeshCount() const { return m_meshes.size(); };

		virtual const void* getNativeVBIndex(size_t index) const override;
		virtual const void* getNativeIBIndex(size_t index) const override;

	protected:
		CBgfxSkeletalMesh(const std::string& path, bool inverseNormals);
		virtual ~CBgfxSkeletalMesh();

	private:
		std::vector<::bgfx::VertexBufferHandle> m_vbh;
		std::vector<::bgfx::IndexBufferHandle>m_ibh;

		friend class da::core::CFactory<CSkeletalMesh>;
		friend class da::graphics::CSkeletalMeshFactory;

	public:
		static ::bgfx::VertexLayout getLayout();
	private:
		static ::bgfx::VertexLayout s_layout;
		static bool s_init;

	};

}