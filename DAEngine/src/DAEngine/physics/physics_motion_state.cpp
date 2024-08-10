
#include "physics_motion_state.h"

#include "bullet3/physics_bullet3_motion_state.h"

namespace da::physics
{
	da::physics::CPhysicsDefaultMotionState* CPhysicsDefaultMotionState::create(const glm::mat4& transform)
	{
		return new da::physics::CBullet3DefaultMotionState(transform);
	}

	da::physics::CPhysicsEntityMotionState* CPhysicsEntityMotionState::create(da::core::CEntity* entity)
	{
		return new da::physics::CBullet3EntityMotionState(entity);
	}

}