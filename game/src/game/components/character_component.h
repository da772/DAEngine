#pragma once
#include <DAEngine/components.h>
#include <DAEngine/graphics.h>

class ICharacterMovement;

enum class ECharacterAnims : uint8_t
{
	Idle,
	Jog,
	Sprint,
	Button,
	JogLeft,
	JogRight,
	JogBack,
	Combo1,
	Combo2
};

class CCharacterComponent
{
#ifdef DA_REVIEW
	COMPONENT_H_DEBUG(CCharacterComponent);
#else 
	COMPONENT_H(CCharacterComponent);
#endif

public:
	CCharacterComponent(bool isLocalPlayer, const da::CGuid& guid, da::CEntity& parent);
	~CCharacterComponent();

	float getCamRot() const;
	void setWeaponEntity(const da::CEntity* weapon);
	inline float getCamSensitivity() const { return m_camSensitivity; }
	void attack();
	bool isAttacking() const;

protected:
	void onInitialize();
	void onUpdate(float dt);
	void onShutdown();

private:
	void processAnims(float dt);
	da::FSkeletalAnimGraphNode& getAnim(ECharacterAnims anim);

#ifdef DA_REVIEW
	void onDebugRender();
#endif

private:
	std::vector<da::FSkeletalAnimGraphNode> m_anims;
	da::CSkeletalAnimGraph* m_animGraph;
	glm::vec2 m_cursorPos = { 0.f, 0.f };
	float m_camSensitivity = .1f;
	ICharacterMovement* m_movement = nullptr;
	const da::CEntity* m_weapon = nullptr;
	uint8_t m_attack = 0;
	bool m_isLocalPlayer : 1;


};