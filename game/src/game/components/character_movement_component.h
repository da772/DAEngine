#pragma once

#include <daengine/components.h>
#include <daengine/physics.h>

class CCharacterMovementComponent
{
#ifdef DA_REVIEW
	COMPONENT_H_DEBUG(CCharacterMovementComponent);
#else
	COMPONENT_H(CCharacterMovementComponent)
#endif

public:
	CCharacterMovementComponent(const da::core::CGuid& guid, da::core::CEntity& parent);

	void jump();
	bool isGrounded() const;
	bool isSprinting() const;
	void sprint(bool sprint);
	void setWalkDirection(const glm::vec3& dir);
	void rotate(float dir);
	void setRotation(float dir, bool instance = true);
	void setRotationSpeed(float speed);
	void setWalkSpeed(float speed);
	void applyImpulse(const glm::vec3& impulse);
	const glm::vec3& getWalkDirection() const;
	const glm::vec3 getVelocity() const;

protected:
	void onInitialize();
	void onUpdate(float dt);
	void onShutdown();

private:
	void processMovement(float dt);

#ifdef DA_REVIEW
	void onDebugRender();
#endif

private:
	da::physics::ICharacterController* m_character = nullptr;
	da::physics::CPhysicsShapeCapsule* m_shape = nullptr;
	glm::vec3 m_direction = glm::vec3(0.f);
	float m_rotateDir = 0.f;
	float m_setRotate = 0.f;
	float m_speed = 4.4f;
	float m_rotateSpeed = 128.f;
	da::physics::FCharacterTuning m_tuning = { .15f };
	bool m_sprint : 1 = false;
	bool m_jump : 1 = false;
	bool m_rotateInstant : 1 = false;
	
};