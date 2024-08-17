#pragma once
#include "dacommon.h"
#include "daglm.h"
#include "dastd.h"

#include <recast.h>
#include <DebugDraw.h>

namespace da::ai
{
	class CNavMeshManager;

	class NavCtx : public rcContext
	{

	protected:
		void doLog(const rcLogCategory category, const char* msg, const int len) override;

	};
#ifdef DA_REVIEW
	class CNavDrawer : public duDebugDraw
	{

	public:
		void depthMask(bool state) override;
		void texture(bool state) override;
		void begin(duDebugDrawPrimitives prim, float size = 1.0f) override;
		void vertex(const float* pos, unsigned int color) override;
		void vertex(const float x, const float y, const float z, unsigned int color) override;
		void vertex(const float* pos, unsigned int color, const float* uv) override;
		void vertex(const float x, const float y, const float z, unsigned int color, const float u, const float v) override;
		void end() override;

		

	private:
		duDebugDrawPrimitives m_type;
		std::vector<glm::vec3> m_vertices;
		glm::vec4 m_color;

		static CNavDrawer ms_drawer;

		friend class CNavMeshManager;
	};
#endif

	class CNavCtx : public rcContext
	{
	protected:
		void doLog(const rcLogCategory category, const char* msg, const int len) override;

		static CNavCtx ms_ctx;
		friend class CNavMeshManager;
	};

}