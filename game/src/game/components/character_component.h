#pragma once
#include <DAEngine/components.h>
#include <DAEngine/graphics.h>

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
	CCharacterComponent(const da::core::CGuid& guid, da::core::CEntity& parent);
	~CCharacterComponent();

	float getCamRot() const;
	void setWeaponEntity(const da::core::CEntity* weapon);

protected:
	void onInitialize();
	void onUpdate(float dt);
	void onShutdown();

private:
	void processInput(float dt);
	void processAnims(float dt);
	double wrapAngle(double angle) const;

#ifdef DA_REVIEW
	void onDebugRender();
#endif

private:
	std::vector<da::graphics::FSkeletalAnimGraphNode> m_anims;
	da::graphics::CSkeletalAnimGraph* m_animGraph;
	bool m_attack = false;
	glm::vec2 m_cursorPos = { 0.f, 0.f };
	float m_camRot = 4.7123f;
	float m_camSensitivity = .25f;
	const da::core::CEntity* m_weapon = nullptr;


};