// engine
#include <DAEngine/core.h>
#include <DAEngine/graphics.h>

// game
#include "character_component.h"
#include "character_movement_component.h"
#include "game/character/movement/ai_character_movement.h"
#include "game/character/movement/character_movement_1.h"


#ifdef DA_REVIEW
COMPONENT_CPP_DEBUG(CCharacterComponent)
#else
COMPONENT_CPP(CCharacterComponent)
#endif

CCharacterComponent::CCharacterComponent(bool isLocalPlayer, const da::CGuid& guid, da::CEntity& parent) : m_isLocalPlayer(isLocalPlayer), m_guid(guid), m_parent(parent)
{
	da::CSkeletalMesh* mesh;
	if (isLocalPlayer)
	{
		mesh = da::CSkeletalMeshFactory::create("assets/skeletons/camilla/camilla_02.fbx", false);
		mesh->getMaterial(10).baseColorFactor = { 0.f,0.f,0.f,0.f };
		mesh->getMaterial(13).baseColorFactor = { 0.f,0.f,0.f,0.25f };
	} 
	else
	{
		mesh = da::CSkeletalMeshFactory::create("assets/skeletons/pesant_f/pesant_f_02.fbx", false);
		mesh->getMaterial(11).baseColorFactor = { 0.f,0.f,0.f,0.f };
	}

	for (const da::FMaterialData& mat : mesh->getMaterials())
	{
		const_cast<da::FMaterialData&>(mat).metallicFactor = 0.f;
		const_cast<da::FMaterialData&>(mat).roughnessFactor = 0.45f;
	}
	

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
			
		  { new da::CSkeletalAnimator(new da::CSkeletalAnimation("assets/skeletons/ue_mannequin/Anim_DK2_Idle_RM_01.fbx", mesh)), 1.f }// idle
		, { new da::CSkeletalAnimator(new da::CSkeletalAnimation("assets/skeletons/ue_mannequin/Anim_DK2_Run_F_IP_01.fbx", mesh)), 0.f } // jog
		, { new da::CSkeletalAnimator(new da::CSkeletalAnimation("assets/skeletons/ue_mannequin/Anim_DK2_Run_F_IP_01.fbx", mesh)), 0.f } // sprint
		, { new da::CSkeletalAnimator(new da::CSkeletalAnimation("assets/skeletons/archer/button.fbx", mesh)), 0.f } // button
		, { new da::CSkeletalAnimator(new da::CSkeletalAnimation("assets/skeletons/archer/jogLeft.fbx", mesh)), 0.f } // jog left
		, { new da::CSkeletalAnimator(new da::CSkeletalAnimation("assets/skeletons/archer/jogRight.fbx", mesh)), 0.f } // jog right
		, { new da::CSkeletalAnimator(new da::CSkeletalAnimation("assets/skeletons/archer/jogBack.fbx", mesh)), 0.f } // jog back
		, { new da::CSkeletalAnimator(new da::CSkeletalAnimation("assets/skeletons/ue_mannequin/Anim_DK2_Combo_A1_IP_01.fbx", mesh), false), 0.f } // attack 1
		, { new da::CSkeletalAnimator(new da::CSkeletalAnimation("assets/skeletons/ue_mannequin/Anim_DK2_Combo_A2_IP_01.fbx", mesh), false), 0.f } // attack 2
	};



	m_animGraph = new da::CSkeletalAnimGraph(mesh, m_anims);
	da::FComponentRef<da::CSkeletalMeshComponent> meshComponent = parent.addComponent<da::CSkeletalMeshComponent>(m_animGraph);

	glm::mat4 offset = glm::translate(glm::mat4(1.f), { 0.f,0.f, -1.00f }) * glm::toMat4(glm::quat(glm::radians(glm::vec3(0.f, 0.f, 180.f))));
	meshComponent->setTransform(offset);
	if (m_isLocalPlayer) m_movement = new CCharacterMovement1(m_parent);
	else m_movement = new CAICharacterMovement(m_parent);
}

