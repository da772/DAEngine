
#include "physics/physics_motion_state.h"

#include "physics_bullet3_motion_state.h"

namespace da
{
	da::CPhysicsDefaultMotionState* CPhysicsDefaultMotionState::create(const glm::mat4& transform)
	{
		return new da::CBullet3DefaultMotionState(transform);
	}

	da::CPhysicsEntityMotionState* CPhysicsEntityMotionState::create(da::CEntity* entity)
	{
		return new da::CBullet3EntityMotionState(entity);
	}

}