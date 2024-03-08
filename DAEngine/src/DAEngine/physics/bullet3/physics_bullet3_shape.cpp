#include "dapch.h"

#include "physics_bullet3_shape.h"
#include "bullet3_physics.h"
#include <core/graphics/graphics_smesh.h>

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

	CBullet3TriangleMesh::CBullet3TriangleMesh(da::graphics::CStaticMesh* mesh) : CBullet3Shape(), CPhysicsShapeTriangleMesh(mesh)
	{
		m_mesh = mesh;
		const da::graphics::FMesh& m = mesh->getMeshes()[0];
		m_vertexArray = new btTriangleIndexVertexArray
		(
			m.Indices.size()/3,
			(int*)m.Indices.data(),
			sizeof(uint32_t),
			m.Vertices.size(),
			(btScalar*)m.Vertices.data(),
			sizeof(da::graphics::FVertexBase)
		);
		m_shape = new btBvhTriangleMeshShape(m_vertexArray, false);
	}

	CBullet3TriangleMesh::~CBullet3TriangleMesh()
	{
		delete m_vertexArray;
	}

	da::graphics::CStaticMesh* CBullet3TriangleMesh::getMesh() const
	{
		return m_mesh;
	}

	CBullet3Capsule::CBullet3Capsule(float radius, float height) : CBullet3Shape(), CPhysicsShapeCapsule(radius, height)
	{
		m_shape = new btCapsuleShapeZ(radius, height);
	}

	CBullet3Capsule::~CBullet3Capsule()
	{

	}

	da::graphics::CStaticMesh* CBullet3ConvexHullShape::getMesh() const
	{
		return m_mesh;
	}

	CBullet3ConvexHullShape::CBullet3ConvexHullShape(da::graphics::CStaticMesh* mesh) : CPhysicsShapeConvexHull(mesh)
	{
		m_mesh = mesh;
		const da::graphics::FMesh& m = mesh->getMeshes()[0];

		btConvexHullShape* convexHull = new btConvexHullShape(0, 0, sizeof(btVector3));

		btVector3* points = new btVector3[m.Indices.size()];

		for (size_t i = 0; i < m.Indices.size(); i++) {
			const auto& pos = m.Vertices[m.Indices[i]].Pos;
			convexHull->addPoint({ pos.x, pos.y, pos.z });
		}

		m_shape = convexHull;
	}

	CBullet3ConvexHullShape::~CBullet3ConvexHullShape()
	{
	}

}