#pragma once
#include <DAEngine/components.h>

class ICharacterMovement
{
public:
	inline ICharacterMovement(const da::core::CEntity& entity) : m_character((da::core::CEntity*)&entity) {};
	virtual void initialize() = 0;
	virtual void update(float dt) = 0;
	virtual void shutdown() = 0;
	virtual float getMoveDir() const = 0;

#ifdef DA_REVIEW
	inline virtual void debugUpdate() {};
#endif

protected:
	da::core::CEntity* m_character;
};