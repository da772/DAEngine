#pragma once

#include "character_movement.h"

class CCharacterMovement1 : public ICharacterMovement
{
public:
	CCharacterMovement1(const da::core::CEntity& entity);
	virtual void initialize() override;
	virtual void update(float dt) override;
	virtual void shutdown() override;
	virtual float getMoveDir() const override;

private:
	void processInput(float dt);
private:
	glm::vec2 m_cursorPos = { 0.f, 0.f };
	float m_camRot = 4.7123f;
};