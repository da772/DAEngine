
#if defined(DA_DEBUG) || defined(DA_RELEASE)
#include "bullet3_debug_draw.h"
#include "debug/graphics_debug_render.h"

namespace da
{
	void CBullet3DebugDraw::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
	{
		da::CDebugRender::drawLine({ from.x(), from.y(), from.z() }, { to.x(), to.y(), to.z() }, .025f, { color.x(), color.y(), color.z(), color.w() }, false, true);
	}

	void CBullet3DebugDraw::drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color)
	{
		da::CDebugRender::drawCube({ PointOnB.x(), PointOnB.y(), PointOnB.z() }, glm::quat(), glm::vec3(.025f), {color.x(), color.y(), color.z(), color.w()}, true, true);

		btVector3 endPos = normalOnB * distance;
		da::CDebugRender::drawLine({ PointOnB.x(), PointOnB.y(), PointOnB.z() }, { endPos.x(), endPos.y(), endPos.z() }, .025f, { color.x(), color.y(), color.z(), color.w() }, false, true);
	}

	void CBullet3DebugDraw::reportErrorWarning(const char* warningString)
	{
		LOG_ERROR(ELogChannel::Physics, "%s", warningString);
	}

	void CBullet3DebugDraw::draw3dText(const btVector3& location, const char* textString)
	{
		
	}

	void CBullet3DebugDraw::setDebugMode(int debugMode)
	{
		m_debugMode = debugMode;
	}

	int CBullet3DebugDraw::getDebugMode() const
	{
		return m_debugMode;
	}

}
#endif