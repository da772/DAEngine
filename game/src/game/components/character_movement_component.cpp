#include "character_movement_component.h"

#ifdef DA_REVIEW
COMPONENT_CPP_DEBUG(CCharacterMovementComponent)
#else
COMPONENT_CPP(CCharacterMovementComponent)
#endif

CCharacterMovementComponent::CCharacterMovementComponent(const da::core::CGuid& guid, da::core::CEntity& parent) : m_guid(guid), m_parent(parent)
{
	
}

void CCharacterMovementComponent::onInitialize()
{
	m_shape = da::physics::CPhysicsShapeCapsule::create(.3f, 1.7f);
	m_character = da::physics::CharacterControllerFactory::create(m_tuning, *m_shape);
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

	if (m_character->grounded() && m_jump) {
		m_character->jump({ 0.f,0.f,6.5f });
	}

	m_character->setWalkDirection(m_direction * walkSpeed);
	m_direction = glm::vec3(0.f);
	m_jump = false;
	m_sprint = false;
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


const glm::vec3& CCharacterMovementComponent::getWalkDirection() const
{
	return m_direction;
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

	m_character->debugDraw();
}
#endif
