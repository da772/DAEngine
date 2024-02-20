#pragma once

#include "physics_shape.h"
#include "physics_motion_state.h"

namespace da::physics
{
	enum class EPhysicsActivationState : uint8_t
	{
		ActiveTag = 1,
		IslandSleeping = 2,
		WantsDeactivation = 3,
		DisableDeactivation  = 4,
		DisableSimulation = 5,
		FixedBaseMultiBody = 6
	};

	class IPhysicsRigidBody
	{
	public:
		IPhysicsRigidBody(IPhysicsShape* shape, IPhysicsMotionState* state, float mass, const glm::vec3& inertia);

		virtual glm::mat4 getTransform() const = 0;
		virtual void setTransform(const glm::mat4& transform) = 0;
		virtual void applyImpulse(const glm::vec3& impulse) = 0;
		virtual void setRestitution(float restitution) = 0;
		virtual void setActivationState(EPhysicsActivationState state) = 0;
		virtual void setUserData(void* ptr) = 0;

		virtual void setAngularVelocity(const glm::vec3&) = 0;
		virtual void setAngularFactor(const glm::vec3& factor) = 0;
		virtual void setGravityFactor(const glm::vec3& factor) = 0;
		virtual void setLinearFactor(const glm::vec3& factor) = 0;

		virtual const glm::vec3& getLinearFactor() const = 0;
		virtual const glm::vec3& getGravityFactor() const = 0;
		virtual const glm::vec3& getAngularFactor() const = 0;
		virtual const glm::vec3& getAngularVelocity() const = 0;

#if defined(DA_DEBUG) || defined(DA_RELEASE)
		virtual void debugDraw() = 0;
#endif

		IPhysicsShape* getShape() const;

		static IPhysicsRigidBody* create(IPhysicsShape* shape, IPhysicsMotionState* state, float mass, const glm::vec3& inertia);

	protected:
		IPhysicsShape* m_shape;

	};

}