CCharacterComponent::~CCharacterComponent()
{
	for (const da::FSkeletalAnimGraphNode& node : m_anims) {
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
	da::FComponentRef<CCharacterMovementComponent> movement = m_parent.getComponent<CCharacterMovementComponent>();
	const glm::vec3& direction = movement->getWalkDirection();
	const bool isSprinting = movement->isSprinting();

	float forward = glm::clamp(glm::length(direction), -1.f, 1.f);//glm::clamp(glm::dot(m_parent.getTransform().forward(), direction), -1.f, 1.f);

	// attack
	if (m_attack) {

		if (m_attack == 2 && getAnim(ECharacterAnims::Combo1).Weight > 0.f && getAnim(ECharacterAnims::Combo1).Animator->getPlayTime() >= 20.f)
		{
			getAnim(ECharacterAnims::Combo2).Weight = 1.f;
			getAnim(ECharacterAnims::Combo1).Weight = 0.f;
			getAnim(ECharacterAnims::Combo2).Animator->setPlayTime(0.f);
		} 

		if (m_attack != 2 && getAnim(ECharacterAnims::Combo1).Animator->getPlayTime() >= 34.f/*m_anims[7].Animator->getMaxPlayTime()*/)
		{
			m_attack = 0;

			for (size_t i = 0; i < m_anims.size(); i++)
			{
				if (i == (size_t)ECharacterAnims::Combo1) continue;
				getAnim((ECharacterAnims)i).Animator->setPlayTime(0.f);
			}
		}
		else if (m_attack != 2) {
			getAnim(ECharacterAnims::Combo1).Weight = std::min(getAnim(ECharacterAnims::Combo1).Weight + 2.f * dt, std::min(1.f, (float)m_attack));
		}

		if (m_attack == 2 && getAnim(ECharacterAnims::Combo2).Animator->getPlayTime() >= 25.f)
		{
			m_attack = 0;
		}
	}
	else {
		getAnim(ECharacterAnims::Combo1).Weight = std::max(getAnim(ECharacterAnims::Combo1).Weight - 2.f * dt, 0.f);
		getAnim(ECharacterAnims::Combo2).Weight = std::max(getAnim(ECharacterAnims::Combo2).Weight - 2.f * dt, 0.f);
	}

	// jog
	if (forward != 0.00f && !isSprinting) {
		getAnim(ECharacterAnims::Jog).Weight = std::min(getAnim(ECharacterAnims::Jog).Weight + 2.f * dt, std::abs(forward));
	}
	else {
		getAnim(ECharacterAnims::Jog).Weight = std::max(getAnim(ECharacterAnims::Jog).Weight - 2.f * dt, 0.f);
	}

	// sprint
	if (forward != 0.00f && isSprinting) {
		getAnim(ECharacterAnims::Sprint).Weight = std::min(getAnim(ECharacterAnims::Sprint).Weight + 2.f * dt, std::abs(forward));
	}
	else {
		getAnim(ECharacterAnims::Sprint).Weight = std::max(getAnim(ECharacterAnims::Sprint).Weight - 2.f * dt, 0.f);
	}

	// jog back
	if (forward < 0.f) {
		//getAnim(6].Weight = std::min(getAnim(6].Weight + 2.f * dt, std::abs(forward));
	}
	else {
		//getAnim(6].Weight = std::max(getAnim(6].Weight - 2.f * dt, 0.f);
	}

	// right
	float right = glm::dot(m_parent.getTransform().right(), direction);
	if (right < 0.f) {
		//getAnim(4].Weight = std::min(getAnim(4].Weight + 2.f * dt, std::abs(right));
	}
	else {
		//getAnim(4].Weight = std::max(getAnim(4].Weight - 2.f * dt, 0.f);
	}

	// left
	if (right > 0.f) {
		//getAnim(5].Weight = std::min(getAnim(5].Weight + 2.f * dt, std::abs(right));
	}
	else {
		//getAnim(5].Weight = std::max(getAnim(5].Weight - 2.f * dt, 0.f);
	}

	// idle
	if (forward || right || m_attack) {
		getAnim(ECharacterAnims::Idle).Weight = std::max(getAnim(ECharacterAnims::Idle).Weight - 2.f * dt, 0.f);
	}
	else {
		getAnim(ECharacterAnims::Idle).Weight = std::min(getAnim(ECharacterAnims::Idle).Weight + 2.f * dt, 1.f);
	}


	if (getAnim(ECharacterAnims::Combo1).Weight > 0.f) {
		float pTime = getAnim(ECharacterAnims::Combo1).Animator->getPlayTime();
		if (m_weapon) {
			da::FComponentRef<da::CCollisionComponent> colComp = m_weapon->getComponent<da::CCollisionComponent>();
			colComp->enable(pTime >= 14.f && pTime <= 20.f);

			if (pTime >= 3.f && pTime < 4.f)
			{
				movement->applyImpulse(m_parent.getTransform().forward() * 6.f);
			}

#ifdef DA_REVIEW
			if (pTime >= 14.f && pTime <= 20.f) colComp->debugRender();
#endif
		}
	}

	if (getAnim(ECharacterAnims::Combo2).Weight > 0.f) {
		float pTime = getAnim(ECharacterAnims::Combo2).Animator->getPlayTime();
		if (m_weapon) {
			da::FComponentRef<da::CCollisionComponent> colComp = m_weapon->getComponent<da::CCollisionComponent>();
			colComp->enable(pTime >= 2.f && pTime <= 8.f);

			if (pTime >= 5.f && pTime < 6.f)
			{
				//movement->applyImpulse(m_parent.getTransform().forward() * 3.f);
			}

#ifdef DA_REVIEW
			if (pTime >= 2.f && pTime <= 8.f) colComp->debugRender();
#endif
		}
	}

}

da::FSkeletalAnimGraphNode& CCharacterComponent::getAnim(ECharacterAnims anim)
{
	return m_animGraph->getNodes()[(uint8_t)anim];
}

void CCharacterComponent::onShutdown()
{

}

float CCharacterComponent::getCamRot() const
{
	if (!m_movement) return 0.f;
	return m_movement->getMoveDir();
}

void CCharacterComponent::setWeaponEntity(const da::CEntity* weapon)
{
	m_weapon = weapon;

	da::FComponentRef<da::CCollisionComponent> colComp = weapon->getComponent<da::CCollisionComponent>();
	colComp->enable(false);
}

void CCharacterComponent::attack()
{
	switch (m_attack)
	{
		case 0:
		{
			m_attack = 1;
			getAnim(ECharacterAnims::Combo1).Weight = 0.f;
			getAnim(ECharacterAnims::Combo1).Animator->setPlayTime(0.f);
			getAnim(ECharacterAnims::Combo2).Animator->setPlayTime(0.f);
			getAnim(ECharacterAnims::Combo2).Weight = 0.f;
			break;
		}
		case 1:
		{
			if (getAnim(ECharacterAnims::Combo1).Animator->getPlayTime() >= 13.f && getAnim(ECharacterAnims::Combo1).Animator->getPlayTime() < 21.f)
			{
				m_attack = 2;
				getAnim(ECharacterAnims::Combo2).Animator->setPlayTime(0.f);
			}
			break;
		}
	}
}

bool CCharacterComponent::isAttacking() const
{
	return m_attack;
}

#ifdef DA_REVIEW
void CCharacterComponent::onDebugRender()
{
	if (!m_isLocalPlayer)
	{
		CAICharacterMovement* ai = (CAICharacterMovement*)m_movement;
		ImGui::LabelText("###aiToggle", "AI Status: %s", ai->isPaused() ? "Paused" : "Playing");
		if (ImGui::Button("Toggle Status"))
		{
			ai->setPause(!ai->isPaused());
		}
	}
	

	if (ImGui::Begin("Anim Test")) {
		da::FComponentRef<da::CSkeletalMeshComponent> skMesh = m_parent.getComponent<da::CSkeletalMeshComponent>();
		//skMesh->getSkeletalAnimator()->debugRenderJoints(skMesh->getTransform());
		std::vector<da::FSkeletalAnimGraphNode>& nodes = m_animGraph->getNodes();

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