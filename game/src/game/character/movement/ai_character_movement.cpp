#include <daengine/core.h>
#include <daengine/ai.h>
#include <daengine/debug.h>


#include "ai_character_movement.h"
#include "game/components/character_movement_component.h"
#include "game/components/character_component.h"


CAICharacterMovement::CAICharacterMovement(const da::CEntity& entity) : ICharacterMovement(entity)
{

}

void CAICharacterMovement::initialize()
{

}

void CAICharacterMovement::update(float dt)
{
	if (m_pause) return;
	processAi();
}

float CAICharacterMovement::getMoveDir() const
{
	return 0.f;
}

void CAICharacterMovement::setPause(bool pause)
{
	m_pause = pause;
}

void CAICharacterMovement::shutdown()
{

}

void CAICharacterMovement::processAi()
{

	da::FComponentRef<CCharacterComponent> character = m_character->getComponent<CCharacterComponent>();

	if (character->isAttacking())
	{
		return;
	}

	const da::CEntity* target = da::CSceneManager::getScene()->getEntityFromTag(HASHSTR("Character"));
	const glm::vec3 targetPos = target->getTransform().position();
	const glm::vec3 currentPos = m_character->getTransform().position();

	if (glm::distance(currentPos, targetPos) < 1.5f)
	{
		if (!m_path.empty())
		{
			character->attack();
		}
		m_path = {};
		
		return;
	}

	if (m_path.empty())
	{
		if (da::INavMesh* navMesh = da::CNavMeshManager::getNavMesh())
		{
			m_path = navMesh->findPath(currentPos, targetPos);
			if (!m_path.empty()) m_path.erase(m_path.begin());
			return;
		}
	}

	const glm::vec3 pathPos = m_path[0];


	if (glm::distance(currentPos, pathPos) < 1.5f)
	{
		m_path.erase(m_path.begin());
		return;
	}

	da::FComponentRef<CCharacterMovementComponent> movement = m_character->getComponent<CCharacterMovementComponent>();
	glm::vec3 dir = glm::vec3(pathPos.x, pathPos.y, 0.f) - glm::vec3(currentPos.x, currentPos.y, 0.f);
	if (glm::length(dir) == 0.0f) {
		m_path = {};
		return;
	}
	dir = glm::normalize(dir);
	movement->setWalkDirection(dir);

	glm::vec3 rot = da::calculateEulerRotationForDirection(dir);
	movement->setRotationSpeed(360.f);
	movement->setRotation(rot.z, false);
}


#ifdef DA_REVIEW
void CAICharacterMovement::debugUpdate()
{
	if (m_path.empty())
	{
		return;
	}


	for (uint32_t i = 0; i < m_path.size(); i++) {
		if (i != m_path.size() - 1) {
			da::CDebugRender::drawLine(m_path[i], m_path[i + 1], .1f, {1.f, 1.f, 1.f, 1.f});
		}
		
		da::CDebugRender::drawCube(m_path[i], glm::quat(1.f, 0.f, 0.f, 0.f), { .05f,.05f,.05f }, { 1.f,1.f,0.f,1.f }, false);
	}
	
}
#endif

