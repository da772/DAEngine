#pragma once

#include "../physics_shape.h"
#include "bullet3_physics.h"

namespace da::physics
{
	class CBullet3Shape :public IPhysicsShape
	{
	public:
		inline btCollisionShape* getShape() const { return m_shape;};
		virtual ~CBullet3Shape();
	protected:
		inline CBullet3Shape() : m_shape(nullptr) {};
		btCollisionShape* m_shape;
	};

	class CBullet3Cube : public CBullet3Shape, public CPhysicsShapeCube
	{
	public:
		CBullet3Cube(const glm::vec3& extents);
		inline virtual EPhysicsShapeType getType() const override { return EPhysicsShapeType::Cube; }

	};

	class CBullet3Sphere : public  CBullet3Shape, public  CPhysicsShapeSphere
	{
	public:
		CBullet3Sphere(const float radius);
		inline virtual EPhysicsShapeType getType() const override { return EPhysicsShapeType::Sphere; }

	};

	class CBullet3TriangleMesh : public CBullet3Shape, public  CPhysicsShapeTriangleMesh
	{
	public:
		CBullet3TriangleMesh(da::graphics::CStaticMesh* mesh);
		virtual ~CBullet3TriangleMesh();
		inline virtual EPhysicsShapeType getType() const override { return EPhysicsShapeType::TriangleMesh; }
		virtual da::graphics::CStaticMesh* getMesh() const override;
	private:
		btTriangleIndexVertexArray* m_vertexArray;
		da::graphics::CStaticMesh* m_mesh;

	};

	class CBullet3Capsule : public CBullet3Shape, public CPhysicsShapeCapsule
	{
	public:
		CBullet3Capsule(float radius, float height);
		virtual ~CBullet3Capsule();
		inline virtual EPhysicsShapeType getType() const override { return EPhysicsShapeType::Capsule; }

	};
}