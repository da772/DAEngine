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
	}
}

void CCharacter::update(float dt)
{
	
}

void CCharacter::shutdown()
{
	da::core::CSceneManager::getScene()->removeEntity(m_entity);
}
