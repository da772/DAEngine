#pragma once

#include "physics/physics_shape.h"
#include "bullet3_physics.h"

namespace da
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
		CBullet3TriangleMesh(da::CStaticMesh* mesh, uint32_t index);
		virtual ~CBullet3TriangleMesh();
		inline virtual EPhysicsShapeType getType() const override { return EPhysicsShapeType::TriangleMesh; }
		virtual da::CStaticMesh* getMesh() const override;
	private:
		btTriangleIndexVertexArray* m_vertexArray;
		da::CStaticMesh* m_mesh;

	};

	class CBullet3Capsule : public CBullet3Shape, public CPhysicsShapeCapsule
	{
	public:
		CBullet3Capsule(float radius, float height);
		virtual ~CBullet3Capsule();
		inline virtual EPhysicsShapeType getType() const override { return EPhysicsShapeType::Capsule; }

	};

	class CBullet3ConvexHullShape : public CBullet3Shape, public  CPhysicsShapeConvexHull
	{
	public:
		CBullet3ConvexHullShape(da::CStaticMesh* mesh, uint32_t index);
		virtual ~CBullet3ConvexHullShape();
		inline virtual EPhysicsShapeType getType() const override { return EPhysicsShapeType::ConvexHull; }
		virtual da::CStaticMesh* getMesh() const override;
	private:
		da::CStaticMesh* m_mesh;

	};

	class CBullet3CompoundShape : public CBullet3Shape, public CPhysicsShapeCompound
	{
	public:
		CBullet3CompoundShape(const std::vector<IPhysicsShape*>& shapes, const std::vector<glm::mat4>& transforms);
		inline virtual EPhysicsShapeType getType() const override { return EPhysicsShapeType::Compound; }

		virtual ~CBullet3CompoundShape();

	private:
		std::vector<IPhysicsShape*> m_shapes;
	};
}