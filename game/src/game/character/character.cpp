#include "character.h"
#include "../components/character_movement_component.h"
#include "../components/character_component.h"

#include <DAEngine/components.h>

CCharacter::CCharacter()
{

}

void CCharacter::initialize()
{
	if (da::core::CScene* scene = da::core::CSceneManager::getScene()) {
		m_entity = scene->createEntity();
		m_entity->setTag(HASHSTR("Character"));
		m_entity->addComponent<CCharacterMovementComponent>(da::core::CGuid::Generate());
		da::core::FComponentRef<CCharacterComponent> characterRef = m_entity->addComponent<CCharacterComponent>(da::core::CGuid::Generate());

		m_sword = scene->createEntity();
		m_sword->setTag(HASHSTR("Sword"));
		m_sword->addComponent<da::core::CSmeshComponent>("assets/prop/weapon/prop_weap_sword.fbx");
		da::physics::IPhysicsShape* shape = da::physics::CPhysicsShapeCube::create({ .017f, .057f, .598f });
		m_sword->addComponent<da::core::CCollisionComponent>(*shape, glm::translate(glm::mat4(1.f), { 0.f, 0.f, 1.f }), da::core::CGuid::Generate());

		characterRef->setWeaponEntity(m_sword);
	}
}

void CCharacter::update(float dt)
{
	
}

void CCharacter::lateUpdate(float dt)
{
	processCamera(dt);
	da::core::FComponentRef<da::core::CSkeletalMeshComponent> skele = m_entity->getComponent<da::core::CSkeletalMeshComponent>();
	glm::mat4 transform;
	glm::vec3 position;
	glm::quat rot;

	if (!skele->getSkeletalAnimator()->getCurrentAnim()) return;

	if (!skele->getSkeletalAnimator()->getBoneWorldPosition(HASHSTR("mixamorig:RightHand"), skele->getTransform(), position)) {
		return;
	}

	if (!skele->getSkeletalAnimator()->getBoneWorldRotation(HASHSTR("mixamorig:RightHand"), skele->getTransform(), rot)) {
		return;
	}

	m_sword->getTransform().setPosition(position + rot * glm::vec3(-0.1f, 0.1f, 0.02f));
	m_sword->getTransform().setRotation(rot * glm::quat(glm::radians(glm::vec3(0.f, 90.f, 0.f))));
}

void CCharacter::shutdown()
{
	da::core::CSceneManager::getScene()->removeEntity(m_entity);
	da::core::CSceneManager::getScene()->removeEntity(m_sword);
}

const da::core::CEntity* CCharacter::getEntity() const
{
	return m_entity;
}

void CCharacter::processCamera(float dt)
{
	da::core::CCamera* cam = da::core::CCamera::getCamera();
	const float xPos = da::core::CInput::getCursorX();
	const float yPos = da::core::CInput::getCursorY();

	da::core::FComponentRef<CCharacterComponent> character = m_entity->getComponent<CCharacterComponent>();

	if (da::core::CInput::mouseInputPressed(1))
	{
		float yDiff = yPos - m_cursorPos.y;
		m_camHeight += glm::radians(yDiff) * m_camSensitivity;
		m_camHeight = std::clamp(m_camHeight, -0.5f, 3.f);
	}

	m_camRot = character->getCamRot();

	glm::vec3 xOff = (m_camDist * std::cos(m_camRot)) * glm::vec3(1.f, 0.f, 0.f);
	glm::vec3 yOff = (m_camDist * std::sin(m_camRot)) * glm::vec3(0.f, 1.f, 0.f);
	double yScroll = da::core::CInput::getScrollY();

	if (m_scrollY != yScroll) {
		m_camDist -= 5.f * dt * yScroll;
		m_camDist = std::clamp(m_camDist, 1.f, 5.f);
		m_scrollY = yScroll;
	}

	m_cursorPos = { xPos, yPos };

	glm::vec3 pos = m_entity->getTransform().position() + glm::vec3(0.f, 0.f, m_camHeight) + xOff + yOff;
	cam->setPosition(pos);
	cam->lookAt(m_entity->getTransform().position());
}
