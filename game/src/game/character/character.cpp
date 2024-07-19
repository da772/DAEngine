#include "character.h"
#include "../components/character_movement_component.h"
#include "../components/character_component.h"

#include <DAEngine/components.h>

void CCharacter::initialize()
{
	if (da::core::CScene* scene = da::core::CSceneManager::getScene()) {
		m_entity = scene->createEntity();
		m_entity->setTag(HASHSTR("Character"));
		m_entity->addComponent<CCharacterMovementComponent>(da::core::CGuid::Generate());
		m_entity->addComponent<CCharacterComponent>(da::core::CGuid::Generate());

		m_sword = scene->createEntity();
		m_sword->setTag(HASHSTR("Sword"));
		m_sword->addComponent<da::core::CSmeshComponent>("assets/prop/weapon/prop_weap_sword.fbx");
	}
}

void CCharacter::update(float dt)
{
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
