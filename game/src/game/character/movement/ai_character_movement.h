#pragma once

#include "character_movement.h"

class CAICharacterMovement : public ICharacterMovement
{

public:
	CAICharacterMovement(const da::core::CEntity& entity);
	void initialize() override;
	void update(float dt) override;
	void shutdown() override;
	float getMoveDir() const override;
#ifdef DA_REVIEW
	void debugUpdate() override;
#endif

private:
	void processAi();

private:
	std::vector<glm::vec3> m_path;
};