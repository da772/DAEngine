

#include <DAEngine/core.h>
#include <DAEngine/graphics.h>

#include "character_movement_1.h"
#include "game/components/character_movement_component.h"
#include "game/components/character_component.h"

CCharacterMovement1::CCharacterMovement1(const da::core::CEntity& entity) : ICharacterMovement(entity)
{

}

void CCharacterMovement1::initialize()
{

}

void CCharacterMovement1::update(float dt)
{
	processInput(dt);
}

void CCharacterMovement1::shutdown()
{

}

float CCharacterMovement1::getMoveDir() const
{
	return m_camRot;
}

void CCharacterMovement1::processInput(float dt)
{
	da::core::FComponentRef<CCharacterMovementComponent> movement = m_character->getComponent<CCharacterMovementComponent>();
	da::core::FComponentRef<CCharacterComponent> character = m_character->getComponent<CCharacterComponent>();
	da::core::CCamera* cam = da::core::CCamera::getCamera();

	const float xPos = da::core::CInput::getCursorX();

	if (da::core::CInput::mouseInputPressed(1))
	{
		float xDiff = xPos - m_cursorPos.x;
		m_camRot -= glm::radians(xDiff) * character->getCamSensitivity();
		m_camRot = da::core::maths::wrapAngle(m_camRot);
	}
	m_cursorPos = { xPos, 0.f };

	if (character->isAttacking()) return;

	glm::vec3 direction = glm::vec3(0);
	float rotate = 0.f;
	bool jump = false;
	bool sprint = false;

	// Space
	jump = da::core::CInput::inputPressed(32);
	// LShift
	sprint = da::core::CInput::inputPressed(340);

	bool S = da::core::CInput::inputPressed(83);
	bool W = da::core::CInput::inputPressed(87);

	if (S || W) {
		direction = (float)(-S + W) * glm::normalize(glm::vec3(cam->forward().x, cam->forward().y, 0.f));
	}

	bool A = da::core::CInput::inputPressed(65);
	bool D = da::core::CInput::inputPressed(68);
	if (A || D) {

		float dirAngle = -90.f;

		if (W) {
			dirAngle = -45.f;
		}
		else if (S) {
			dirAngle = -135.f;
		}

		float dir = cam->rotationEuler().z + ((float)(-A + D) * dirAngle);
		movement->setRotation(glm::radians(dir), false);
		direction += (float)(-A + D) * cam->right();
	}

	movement->setRotationSpeed(360.f);

	if (W || S) {

		if (!A && !D)
		{
			double targetDir = cam->rotationEuler().z;
			double dir = da::core::maths::wrapAngle(glm::radians(targetDir)) - da::core::maths::wrapAngle(glm::radians(m_character->getTransform().rotationEuler().z));
			if (S) dir *= -1.f;
			double wrappedDir = da::core::maths::wrapAngle(dir);
			if (std::abs(wrappedDir) > 0.001f) {
				if (std::abs(wrappedDir) <= .05f) {
					movement->setRotation(targetDir);
				}
				else {
					rotate += wrappedDir > 3.14f ? -1.f : 1.f;
				}
			}
		}
	}

	if (jump) {
		movement->jump();
	}

	if (da::core::CInput::mouseInputPressed(0)) {
		character->attack();
		sprint = false;
		direction = glm::vec3(0.f);
		rotate = 0.f;
	}

	if (glm::length2(direction) > 0.f) {
		direction = glm::normalize(direction);
	}

	movement->sprint(sprint);
	movement->setWalkDirection(direction);
	movement->rotate(rotate);
}
