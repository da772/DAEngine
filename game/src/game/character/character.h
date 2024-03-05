#pragma once

namespace da::core
{
	class CEntity;
}


class CCharacter
{
public:
	void initialize();
	void update(float dt);
	void shutdown();

private:
	da::core::CEntity* m_entity;

};