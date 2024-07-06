#pragma once

namespace da::core
{
	class CEntity;
}

namespace da::graphics
{
	class CSkeletalAnimation;
	class CSkeletalAnimGraph;
}

class CCharacter
{
public:
	void initialize();
	void update(float dt);
	void shutdown();

private:
	da::core::CEntity* m_entity;
	da::graphics::CSkeletalAnimation* m_anim1, *m_anim2, *m_anim3;
	da::graphics::CSkeletalAnimGraph* m_animGraph;

};