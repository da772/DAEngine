#pragma once

#include "../physics_rigid_body.h"

class btRigidBody;

namespace da::physics
{
	class CBullet3RigidBody : public IPhysicsRigidBody
	{
	public:
		CBullet3RigidBody(IPhysicsShape* shape, IPhysicsMotionState* state, float mass, const glm::vec3& inertia);
		virtual ~CBullet3RigidBody();
		glm::mat4 getTransform() const override;
		virtual void setTransform(const glm::mat4& transform) override;
		virtual void applyImpulse(const glm::vec3& impulse) override;
		void setActivationState(EPhysicsActivationState state) override;
		btRigidBody* getRigidBody() const;
		virtual void setRestitution(float restitution) override;
		virtual void setUserData(void* ptr) override;
		

		virtual void setAngularVelocity(const glm::vec3&) override;
		virtual void setAngularFactor(const glm::vec3& factor) override;
		virtual void setGravityFactor(const glm::vec3& factor) override;
		virtual void setLinearFactor(const glm::vec3& factor) override;

		virtual const glm::vec3& getLinearFactor() const override;
		virtual const glm::vec3& getGravityFactor() const override;
		virtual const glm::vec3& getAngularFactor() const override;
		virtual const glm::vec3& getAngularVelocity() const override;

	private:
		btRigidBody* m_rigidBody;
		glm::vec3 m_linearFactor, m_gravityFactor, m_angularFactor;
	};
}