

#include <DAEngine/core.h>
#include <DAEngine/graphics.h>

#include "character_movement_1.h"
#include "game/components/character_movement_component.h"
#include "game/components/character_component.h"

CCharacterMovement1::CCharacterMovement1(const da::CEntity& entity) : ICharacterMovement(entity)
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
	da::CInputContext inputCtx(HASHSTR("CharacterInput"), 0);
	
	if (!da::CInput::inputContextAvailable())
	{
		return;
	}

	da::FComponentRef<CCharacterMovementComponent> movement = m_character->getComponent<CCharacterMovementComponent>();
	da::FComponentRef<CCharacterComponent> character = m_character->getComponent<CCharacterComponent>();
	da::CCamera* cam = da::CCamera::getCamera();

	const float xPos = da::CInput::getCursorX();

	if (true/*da::CInput::mouseInputPressed(1)*/)
	{
		float xDiff = xPos - m_cursorPos.x;
		m_camRot -= glm::radians(xDiff) * character->getCamSensitivity();
		m_camRot = da::wrapAngle(m_camRot);
	}
	m_cursorPos = { xPos, 0.f };

	if (character->isAttacking()) {
		if (da::CInput::mouseInputPressed(0)) {
			character->attack();
		}
		return;
	}

	glm::vec3 direction = glm::vec3(0);
	float rotate = 0.f;
	bool jump = false;
	bool sprint = false;

	// Space
	jump = da::CInput::inputPressed(32);
	// LShift
	sprint = da::CInput::inputPressed(340);

	bool S = da::CInput::inputPressed(83);
	bool W = da::CInput::inputPressed(87);

	if (S || W) {
		direction = (float)(-S + W) * glm::normalize(glm::vec3(cam->forward().x, cam->forward().y, 0.f));
	}

	bool A = da::CInput::inputPressed(65);
	bool D = da::CInput::inputPressed(68);

	if (A && D)
	{
		A = false;
		D = false;
	}

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

			if (S)
			{
				targetDir += 180.f;
			}
			double dir = da::wrapAngle(glm::radians(targetDir)) - da::wrapAngle(glm::radians(m_character->getTransform().rotationEuler().z));
			double wrappedDir = da::wrapAngle(dir);
			
			if (std::abs(wrappedDir) > 0.001f) {
				if (std::abs(wrappedDir) <= .1f) {
					movement->setRotation(targetDir, true);
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

	if (da::CInput::mouseInputPressed(0)) {
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
