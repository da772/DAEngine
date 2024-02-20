#pragma once

#include "../physics_motion_state.h"
#include "bullet3_physics.h"

namespace da::physics
{
	class CBullet3MotionState : public IPhysicsMotionState
	{
		public:
		btMotionState* getMotionState() const;

	protected:
		inline CBullet3MotionState() : IPhysicsMotionState() {};
		btMotionState* m_motionState;
	};

	class CBullet3DefaultMotionState : public CBullet3MotionState, public CPhysicsDefaultMotionState
	{
	public:
		CBullet3DefaultMotionState(const glm::mat4& transform);
		virtual ~CBullet3DefaultMotionState();
	};

	class CBullet3EntityMotionState : public CBullet3MotionState, public CPhysicsEntityMotionState, public btMotionState
	{
	public:
		CBullet3EntityMotionState(da::core::CEntity* entity);
		virtual ~CBullet3EntityMotionState();

		void getWorldTransform(btTransform& worldTrans) const override;
		void setWorldTransform(const btTransform& worldTrans) override;
		
	private:
		glm::mat4 m_finalTransform;
	};
}