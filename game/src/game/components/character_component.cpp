// engine
#include <DAEngine/core.h>

// game
#include "character_component.h"
#include "character_movement_component.h"
#include "game/character/movement/ai_character_movement.h"
#include "game/character/movement/character_movement_1.h"

// TODO: Remove
#include <DAEngine\platform\graphics\bgfx\bgfx_skeletal_mesh.h>

#ifdef DA_REVIEW
COMPONENT_CPP_DEBUG(CCharacterComponent)
#else
COMPONENT_CPP(CCharacterComponent)
#endif

CCharacterComponent::CCharacterComponent(bool isLocalPlayer, const da::core::CGuid& guid, da::core::CEntity& parent) : m_isLocalPlayer(isLocalPlayer), m_guid(guid), m_parent(parent)
{
	da::platform::CBgfxSkeletalMesh* mesh = new da::platform::CBgfxSkeletalMesh("assets/skeletons/camilla/camilla_02.fbx", false);

	for (const da::graphics::FMaterialData& mat : mesh->getMaterials())
	{
		const_cast<da::graphics::FMaterialData&>(mat).metallicFactor = 0.f;
	}

	mesh->getMaterial(10).baseColorFactor = { 0.f,0.f,0.f,0.f };
	mesh->getMaterial(13).baseColorFactor = { 0.f,0.f,0.f,0.25f };

	// Skin
	//mesh->getMaterial(0).metallicFactor = .200f;
	//mesh->getMaterial(0).roughnessFactor = 0.45f;
	//// Eye
	//mesh->getMaterial(1).metallicFactor = .1f;
	//mesh->getMaterial(1).roughnessFactor = 0.0f;
	//// Clothes
	//mesh->getMaterial(2).metallicFactor = .2f;
	//mesh->getMaterial(2).roughnessFactor = 1.0f;
	//// Lashes
	//mesh->getMaterial(3).metallicFactor = .1f;
	//mesh->getMaterial(3).roughnessFactor = 0.5f;

	m_anims = {
			
		  { new da::graphics::CSkeletalAnimator(new da::graphics::CSkeletalAnimation("assets/skeletons/camilla/camilla_02.fbx", mesh)), 1.f }// idle
		, { new da::graphics::CSkeletalAnimator(new da::graphics::CSkeletalAnimation("assets/skeletons/ue_mannequin/Anim_DK2_Run_F_IP_01.fbx", mesh)), 0.f } // jog
		, { new da::graphics::CSkeletalAnimator(new da::graphics::CSkeletalAnimation("assets/skeletons/ue_mannequin/Anim_DK2_Run_F_IP_01.fbx", mesh)), 0.f } // sprint
		, { new da::graphics::CSkeletalAnimator(new da::graphics::CSkeletalAnimation("assets/skeletons/archer/button.fbx", mesh)), 0.f } // button
		, { new da::graphics::CSkeletalAnimator(new da::graphics::CSkeletalAnimation("assets/skeletons/archer/jogLeft.fbx", mesh)), 0.f }
		, { new da::graphics::CSkeletalAnimator(new da::graphics::CSkeletalAnimation("assets/skeletons/archer/jogRight.fbx", mesh)), 0.f }
		, { new da::graphics::CSkeletalAnimator(new da::graphics::CSkeletalAnimation("assets/skeletons/archer/jogBack.fbx", mesh)), 0.f }
		, { new da::graphics::CSkeletalAnimator(new da::graphics::CSkeletalAnimation("assets/skeletons/ue_mannequin/Anim_DK2_Run_F_IP_01.fbx", mesh), false), 0.f }
	};



	m_animGraph = new da::graphics::CSkeletalAnimGraph(mesh, m_anims);
	da::core::FComponentRef<da::core::CSkeletalMeshComponent> meshComponent = parent.addComponent<da::core::CSkeletalMeshComponent>(m_animGraph);

	glm::mat4 offset = glm::translate(glm::mat4(1.f), { 0.f,0.f, -1.f }) * glm::toMat4(glm::quat(glm::radians(glm::vec3(0.f, 0.f, 180.f))));
	meshComponent->setTransform(offset);
	if (m_isLocalPlayer) m_movement = new CCharacterMovement1(m_parent);
	else m_movement = new CAICharacterMovement(m_parent);
}

CCharacterComponent::~CCharacterComponent()
{
	for (const da::graphics::FSkeletalAnimGraphNode& node : m_anims) {
		delete node.Animator->getCurrentAnim();
		delete node.Animator;
	}

	m_anims = {};
	if (m_movement) {
		delete m_movement;
		m_movement = nullptr;
	}
}

void CCharacterComponent::onInitialize()
{
	ASSERT(m_parent.getComponent<CCharacterMovementComponent>().isValid());
}

void CCharacterComponent::onUpdate(float dt)
{
	if (m_movement) m_movement->update(dt);
	processAnims(dt);
}

