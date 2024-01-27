#include "dapch.h"
#include "bgfx_static_mesh.h"
#include "bgfx_util.h"

namespace da::platform
{

	bool CBgfxStaticMesh::s_init;
	::bgfx::VertexLayout CBgfxStaticMesh::s_layout;

	CBgfxStaticMesh::CBgfxStaticMesh(const std::string& path, bool inverseNormals) : CStaticMesh(path, inverseNormals)
	{
		m_vbh.reserve(m_meshes.size());
		m_ibh.reserve(m_meshes.size());
		for (size_t i = 0; i < m_meshes.size(); i++) {
			const da::graphics::FMesh& mesh = m_meshes[i];

			::bgfx::VertexBufferHandle vbh = ::bgfx::createVertexBuffer(
				// Static data can be passed with ::bgfx::makeRef
				::bgfx::makeRef(mesh.Vertices.data(), mesh.Vertices.size() * sizeof(da::graphics::FVertexBase))
				, CBgfxStaticMesh::getLayout()
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

	bgfx::VertexLayout CBgfxStaticMesh::getLayout()
	{
		if (s_init) return s_layout;

		s_init = true;
		s_layout.begin()
			.add(::bgfx::Attrib::Position, 3, ::bgfx::AttribType::Float)
			.add(::bgfx::Attrib::Normal, 3, ::bgfx::AttribType::Float)
			.add(::bgfx::Attrib::Tangent, 3, ::bgfx::AttribType::Float)
			.add(::bgfx::Attrib::TexCoord0, 2, ::bgfx::AttribType::Float)
			.end();
		return s_layout;
	}

	CBgfxStaticMesh::~CBgfxStaticMesh()
	{
		for (size_t i = 0; i < m_vbh.size(); i++) {
			BGFXDESTROY(m_vbh[i]);
		}

		for (size_t i = 0; i < m_ibh.size(); i++) {
			BGFXDESTROY(m_ibh[i]);
		}
		
	}

	void CBgfxStaticMesh::setBuffers(size_t index, uint8_t stream)
	{
		::bgfx::setVertexBuffer(stream, m_vbh[index]);
		::bgfx::setIndexBuffer(m_ibh[index]);
	}

	const void* CBgfxStaticMesh::getNativeVBIndex(size_t index) const
	{
		ASSERT(m_vbh.size() >= index);
		return &m_vbh[index];
	}

	const void* CBgfxStaticMesh::getNativeIBIndex(size_t index) const
	{
		ASSERT(m_ibh.size() >= index);
		return &m_ibh[index];
	}

}