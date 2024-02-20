#include "dapch.h"

#include "bgfx_line_mesh.h"
#include "bgfx_util.h"

namespace da::platform {

	CBgfxLineMesh::CBgfxLineMesh() : CBgfxStaticMesh()
	{
		memset(m_dynamicMeshes.data(), 0, sizeof(da::graphics::FVertexBase) * m_dynamicMeshes.size());
		memset(m_dynamicIndices.data(), 0, sizeof(uint32_t) * m_dynamicMeshes.size());
	}	

	CBgfxLineMesh::~CBgfxLineMesh()
	{
		BGFXTRYDESTROY(m_vbh);
		BGFXTRYDESTROY(m_ibh);
	}

	void CBgfxLineMesh::addTransientLine(const glm::vec3& startPos, const glm::vec3& endPos, const glm::vec4& color, float width)
	{
		if (m_indexCount >= m_dynamicIndices.size() || m_vertexCount >= m_dynamicMeshes.size()) {
			return;
		}

		glm::vec3 forwardVec = glm::normalize(startPos - endPos);
		width *= .5f;
		//PERPENDICULAR VEC
		glm::vec3 fVec = { 0.f, 1.f, 0.f };

		if (forwardVec == -fVec) {
			fVec = { 0.f, 0.f, 1.f };
		}

		glm::vec3 perpVec = cross(forwardVec, fVec);
		perpVec = normalize(perpVec);

		//MID POINT
		glm::vec3 midP = (startPos + endPos) / 2.f;

		// GEN POINTS
		glm::vec3 n1 = startPos + (width * perpVec);
		glm::vec3 n2 = startPos - (width * perpVec);
		glm::vec3 n3 = endPos + (width * perpVec);
		glm::vec3 n4 = endPos - (width * perpVec);

		da::graphics::FVertexBase p1, p2, p3, p4;
		p1.Pos = Vector3f(&n1[0]);
		p1.Normal = { color.x, color.y, color.z };
		p2.Pos = Vector3f(&n2[0]);
		p2.Normal = { color.x, color.y, color.z };
		p3.Pos = Vector3f(&n3[0]);
		p3.Normal = { color.x, color.y, color.z };
		p4.Pos = Vector3f(&n4[0]);
		p4.Normal = { color.x, color.y, color.z };

		uint32_t vrtx = m_vertexCount;

		m_dynamicMeshes[m_vertexCount++] = p1;
		m_dynamicMeshes[m_vertexCount++] = p2;
		m_dynamicMeshes[m_vertexCount++] = p3;
		m_dynamicMeshes[m_vertexCount++] = p4;



		m_dynamicIndices[m_indexCount++] = vrtx+3;
		m_dynamicIndices[m_indexCount++] = vrtx+1;
		m_dynamicIndices[m_indexCount++] = vrtx+2;
		m_dynamicIndices[m_indexCount++] = vrtx;
		m_dynamicIndices[m_indexCount++] = vrtx+1;
		m_dynamicIndices[m_indexCount++] = vrtx+2;

		if (!::bgfx::isValid(m_vbh))
		{
			m_vbh = ::bgfx::createDynamicVertexBuffer(
				::bgfx::makeRef(m_dynamicMeshes.data(), sizeof(da::graphics::FVertexBase)*m_dynamicMeshes.size()),
				CBgfxStaticMesh::getLayout()
				, BGFX_BUFFER_COMPUTE_TYPE_FLOAT
			);

			m_ibh = ::bgfx::createDynamicIndexBuffer(
				::bgfx::makeRef(m_dynamicIndices.data(), sizeof(uint32_t)*m_dynamicIndices.size())
				, BGFX_BUFFER_INDEX32
			);
		}
	}

	void CBgfxLineMesh::clearAll()
	{
		if (!::bgfx::isValid(m_vbh))
		{
			return;
		}

		m_indexCount = 0;
		m_vertexCount = 0;
		::bgfx::update(m_vbh, 0, ::bgfx::makeRef(m_dynamicMeshes.data(), sizeof(da::graphics::FVertexBase)));
		::bgfx::update(m_ibh, 0, ::bgfx::makeRef(m_dynamicIndices.data(), sizeof(uint32_t)));
	}

	void CBgfxLineMesh::setBufferData() const
	{
		if (!::bgfx::isValid(m_vbh))
		{
			return;
		}

		if (!m_vertexCount) return;

		::bgfx::update(m_vbh, 0, ::bgfx::makeRef(m_dynamicMeshes.data(), sizeof(da::graphics::FVertexBase) * m_vertexCount));
		::bgfx::update(m_ibh, 0, ::bgfx::makeRef(m_dynamicIndices.data(), sizeof(uint32_t) * m_indexCount));
		::bgfx::setIndexBuffer(m_ibh, 0, m_indexCount);
		::bgfx::setVertexBuffer(0, m_vbh, 0, sizeof(da::graphics::FVertexBase) * m_vertexCount);
	}

}