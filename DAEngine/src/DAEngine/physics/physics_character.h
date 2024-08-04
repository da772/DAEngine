#pragma once

namespace da::physics
{
	class IPhysicsShape;

	struct FCharacterTuning
	{
		float StepHeight = .01f;
	};

	class ICharacterController
	{
	public:
		inline ICharacterController(const FCharacterTuning& data, IPhysicsShape& shape) : m_tuning(data) {};
		inline virtual ~ICharacterController() {};

		virtual void warp(const glm::vec3& pos) = 0;
		virtual void setWalkDirection(const glm::vec3& dir) = 0;
		virtual void setLinearVelocity(const glm::vec3& dir) = 0;
		virtual const glm::vec3& getWalkDirection() const = 0;
		virtual void jump(const glm::vec3& jump) = 0;
		virtual bool grounded() const = 0;
		virtual void applyImpulse(const glm::vec3& impulse) = 0;
		virtual const glm::vec3& getLinearVelocity() const = 0;
		virtual const glm::vec3& getAngularVelocity() const = 0;
		virtual const glm::vec3 getPosition() const = 0;
		virtual void setUserData(void* ptr) = 0;

#ifdef DA_REVIEW
		virtual void debugDraw() = 0;
#endif

	protected:
		FCharacterTuning m_tuning;
	};

	class CharacterControllerFactory
	{
	public:
		static ICharacterController* create(const FCharacterTuning& data, IPhysicsShape& rigidBody);
	};
}