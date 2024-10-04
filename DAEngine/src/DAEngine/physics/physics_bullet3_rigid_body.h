#pragma once

#include "physics/physics_rigid_body.h"

class btRigidBody;

namespace da
{
	class CBullet3RigidBody : public IPhysicsRigidBody
	{
	public:
		CBullet3RigidBody(IPhysicsShape* shape, IPhysicsMotionState* state, float mass, const glm::vec3& inertia);
		virtual ~CBullet3RigidBody();
		glm::mat4 getTransform() const override;
		virtual void setTransform(const glm::mat4& transform) override;
		virtual void applyImpulse(const glm::vec3& impulse) override;
		virtual void applyImpulse(const glm::vec3& impulse, const glm::vec3& location) override;
		void setActivationState(EPhysicsActivationState state) override;
		btRigidBody* getRigidBody() const;
		virtual void setRestitution(float restitution) override;
		virtual void setUserData(void* ptr) override;
		virtual void reset();
		

		virtual void setLinearVelocity(const glm::vec3& impulse) override;
		virtual void setAngularVelocity(const glm::vec3&) override;
		virtual void setAngularFactor(const glm::vec3& factor) override;
		virtual void setGravityFactor(const glm::vec3& factor) override;
		virtual void setLinearFactor(const glm::vec3& factor) override;

		virtual const glm::vec3& getLinearFactor() const override;
		virtual const glm::vec3& getGravityFactor() const override;
		virtual const glm::vec3& getAngularFactor() const override;
		virtual const glm::vec3 getAngularVelocity() const override;
		virtual const glm::vec3 getLinearVelocity() const override;

#if defined(DA_DEBUG) || defined(DA_RELEASE)
		virtual void debugDraw() override;
#endif

	private:
		btRigidBody* m_rigidBody;
		glm::vec3 m_linearFactor, m_gravityFactor, m_angularFactor;
	};
}