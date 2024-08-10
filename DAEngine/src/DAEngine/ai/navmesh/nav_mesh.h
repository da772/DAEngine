#pragma once
#include "dacommon.h"
#include "daglm.h"
#include "dastd.h"

namespace da::graphics
{
	class CStaticMesh;
}

class dtNavMeshQuery;
class dtNavMesh;
class dtCrowd;

namespace da::ai
{
	class INavMesh
	{
	public:
		virtual void initialize() = 0;
		virtual void update(float dt) = 0;
		virtual std::vector<glm::vec3> findPath(const glm::vec3& startPos, const glm::vec3& endPos) = 0;
#ifdef DA_REVIEW
		virtual void debugRender() = 0;
#endif
		virtual void shutdown() = 0;
	};
}