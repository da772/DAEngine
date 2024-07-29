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
	Swing2
};

class CCharacterComponent
{
#ifdef DA_REVIEW
	COMPONENT_H_DEBUG(CCharacterComponent);
#else 
	COMPONENT_H(CCharacterComponent);
#endif

public:
	CCharacterComponent(bool isLocalPlayer, const da::core::CGuid& guid, da::core::CEntity& parent);
	~CCharacterComponent();

	float getCamRot() const;
	void setWeaponEntity(const da::core::CEntity* weapon);
	inline float getCamSensitivity() const { return m_camSensitivity; }
	void attack();
	bool isAttacking() const;

protected:
	void onInitialize();
	void onUpdate(float dt);
	void onShutdown();

private:
	void processAnims(float dt);

#ifdef DA_REVIEW
	void onDebugRender();
#endif

private:
	std::vector<da::graphics::FSkeletalAnimGraphNode> m_anims;
	da::graphics::CSkeletalAnimGraph* m_animGraph;
	glm::vec2 m_cursorPos = { 0.f, 0.f };
	float m_camSensitivity = .25f;
	ICharacterMovement* m_movement = nullptr;
	const da::core::CEntity* m_weapon = nullptr;
	bool m_attack : 1 = false;
	bool m_isLocalPlayer : 1;


};