#pragma once
#include "bgfx_static_mesh.h"

namespace da::platform
{

	struct FTransientBufferData
	{
		::bgfx::TransientVertexBuffer* pVBH;
		::bgfx::TransientIndexBuffer* pIBH;

	};

	class CBgfxLineMesh : public CBgfxStaticMesh
	{
	public:
		CBgfxLineMesh();

		FTransientBufferData addTransientLine(const glm::vec3& startPos, const glm::vec3& endPos, float width);

		virtual ~CBgfxLineMesh();
	};
}