void CCharacterComponent::processAnims(float dt)
{
	da::core::FComponentRef<CCharacterMovementComponent> movement = m_parent.getComponent<CCharacterMovementComponent>();
	const glm::vec3& direction = movement->getWalkDirection();
	const bool isSprinting = movement->isSprinting();

	float forward = glm::clamp(glm::length(direction), -1.f, 1.f);//glm::clamp(glm::dot(m_parent.getTransform().forward(), direction), -1.f, 1.f);

	// attack
	if (m_attack) {
		if (m_anims[7].Animator->getPlayTime() >= m_anims[7].Animator->getMaxPlayTime())
		{
			m_attack = false;
		}
		else {
			m_animGraph->getNodes()[7].Weight = std::min(m_animGraph->getNodes()[7].Weight + 2.f * dt, (float)m_attack);
		}
	}
	else {
		m_animGraph->getNodes()[7].Weight = std::max(m_animGraph->getNodes()[7].Weight - 2.f * dt, 0.f);
	}

	// jog
	if (forward != 0.00f && !isSprinting) {
		m_animGraph->getNodes()[1].Weight = std::min(m_animGraph->getNodes()[1].Weight + 2.f * dt, std::abs(forward));
	}
	else {
		m_animGraph->getNodes()[1].Weight = std::max(m_animGraph->getNodes()[1].Weight - 2.f * dt, 0.f);
	}

	// sprint
	if (forward != 0.00f && isSprinting) {
		m_animGraph->getNodes()[2].Weight = std::min(m_animGraph->getNodes()[2].Weight + 2.f * dt, std::abs(forward));
	}
	else {
		m_animGraph->getNodes()[2].Weight = std::max(m_animGraph->getNodes()[2].Weight - 2.f * dt, 0.f);
	}

	// jog back
	if (forward < 0.f) {
		//m_animGraph->getNodes()[6].Weight = std::min(m_animGraph->getNodes()[6].Weight + 2.f * dt, std::abs(forward));
	}
	else {
		//m_animGraph->getNodes()[6].Weight = std::max(m_animGraph->getNodes()[6].Weight - 2.f * dt, 0.f);
	}

	// right
	float right = glm::dot(m_parent.getTransform().right(), direction);
	if (right < 0.f) {
		//m_animGraph->getNodes()[4].Weight = std::min(m_animGraph->getNodes()[4].Weight + 2.f * dt, std::abs(right));
	}
	else {
		//m_animGraph->getNodes()[4].Weight = std::max(m_animGraph->getNodes()[4].Weight - 2.f * dt, 0.f);
	}

	// left
	if (right > 0.f) {
		//m_animGraph->getNodes()[5].Weight = std::min(m_animGraph->getNodes()[5].Weight + 2.f * dt, std::abs(right));
	}
	else {
		//m_animGraph->getNodes()[5].Weight = std::max(m_animGraph->getNodes()[5].Weight - 2.f * dt, 0.f);
	}

	// idle
	if (forward || right || m_attack) {
		m_animGraph->getNodes()[0].Weight = std::max(m_animGraph->getNodes()[0].Weight - 2.f * dt, 0.f);
	}
	else {
		m_animGraph->getNodes()[0].Weight = std::min(m_animGraph->getNodes()[0].Weight + 2.f * dt, 1.f);
	}


	if (m_animGraph->getNodes()[(uint8_t)ECharacterAnims::Swing2].Weight > 0.f) {
		float pTime = m_animGraph->getNodes()[(uint8_t)ECharacterAnims::Swing2].Animator->getPlayTime();
		if (m_weapon) {
			da::core::FComponentRef<da::core::CCollisionComponent> colComp = m_weapon->getComponent<da::core::CCollisionComponent>();
			colComp->enable(pTime >= 20.f && pTime <= 30.f);
#ifdef DA_REVIEW
			if (pTime >= 20.f && pTime <= 30.f) colComp->debugRender();
#endif
		}
	}

}
void CCharacterComponent::onShutdown()
{

}

float CCharacterComponent::getCamRot() const
{
	if (!m_movement) return 0.f;
	return m_movement->getMoveDir();
}

void CCharacterComponent::setWeaponEntity(const da::core::CEntity* weapon)
{
	m_weapon = weapon;

	da::core::FComponentRef<da::core::CCollisionComponent> colComp = weapon->getComponent<da::core::CCollisionComponent>();
	colComp->enable(false);
}

void CCharacterComponent::attack()
{
	m_attack = true;
	m_anims[7].Animator->setPlayTime(0.f);
}

bool CCharacterComponent::isAttacking() const
{
	return m_attack;
}

#ifdef DA_REVIEW
void CCharacterComponent::onDebugRender()
{
	if (ImGui::Begin("Anim Test")) {
		da::core::FComponentRef<da::core::CSkeletalMeshComponent> skMesh = m_parent.getComponent<da::core::CSkeletalMeshComponent>();
		skMesh->getSkeletalAnimator()->debugRenderJoints(skMesh->getTransform());
		std::vector<da::graphics::FSkeletalAnimGraphNode>& nodes = m_animGraph->getNodes();
		for (int i = 0; i < nodes.size(); i++) {
			ImGui::LabelText("##label", "%.2f : %s: ", nodes[i].Animator->getPlayTime(), nodes[i].Animator->getCurrentAnim()->getAnimName().c_str());
			ImGui::InputFloat(std::to_string(i).c_str(), &nodes[i].Weight, .025, 0.f);
		}

		if (m_movement)
		{
			m_movement->debugUpdate();
		}
		
	}
	ImGui::End();
}
#endif