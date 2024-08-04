#include "character_movement_component.h"

#ifdef DA_REVIEW
#include <DAEngine/core/graphics/graphics_debug_render.h>
#include <DAEngine/graphics.h>
COMPONENT_CPP_DEBUG(CCharacterMovementComponent)
#else
COMPONENT_CPP(CCharacterMovementComponent)
#endif

CCharacterMovementComponent::CCharacterMovementComponent(const da::core::CGuid& guid, da::core::CEntity& parent) : m_guid(guid), m_parent(parent)
{
	
}

void CCharacterMovementComponent::onInitialize()
{
	m_shape = da::physics::CPhysicsShapeCapsule::create(.3f, 1.5f);
	m_character = da::physics::CharacterControllerFactory::create(m_tuning, *m_shape);
	m_character->setUserData((void*)&m_parent);
}

void CCharacterMovementComponent::onUpdate(float dt)
{
	const glm::vec3 position = m_character->getPosition();
	m_parent.getTransform().setPosition(position);
	m_parent.getTransform().matrix();
	processMovement(dt);
}

void CCharacterMovementComponent::processMovement(float dt)
{
	float walkVelocity = m_speed;

	if (m_sprint) {
		walkVelocity *= 1.5f;
	}

	float walkSpeed = walkVelocity * da::physics::CPhysics::getFixedTime();
	float rotateSpeed = m_rotateSpeed * m_rotateDir * dt;

	if (m_character->grounded() && m_jump) {
		m_character->jump({ 0.f,0.f,6.5f });
	}

	if (m_setRotate != 0.f) {
		if (m_rotateInstant) {
			m_parent.getTransform().setRotation({ 0.f, 0.f, m_setRotate });
		}
		else {
			float z = glm::radians(m_parent.getTransform().rotationEuler().z);
			double angle = da::core::maths::wrapAngle(m_setRotate - z);
			if (std::abs(angle) <= .1f) {
				m_parent.getTransform().setRotation({ 0.f, 0.f, glm::degrees(m_setRotate) });
				m_rotateInstant = true;
			}
			else {
				m_parent.getTransform().offsetRotation({ 0.f, 0.f, m_rotateSpeed * (angle > 3.14159f ? -1.f : 1.f) * dt });
			}
		}
	}
	else {
		m_parent.getTransform().offsetRotation({ 0.f, 0.f, rotateSpeed });
	}

	m_parent.getTransform().matrix();
	m_character->setWalkDirection(m_direction * walkSpeed);

	m_direction = glm::vec3(0.f);
	m_jump = false;
	m_sprint = false;
	m_rotateDir = 0.f;
	if (m_rotateInstant) {
		m_setRotate = 0.f;
		m_rotateInstant = false;
	}
	
}


void CCharacterMovementComponent::jump()
{
	m_jump = true;
}

bool CCharacterMovementComponent::isGrounded() const
{
	return m_character->grounded();
}

bool CCharacterMovementComponent::isSprinting() const
{
	return m_sprint;
}

void CCharacterMovementComponent::sprint(bool sprint)
{
	m_sprint = sprint;
}

void CCharacterMovementComponent::setWalkDirection(const glm::vec3& dir)
{
	m_direction = dir;
}


void CCharacterMovementComponent::rotate(float dir)
{
	m_rotateDir = dir;
}

void CCharacterMovementComponent::setRotation(float dir, bool instant)
{
	m_setRotate = dir;
	m_rotateInstant = instant;
}

void CCharacterMovementComponent::setRotationSpeed(float speed)
{
	m_rotateSpeed = speed;
}

void CCharacterMovementComponent::setWalkSpeed(float speed)
{
	m_speed = speed;
}

void CCharacterMovementComponent::applyImpulse(const glm::vec3& impulse)
{
	m_character->applyImpulse(impulse);
}

const glm::vec3& CCharacterMovementComponent::getWalkDirection() const
{
	return m_direction;
}

const glm::vec3 CCharacterMovementComponent::getVelocity() const
{
	return m_character->getLinearVelocity();
}

void CCharacterMovementComponent::onShutdown()
{
	ASSERT(m_shape);
	ASSERT(m_character);
	delete m_shape;
	delete m_character;
	m_shape = nullptr;
	m_character = nullptr;
}

#ifdef DA_REVIEW
void CCharacterMovementComponent::onDebugRender()
{
	if (ImGui::Begin("walk direction")) {
		float walkSpeed = m_sprint * da::physics::CPhysics::getFixedTime();
		ImGui::LabelText("##for", "FORWARD: %f, %f, %f", m_parent.getTransform().forward().x, m_parent.getTransform().forward().y, m_parent.getTransform().forward().z);
		ImGui::LabelText("##right", "RIGHT: %f, %f, %f", m_parent.getTransform().right().x, m_parent.getTransform().right().y, m_parent.getTransform().right().z);
		ImGui::LabelText("##dir", "DIR: %f, %f, %f", m_direction.x, m_direction.y, m_direction.z);
		ImGui::LabelText("##dir2", "DIR2: %f, %f, %f", m_direction.x * walkSpeed, m_direction.y * walkSpeed, m_direction.z * walkSpeed);
		ImGui::LabelText("##speed", "Speed: %f", glm::length(m_character->getLinearVelocity()));
	}

	ImGui::End();

	glm::quat quat = da::core::CCamera::lookAt(m_direction, glm::vec3(0.f));
	da::graphics::CDebugRender::drawCone(m_parent.getTransform().position(), quat, {.001f, .005f, .001f}, { 1.f,0.f,1.f,1.f }, false);

	m_character->debugDraw();
}
#endif
