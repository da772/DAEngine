#include "dapch.h"
#include "bgfx_skeletal_mesh.h"
#include "bgfx_util.h"

namespace da::platform
{
	bool CBgfxSkeletalMesh::s_init;
	::bgfx::VertexLayout CBgfxSkeletalMesh::s_layout;

	CBgfxSkeletalMesh::CBgfxSkeletalMesh(const std::string& path, bool inverseNormals) : CSkeletalMesh(path, inverseNormals)
	{
		m_vbh.reserve(m_meshes.size());
		m_ibh.reserve(m_meshes.size());
		for (size_t i = 0; i < m_meshes.size(); i++) {
			const da::graphics::FSkeletalMesh& mesh = m_meshes[i];

			::bgfx::VertexBufferHandle vbh = ::bgfx::createVertexBuffer(
				// Static data can be passed with ::bgfx::makeRef
				::bgfx::makeRef(mesh.Vertices.data(), mesh.Vertices.size() * sizeof(da::graphics::FSkeletalVertexBase))
				, CBgfxSkeletalMesh::getLayout()
				, BGFX_BUFFER_COMPUTE_TYPE_FLOAT
			);

			m_vbh.push_back(vbh);

			LOG_ASSERT(::bgfx::isValid(vbh), ELogChannel::Platform, "Failed to create VBH for %s", path.c_str());

			// Create static index buffer for triangle list rendering.
			::bgfx::IndexBufferHandle ibh = ::bgfx::createIndexBuffer(
				// Static data can be passed with ::bgfx::makeRef
				::bgfx::makeRef(mesh.Indices.data(), sizeof(uint32_t) * mesh.Indices.size())
				, BGFX_BUFFER_INDEX32
			);

			m_ibh.push_back(ibh);

			LOG_ASSERT(::bgfx::isValid(ibh), ELogChannel::Platform, "Failed to create IBH for %s", path.c_str());
		}
	}

	bgfx::VertexLayout CBgfxSkeletalMesh::getLayout()
	{
		if (s_init) return s_layout;

		s_init = true;
		s_layout.begin()
			.add(::bgfx::Attrib::Position, 3, ::bgfx::AttribType::Float)
			.add(::bgfx::Attrib::Normal, 3, ::bgfx::AttribType::Float)
			.add(::bgfx::Attrib::Tangent, 3, ::bgfx::AttribType::Float)
			.add(::bgfx::Attrib::TexCoord0, 2, ::bgfx::AttribType::Float)
			.add(::bgfx::Attrib::Indices, 4, ::bgfx::AttribType::Float)
			.add(::bgfx::Attrib::Weight, 4, ::bgfx::AttribType::Float)
			.end();
		return s_layout;
	}

	CBgfxSkeletalMesh::~CBgfxSkeletalMesh()
	{
		for (size_t i = 0; i < m_vbh.size(); i++) {
			BGFXDESTROY(m_vbh[i]);
		}

		for (size_t i = 0; i < m_ibh.size(); i++) {
			BGFXDESTROY(m_ibh[i]);
		}

	}

	void CBgfxSkeletalMesh::setBuffers(size_t index, uint8_t stream)
	{
		::bgfx::setVertexBuffer(stream, m_vbh[index]);
		::bgfx::setIndexBuffer(m_ibh[index]);
	}

	const void* CBgfxSkeletalMesh::getNativeVBIndex(size_t index) const
	{
		ASSERT(m_vbh.size() >= index);
		return &m_vbh[index];
	}

	const void* CBgfxSkeletalMesh::getNativeIBIndex(size_t index) const
	{
		ASSERT(m_ibh.size() >= index);
		return &m_ibh[index];
	}

}