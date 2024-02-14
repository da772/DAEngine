#pragma once


namespace da::graphics
{
	class CStaticMesh;
}


namespace da::physics {

	enum class EPhysicsShapeType
	{
		Cube,
		Sphere,
		TriangleMesh,
		Capsule
	};

	class IPhysicsShape
	{
	public:
		virtual EPhysicsShapeType getType() const = 0;

	protected:
		inline IPhysicsShape() {};
		inline virtual ~IPhysicsShape() {};
	};

	class CPhysicsShapeCube : public IPhysicsShape
	{
	public:
		inline virtual EPhysicsShapeType getType() const override { return EPhysicsShapeType::Cube; }
		static CPhysicsShapeCube* create(const glm::vec3& extents);
	protected:
		inline CPhysicsShapeCube(const glm::vec3& extents) {};
		inline virtual ~CPhysicsShapeCube() {};

	};

	class CPhysicsShapeSphere : public IPhysicsShape
	{
	public:
		inline virtual EPhysicsShapeType getType() const override { return EPhysicsShapeType::Sphere; }

		static CPhysicsShapeSphere* create(const float radius);
	protected:
		inline CPhysicsShapeSphere(const float radius) {};
		inline virtual ~CPhysicsShapeSphere() {};
	};

	class CPhysicsShapeTriangleMesh : public IPhysicsShape
	{
	public:
		inline virtual EPhysicsShapeType getType() const override { return EPhysicsShapeType::TriangleMesh; }
		inline virtual da::graphics::CStaticMesh* getMesh() const { return nullptr; }

		static CPhysicsShapeTriangleMesh* create(da::graphics::CStaticMesh* mesh);
	protected:
		inline CPhysicsShapeTriangleMesh(da::graphics::CStaticMesh* mesh) {};
		inline virtual ~CPhysicsShapeTriangleMesh() {};
	};

	class CPhysicsShapeCapsule : public IPhysicsShape
	{
	public:
		inline virtual EPhysicsShapeType getType() const override { return EPhysicsShapeType::Capsule; }
		inline virtual da::graphics::CStaticMesh* getMesh() const { return nullptr; }

		static CPhysicsShapeCapsule* create(float radius, float height);
	protected:
		inline CPhysicsShapeCapsule(float radius, float height) {};
		inline virtual ~CPhysicsShapeCapsule() {};
	};
}
