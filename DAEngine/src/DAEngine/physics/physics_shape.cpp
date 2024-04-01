#include "dapch.h"
#include "physics_shape.h"

#include "bullet3/physics_bullet3_shape.h"
#include "core/graphics/graphics_smesh.h"

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

	da::physics::CPhysicsShapeTriangleMesh* CPhysicsShapeTriangleMesh::create(da::graphics::CStaticMesh* mesh, uint32_t index)
	{
		return new da::physics::CBullet3TriangleMesh(mesh, index);
	}

	da::physics::CPhysicsShapeCapsule* CPhysicsShapeCapsule::create(float radius, float height)
	{
		return new da::physics::CBullet3Capsule(radius, height);
	}

	da::physics::CPhysicsShapeConvexHull* CPhysicsShapeConvexHull::create(da::graphics::CStaticMesh* mesh, uint32_t index)
	{
		return new da::physics::CBullet3ConvexHullShape(mesh, index);
	}

	da::physics::CPhysicsShapeCompound* CPhysicsShapeCompound::create(const std::vector<IPhysicsShape*>& shapes, const std::vector<glm::mat4>& transforms)
	{
		return new da::physics::CBullet3CompoundShape(shapes, transforms);
	}

	da::physics::IPhysicsShape* IPhysicsShape::createTriangleMesh(da::graphics::CStaticMesh* mesh)
	{
		std::vector<IPhysicsShape*> shapes;
		std::vector<glm::mat4> transforms;
		for (size_t i = 0; i < mesh->getMeshes().size(); i++) {
			shapes.push_back(CPhysicsShapeTriangleMesh::create(mesh, i));
		}

		return CPhysicsShapeCompound::create(shapes, transforms);
	}

	da::physics::IPhysicsShape* IPhysicsShape::createMeshConvexHull(da::graphics::CStaticMesh* mesh)
	{
		std::vector<IPhysicsShape*> shapes;
		std::vector<glm::mat4> transforms;
		for (size_t i = 0; i < mesh->getMeshes().size(); i++) {
			shapes.push_back(CPhysicsShapeConvexHull::create(mesh, i));
		}

		return CPhysicsShapeCompound::create(shapes, transforms);
	}

}