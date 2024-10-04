#pragma once

#include "character_movement.h"

class CAICharacterMovement : public ICharacterMovement
{

public:
	CAICharacterMovement(const da::CEntity& entity);
	void initialize() override;
	void update(float dt) override;
	void shutdown() override;
	float getMoveDir() const override;
	inline bool isPaused() const { return m_pause; }
	void setPause(bool pause);

#ifdef DA_REVIEW
	void debugUpdate() override;
#endif

private:
	void processAi();

private:
	std::vector<glm::vec3> m_path;
	bool m_pause : 1 = true;
};