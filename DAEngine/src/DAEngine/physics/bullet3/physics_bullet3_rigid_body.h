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
		void setUserData(void* ptr) override;
		virtual const glm::vec3& getAngularVelocity() const override;
		virtual void setAngularVelocity(const glm::vec3&) const override;

	private:
		btRigidBody* m_rigidBody;
	};
}