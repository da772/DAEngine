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

}