
#include "physics/physics_shape.h"

#include "physics_bullet3_shape.h"
#include "graphics/graphics_smesh.h"

namespace da
{

	da::CPhysicsShapeCube* CPhysicsShapeCube::create(const glm::vec3& extents)
	{
		return new da::CBullet3Cube(extents);
	}

	da::CPhysicsShapeSphere* CPhysicsShapeSphere::create(const float radius)
	{
		return new da::CBullet3Sphere(radius);
	}

	da::CPhysicsShapeTriangleMesh* CPhysicsShapeTriangleMesh::create(da::CStaticMesh* mesh, uint32_t index)
	{
		return new da::CBullet3TriangleMesh(mesh, index);
	}

	da::CPhysicsShapeCapsule* CPhysicsShapeCapsule::create(float radius, float height)
	{
		return new da::CBullet3Capsule(radius, height);
	}

	da::CPhysicsShapeConvexHull* CPhysicsShapeConvexHull::create(da::CStaticMesh* mesh, uint32_t index)
	{
		return new da::CBullet3ConvexHullShape(mesh, index);
	}

	da::CPhysicsShapeCompound* CPhysicsShapeCompound::create(const std::vector<IPhysicsShape*>& shapes, const std::vector<glm::mat4>& transforms)
	{
		return new da::CBullet3CompoundShape(shapes, transforms);
	}

	da::IPhysicsShape* IPhysicsShape::createTriangleMesh(da::CStaticMesh* mesh)
	{
		std::vector<IPhysicsShape*> shapes;
		std::vector<glm::mat4> transforms;
		for (size_t i = 0; i < mesh->getMeshes().size(); i++) {
			shapes.push_back(CPhysicsShapeTriangleMesh::create(mesh, i));
		}

		return CPhysicsShapeCompound::create(shapes, transforms);
	}

	da::IPhysicsShape* IPhysicsShape::createMeshConvexHull(da::CStaticMesh* mesh)
	{
		std::vector<IPhysicsShape*> shapes;
		std::vector<glm::mat4> transforms;
		for (size_t i = 0; i < mesh->getMeshes().size(); i++) {
			shapes.push_back(CPhysicsShapeConvexHull::create(mesh, i));
		}

		return CPhysicsShapeCompound::create(shapes, transforms);
	}

}