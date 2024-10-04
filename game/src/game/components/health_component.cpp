#include "health_component.h"

COMPONENT_CPP_NO_UPDATE(CHealthComponent);

CHealthComponent::CHealthComponent(const da::CGuid& id, da::CEntity& parent) : m_guid(id), m_parent(parent)
{

}

void CHealthComponent::onInitialize()
{

}

void CHealthComponent::onShutdown()
{

}

void CHealthComponent::damage(float amt)
{
	m_health -= amt;
}

void CHealthComponent::setHealth(float amt)
{
	m_health = amt;
}

bool CHealthComponent::isDead() const
{
	return m_health <= 0.f;
}

float CHealthComponent::getHealth() const
{
	return m_health;
}
