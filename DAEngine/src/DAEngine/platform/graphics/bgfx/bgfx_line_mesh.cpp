#include "dapch.h"

#include "bgfx_line_mesh.h"

namespace da::platform {

	CBgfxLineMesh::CBgfxLineMesh() : CBgfxStaticMesh()
	{
	
	}	

	CBgfxLineMesh::~CBgfxLineMesh()
	{

	}

	FTransientBufferData CBgfxLineMesh::addTransientLine(const glm::vec3& startPos, const glm::vec3& endPos, float width)
	{
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

		da::graphics::FMesh mesh;

		da::graphics::FVertexBase p1, p2, p3, p4;
		p1.Pos = Vector3f(&n1[0]);
		p2.Pos = Vector3f(&n2[0]);
		p3.Pos = Vector3f(&n3[0]);
		p4.Pos = Vector3f(&n4[0]);

		mesh.Vertices.push_back(p1);
		mesh.Vertices.push_back(p2);
		mesh.Vertices.push_back(p3);
		mesh.Vertices.push_back(p4);

		mesh.Indices.push_back(3);
		mesh.Indices.push_back(1);
		mesh.Indices.push_back(2);
		mesh.Indices.push_back(0);
		mesh.Indices.push_back(1);
		mesh.Indices.push_back(2);

		::bgfx::TransientVertexBuffer* vbh = new ::bgfx::TransientVertexBuffer();
		::bgfx::allocTransientVertexBuffer(vbh, mesh.Vertices.size(), CBgfxStaticMesh::getLayout());
		memcpy(vbh->data, mesh.Vertices.data(), mesh.Vertices.size() * sizeof(da::graphics::FVertexBase));

		LOG_ASSERT(::bgfx::isValid(vbh->handle), ELogChannel::Platform, "Failed to create VBH for LineMesh");

		// Create static index buffer for triangle list rendering.
		::bgfx::TransientIndexBuffer* ibh = new ::bgfx::TransientIndexBuffer();
		::bgfx::allocTransientIndexBuffer(ibh, mesh.Indices.size(), true);
		memcpy(ibh->data, mesh.Indices.data(), sizeof(uint32_t) * mesh.Indices.size());
		
		LOG_ASSERT(::bgfx::isValid(ibh->handle), ELogChannel::Platform, "Failed to create IBH for LineMesh");


		return { vbh, ibh };
	}


}