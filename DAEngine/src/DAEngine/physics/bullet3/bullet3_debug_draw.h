#pragma once
#if defined(DA_DEBUG) || defined(DA_RELEASE)
#include <bullet3/btBulletDynamicsCommon.h>

namespace da::physics
{
	class CBullet3DebugDraw : public btIDebugDraw
	{

	public:
		
		void drawLine(const btVector3& from, const btVector3& to, const btVector3& color) override;
		void drawContactPoint(const btVector3& PointOnB, const btVector3& normalOnB, btScalar distance, int lifeTime, const btVector3& color) override;
		void reportErrorWarning(const char* warningString) override;
		void draw3dText(const btVector3& location, const char* textString) override;
		void setDebugMode(int debugMode) override;
		int getDebugMode() const override;

	private:
		int m_debugMode = (btIDebugDraw::DebugDrawModes::DBG_FastWireframe | btIDebugDraw::DebugDrawModes::DBG_DrawContactPoints);;

	};

}
#endif