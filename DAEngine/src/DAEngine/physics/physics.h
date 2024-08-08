#pragma once

namespace da::core
{
	class CEntity;
}

namespace da::physics
{
	enum class ERayType
	{
		All,
		Single
	};

	struct FHitData
	{
		da::core::CEntity* pEntity;
		glm::vec3 normal;
		glm::vec3 position;
	};

	struct FRayData
	{
		inline FRayData(ERayType type, const glm::vec3& startPos, const glm::vec3& endPos) : eType(type), startPos(startPos), endPos(endPos), bHit(false) {}

		ERayType eType;
		bool bHit;
		std::vector<FHitData> vHits;
		glm::vec3 startPos, endPos;
	};

	class CPhysicsType
	{
	public:
		virtual void initialize() = 0;
		virtual void update(float dt) = 0;
		virtual void shutdown() = 0;

		virtual void reset() = 0;
		virtual void rayCast(FRayData& ray) = 0;

	};

	class CPhysics
	{
	public:
		static void initialize();
		static void update(float dt);
		static void shutdown();

		static void rayCast(FRayData& ray);
		static CPhysicsType* getPhysicsType();
		static double getFixedTime();

	private:
		static double s_fixedTime;
		static CPhysicsType* s_physicsType;
		static double s_lastUpdate;
	};
}