#include "dapch.h"
#include "physics_shape.h"

#include "physics_bullet3_shape.h"

namespace da::physics
{

	da::physics::CPhysicsShapeCube* CPhysicsShapeCube::create(const glm::vec3& extents)
	{
		return new da::physics::CBullet3Cube(extents);
	}

	da::physics::CPhysicsShapeSphere* CPhysicsShapeSphere::create(const float radius)
	{
		return new da::physics::CBullet3Sphere(radius);
	}

}