#pragma once
#include "bgfx_static_mesh.h"

namespace da::platform
{

#define MAX_LINES 100000
#define VERTEX_COUNT MAX_LINES*4
#define INDEX_COUNT MAX_LINES*6

	struct FTransientBufferData
	{
		::bgfx::DynamicVertexBufferHandle vbh;
		::bgfx::DynamicIndexBufferHandle ibh;

	};

	class CBgfxLineMesh : public CBgfxStaticMesh
	{
	public:
		CBgfxLineMesh();

		void addTransientLine(const glm::vec3& startPos, const glm::vec3& endPos, const glm::vec4& color, float width);
		void clearAll();
		void setBufferData() const;

		virtual ~CBgfxLineMesh();

	private:
		::bgfx::DynamicVertexBufferHandle m_vbh = BGFX_INVALID_HANDLE;
		::bgfx::DynamicIndexBufferHandle m_ibh = BGFX_INVALID_HANDLE;
		std::array<da::graphics::FVertexBase, VERTEX_COUNT> m_dynamicMeshes;
		std::array<uint32_t, INDEX_COUNT> m_dynamicIndices;
		uint32_t m_vertexCount = 0;
		uint32_t m_indexCount = 0;

	};
}