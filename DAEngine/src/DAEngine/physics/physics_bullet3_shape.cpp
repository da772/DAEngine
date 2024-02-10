#include "dapch.h"

#include "physics_bullet3_shape.h"
#include "bullet3_physics.h"

namespace da::physics
{
	CBullet3Cube::CBullet3Cube(const glm::vec3& extents) : CBullet3Shape(), CPhysicsShapeCube(extents)
	{
		m_shape = new btBoxShape({ extents.x, extents.y, extents.z });
	}


	CBullet3Sphere::CBullet3Sphere(const float radius) : CBullet3Shape(), CPhysicsShapeSphere(radius)
	{
		m_shape = new btSphereShape(radius);
	}

	CBullet3Shape::~CBullet3Shape()
	{
		ASSERT(m_shape);
		delete m_shape;
	}

}