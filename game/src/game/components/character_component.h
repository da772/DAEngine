#pragma once
#include <DAEngine/components.h>
#include <DAEngine/graphics.h>


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

protected:
	void onInitialize();
	void onUpdate(float dt);
	void onShutdown();

private:
	void processInput(float dt);
	void processCamera(float dt);
	void processAnims(float dt);
	double wrapAngle(double angle) const;

#ifdef DA_REVIEW
	void onDebugRender();
#endif

private:
	glm::vec2 m_cursorPos = { 0.f, 0.f };
	float m_camHeight = 0.5f;
	float m_camRot = 4.7123f;
	float m_camDist = 2.5f;
	float m_scrollY = 0.f;
	float m_camSensitivity = .25f;
	std::vector<da::graphics::FSkeletalAnimGraphNode> m_anims;
	da::graphics::CSkeletalAnimGraph* m_animGraph;
	bool m_attack = false;


};