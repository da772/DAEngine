#pragma once

#include "../physics_character.h"
#include <bullet3/btBulletDynamicsCommon.h>
#include <bullet3/BulletDynamics/Character/btKinematicCharacterController.h>
#include <bullet3/BulletCollision/CollisionDispatch/btGhostObject.h>

namespace da::physics
{
	class CBullet3Character : public ICharacterController
	{
	public:
		CBullet3Character(const FCharacterTuning& data, IPhysicsShape& rigidBody);
		~CBullet3Character();

		void setWalkDirection(const glm::vec3& dir) override;
		const glm::vec3& getWalkDirection() const override;
		void jump(const glm::vec3& jump) override;
		bool grounded() const override;
		const glm::vec3& getLinearVelocity() const override;
		const glm::vec3& getAngularVelocity() const override;
		void warp(const glm::vec3& pos) override;
		const glm::vec3 getPosition() const override;
		void setLinearVelocity(const glm::vec3& dir) override;

#ifdef DA_REVIEW
		virtual void debugDraw() override;
#endif

	private:
		btPairCachingGhostObject m_ghost;
		btKinematicCharacterController m_characterController;

	};
	
}