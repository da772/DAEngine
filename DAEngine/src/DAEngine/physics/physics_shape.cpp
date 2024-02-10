#include "dapch.h"
#include "physics_shape.h"

#include "bullet3/physics_bullet3_shape.h"

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

	da::physics::CPhysicsShapeTriangleMesh* CPhysicsShapeTriangleMesh::create(da::graphics::CStaticMesh* mesh)
	{
		return new da::physics::CBullet3TriangleMesh(mesh);
	}

}