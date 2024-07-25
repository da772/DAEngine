#pragma once
#include <recast.h>
#include <DebugDraw.h>

namespace da::graphics
{
	class CStaticMesh;
}

class dtNavMeshQuery;
class dtNavMesh;
class dtCrowd;

namespace da::ai
{
	class NavCtx : public rcContext
	{

	protected:
		void doLog(const rcLogCategory category, const char* msg, const int len) override
		{
			switch (category)
			{
			case RC_LOG_PROGRESS:
				LOG_DEBUG(ELogChannel::AI, "%s", msg);
				break;
			case RC_LOG_WARNING:
				LOG_WARN(ELogChannel::AI, "%s", msg);
				break;
			case RC_LOG_ERROR:
				LOG_ERROR(ELogChannel::AI, "%s", msg);
				break;

			}
		}

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


	};
#endif

	class CNavCtx : public rcContext
	{

	protected:
		void doLog(const rcLogCategory category, const char* msg, const int len) override;

	};

	class INavMesh
	{
	public:
		virtual void initialize() = 0;
		virtual void update(float dt) = 0;
#ifdef DA_REVIEW
		virtual void debugRender() = 0;
#endif
		virtual void shutdown() = 0;
	};
}