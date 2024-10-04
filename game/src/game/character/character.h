#pragma once

#include <DAEngine/core.h>
#include <DAEngine/graphics.h>

namespace da
{
	class CEntity;
}

class CCharacter;

class CCharacterGui : public da::CGui {
public:
	inline CCharacterGui(const da::CGraphicsApi& graphics, CCharacter* parent) : da::CGui(graphics), m_parent(parent) {}

protected:
	void onRender(float dt);

private:
	CCharacter* m_parent;

};

class CCharacter
{
public:
	CCharacter(const da::CGraphicsApi& graphics, bool isLocalPlayer);
	void initialize();
	void update(float dt);
	void lateUpdate(float dt);
	void shutdown();
	const da::CEntity* getEntity() const;

	friend class CCharacterGui;
private:
	void processCamera(float dt);
#ifdef DA_REVIEW
	void debugUpdate();
#endif

private:
	da::CCamera* m_camera = nullptr;
	da::CEntity* m_entity, * m_sword;
	CCharacterGui m_gui;
	glm::vec2 m_cursorPos = { 0.f, 0.f };
	float m_camHeight = 0.5f;
	float m_camRot = 4.7123f;
	float m_camDist = 2.5f;
	float m_scrollY = 0.f;
	float m_camSensitivity = .25f;
	bool m_isLocalPlayer = false;

};