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
	const glm::vec3& getWalkDirection() const;

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
	bool m_sprint = false;
	bool m_jump = false;
	glm::vec3 m_direction = glm::vec3(0.f);
	float m_speed = 4.f;
	da::physics::FCharacterTuning m_tuning = { .5f };
};