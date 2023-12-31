#include "dapch.h"
#include "bgfx_static_mesh.h"
#include "bgfx_util.h"

namespace da::platform
{

	bool CBgfxStaticMesh::s_init;
	::bgfx::VertexLayout CBgfxStaticMesh::s_layout;

	CBgfxStaticMesh::CBgfxStaticMesh(const std::string& path) : CStaticMesh(path)
	{
		m_vbh = ::bgfx::createVertexBuffer(
			// Static data can be passed with ::bgfx::makeRef
			::bgfx::makeRef(getVertices().data(), getVertices().size() * sizeof(da::core::FVertexBase))
			, CBgfxStaticMesh::getLayout()
			, BGFX_BUFFER_COMPUTE_TYPE_FLOAT
		);

		LOG_ASSERT(::bgfx::isValid(m_vbh), ELogChannel::Platform, "Failed to create VBH for %s", path.c_str());

		// Create static index buffer for triangle list rendering.
		m_ibh = ::bgfx::createIndexBuffer(
			// Static data can be passed with ::bgfx::makeRef
			::bgfx::makeRef(getIndices().data(), sizeof(uint32_t) * getIndices().size())
			, BGFX_BUFFER_INDEX32
		);

		LOG_ASSERT(::bgfx::isValid(m_ibh), ELogChannel::Platform, "Failed to create IBH for %s", path.c_str());
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

	void* CBgfxStaticMesh::getNativeIB() const
	{
		return (void*) &m_ibh;
	}

	void* CBgfxStaticMesh::getNativeVB() const
	{
		return (void*)&m_vbh;
	}

	CBgfxStaticMesh::~CBgfxStaticMesh()
	{
		BGFXDESTROY(m_ibh);
		BGFXDESTROY(m_vbh);
	}

	void CBgfxStaticMesh::setBuffers(uint8_t stream)
	{
		::bgfx::setVertexBuffer(stream, m_vbh);
		::bgfx::setIndexBuffer(m_ibh);
	}

}