#pragma once

#include <DAEngine/core.h>

namespace da::core
{
	class CEntity;
}

class CCharacter
{
public:
	CCharacter();
	void initialize();
	void update(float dt);
	void lateUpdate(float dt);
	void shutdown();
	const da::core::CEntity* getEntity() const;

private:
	void processCamera(float dt);

private:
	da::core::CEntity* m_entity, * m_sword;
	glm::vec2 m_cursorPos = { 0.f, 0.f };
	float m_camHeight = 0.5f;
	float m_camRot = 4.7123f;
	float m_camDist = 2.5f;
	float m_scrollY = 0.f;
	float m_camSensitivity = .25f;

};