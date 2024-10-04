#pragma once

#include <daengine/components.h>

class CHealthComponent
{
	COMPONENT_H_NO_UPDATE(CHealthComponent);
public:
	CHealthComponent(const da::CGuid& id, da::CEntity& parent);
	void damage(float amt);
	void setHealth(float amt);
	bool isDead() const;
	float getHealth() const;

protected:
	void onInitialize();
	void onShutdown();


private:
	float m_health = 100.f;
	float m_maxHealth;
};