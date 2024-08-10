#pragma once
#include "daglm.h"
#include "dastd.h"

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
		Capsule,
		ConvexHull,
		Compound
	};

	class IPhysicsShape
	{
	public:
		virtual EPhysicsShapeType getType() const = 0;

		static IPhysicsShape* createTriangleMesh(da::graphics::CStaticMesh* mesh);
		static IPhysicsShape* createMeshConvexHull(da::graphics::CStaticMesh* mesh);

	public:
		inline virtual ~IPhysicsShape() {};

	protected:
		inline IPhysicsShape() {};
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

		static CPhysicsShapeTriangleMesh* create(da::graphics::CStaticMesh* mesh, uint32_t index = 0);
		inline virtual ~CPhysicsShapeTriangleMesh() {};
	protected:
		inline CPhysicsShapeTriangleMesh(da::graphics::CStaticMesh* mesh, uint32_t index) {};
	};

	class CPhysicsShapeConvexHull: public IPhysicsShape
	{
	public:
		inline virtual EPhysicsShapeType getType() const override { return EPhysicsShapeType::ConvexHull; }
		inline virtual da::graphics::CStaticMesh* getMesh() const { return nullptr; }

		static CPhysicsShapeConvexHull* create(da::graphics::CStaticMesh* mesh, uint32_t index = 0);
		inline virtual ~CPhysicsShapeConvexHull() {};
	protected:
		inline CPhysicsShapeConvexHull(da::graphics::CStaticMesh* mesh, uint32_t index) {};
	};

	class CPhysicsShapeCapsule : public IPhysicsShape
	{
	public:
		inline virtual EPhysicsShapeType getType() const override { return EPhysicsShapeType::Capsule; }
		inline virtual da::graphics::CStaticMesh* getMesh() const { return nullptr; }

		static CPhysicsShapeCapsule* create(float radius, float height);
		inline virtual ~CPhysicsShapeCapsule() {};
	protected:
		inline CPhysicsShapeCapsule(float radius, float height) {};
	};

	class CPhysicsShapeCompound : public IPhysicsShape
	{
	public:
		inline virtual EPhysicsShapeType getType() const override { return EPhysicsShapeType::Compound; }

		static CPhysicsShapeCompound* create(const std::vector<IPhysicsShape*>& shapes, const std::vector<glm::mat4>& transforms);
		inline virtual ~CPhysicsShapeCompound() {};
	protected:
		inline CPhysicsShapeCompound(const std::vector<IPhysicsShape*>& shapes, const std::vector<glm::mat4>& transforms) {};
		
	};
}
