#pragma once

namespace da::physics {

	enum class EPhysicsShapeType
	{
		Cube,
		Sphere
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
}
