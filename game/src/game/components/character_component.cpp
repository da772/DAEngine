// engine
#include <DAEngine/core.h>

// game
#include "character_component.h"
#include "character_movement_component.h"

// TODO: Remove
#include <DAEngine\platform\graphics\bgfx\bgfx_skeletal_mesh.h>

#ifdef DA_REVIEW
COMPONENT_CPP_DEBUG(CCharacterComponent)
#else
COMPONENT_CPP(CCharacterComponent)
#endif

CCharacterComponent::CCharacterComponent(const da::core::CGuid& guid, da::core::CEntity& parent) : m_guid(guid), m_parent(parent)
{
	da::platform::CBgfxSkeletalMesh* mesh = new da::platform::CBgfxSkeletalMesh("assets/skeletons/archer/archer.fbx", false);
	// Skin
	mesh->getMaterial(0).metallicFactor = .200f;
	mesh->getMaterial(0).roughnessFactor = 0.45f;
	// Eye
	mesh->getMaterial(1).metallicFactor = .1f;
	mesh->getMaterial(1).roughnessFactor = 0.0f;
	// Clothes
	mesh->getMaterial(2).metallicFactor = .2f;
	mesh->getMaterial(2).roughnessFactor = 1.0f;
	// Lashes
	mesh->getMaterial(3).metallicFactor = .1f;
	mesh->getMaterial(3).roughnessFactor = 0.5f;

	m_anims = {
		  { new da::graphics::CSkeletalAnimator(new da::graphics::CSkeletalAnimation("assets/skeletons/archer/idle.fbx", mesh)), 1.f }
		, { new da::graphics::CSkeletalAnimator(new da::graphics::CSkeletalAnimation("assets/skeletons/archer/jog.fbx", mesh)), 0.f }
		, { new da::graphics::CSkeletalAnimator(new da::graphics::CSkeletalAnimation("assets/skeletons/archer/sprint.fbx", mesh)), 0.f }
		, { new da::graphics::CSkeletalAnimator(new da::graphics::CSkeletalAnimation("assets/skeletons/archer/button.fbx", mesh)), 0.f }
		, { new da::graphics::CSkeletalAnimator(new da::graphics::CSkeletalAnimation("assets/skeletons/archer/jogLeft.fbx", mesh)), 0.f }
		, { new da::graphics::CSkeletalAnimator(new da::graphics::CSkeletalAnimation("assets/skeletons/archer/jogRight.fbx", mesh)), 0.f }
		, { new da::graphics::CSkeletalAnimator(new da::graphics::CSkeletalAnimation("assets/skeletons/archer/jogBack.fbx", mesh)), 0.f }
		, { new da::graphics::CSkeletalAnimator(new da::graphics::CSkeletalAnimation("assets/skeletons/archer/swing2.fbx", mesh), false), 0.f }
	};

	m_animGraph = new da::graphics::CSkeletalAnimGraph(mesh, m_anims);
	da::core::FComponentRef<da::core::CSkeletalMeshComponent> meshComponent = parent.addComponent<da::core::CSkeletalMeshComponent>(m_animGraph);

	glm::mat4 offset = glm::translate(glm::mat4(1.f), { 0.f,0.f, -1.15f }) * glm::toMat4(glm::quat(glm::radians(glm::vec3(0.f, 0.f, 180.f))));
	meshComponent->setTransform(offset);
}

CCharacterComponent::~CCharacterComponent()
{
	for (const da::graphics::FSkeletalAnimGraphNode& node : m_anims) {
		delete node.Animator->getCurrentAnim();
		delete node.Animator;
	}

	m_anims = {};
}

void CCharacterComponent::onInitialize()
{
	ASSERT(m_parent.getComponent<CCharacterMovementComponent>().isValid());
}

void CCharacterComponent::onUpdate(float dt)
{
	processInput(dt);
	processAnims(dt);
}

void CCharacterComponent::processInput(float dt)
{
	da::core::FComponentRef<CCharacterMovementComponent> movement = m_parent.getComponent<CCharacterMovementComponent>();
	da::core::CCamera* cam = da::core::CCamera::getCamera();

	if (m_attack) return;

	glm::vec3 direction = glm::vec3(0);
	float rotate = 0.f;
	bool jump = false;
	bool sprint = false;

	// Space
	jump = da::core::CInput::inputPressed(32);
	// LShift
	sprint = da::core::CInput::inputPressed(340);

	bool S = da::core::CInput::inputPressed(83);
	bool W = da::core::CInput::inputPressed(87);

	const float xPos = da::core::CInput::getCursorX();
	if (da::core::CInput::mouseInputPressed(1))
	{
		float xDiff = xPos - m_cursorPos.x;
		m_camRot -= glm::radians(xDiff) * m_camSensitivity;
		m_camRot = wrapAngle(m_camRot);
	}
	m_cursorPos = { xPos, 0.f };

	if (S || W) {
		direction = (float)(-S + W)* glm::normalize(glm::vec3(cam->forward().x, cam->forward().y, 0.f));
	}

	bool A = da::core::CInput::inputPressed(65);
	bool D = da::core::CInput::inputPressed(68);
	if (A || D) {
		if (S || W) {
			m_camRot = wrapAngle(m_camRot + (((float)A + (float)-D) * dt * 2.f));
		}
		else {
			float dir = cam->rotationEuler().z + ((float)(-A + D)*-90.f);
			movement->setRotation(glm::radians(dir), false);
			direction += (float)(-A + D) * cam->right();
		}
	}

	movement->setRotationSpeed(164.f);

	if (W || S) {
		double dir = wrapAngle(glm::radians(cam->rotationEuler().z)) - wrapAngle(glm::radians(m_parent.getTransform().rotationEuler().z));
		double wrappedDir = wrapAngle(dir);
		if (std::abs(wrappedDir) > 0.001f) {
			if (std::abs(wrappedDir) <= .05f) {
				movement->setRotation(cam->rotationEuler().z);
			}
			else {
				rotate += wrappedDir > 3.14f ? -1.f : 1.f;
				movement->setRotationSpeed(225.f);
			}
		}
	}

	if (jump) {
		movement->jump();
	}

	if (da::core::CInput::mouseInputPressed(0)) {
		m_anims[7].Animator->setPlayTime(0);
		m_attack = true;
		sprint = false;
		direction = glm::vec3(0.f);
		rotate = 0.f;
	}

	movement->sprint(sprint);
	movement->setWalkDirection(direction);
	movement->rotate(rotate);
}

void CCharacterComponent::processAnims(float dt)
{
	da::core::FComponentRef<CCharacterMovementComponent> movement = m_parent.getComponent<CCharacterMovementComponent>();
	const glm::vec3& direction = movement->getWalkDirection();
	const bool isSprinting = movement->isSprinting();

	float forward = glm::dot(m_parent.getTransform().forward(), direction);

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

	if (forward > 0.f && !isSprinting) {
		m_animGraph->getNodes()[1].Weight = std::min(m_animGraph->getNodes()[1].Weight + 2.f * dt, std::abs(forward));
	}
	else {
		m_animGraph->getNodes()[1].Weight = std::max(m_animGraph->getNodes()[1].Weight - 2.f * dt, 0.f);
	}

	if (forward > 0.f && isSprinting) {
		m_animGraph->getNodes()[2].Weight = std::min(m_animGraph->getNodes()[2].Weight + 2.f * dt, std::abs(forward));
	}
	else {
		m_animGraph->getNodes()[2].Weight = std::max(m_animGraph->getNodes()[2].Weight - 2.f * dt, 0.f);
	}

	if (forward < 0.f) {
		m_animGraph->getNodes()[6].Weight = std::min(m_animGraph->getNodes()[6].Weight + 2.f * dt, std::abs(forward));
	}
	else {
		m_animGraph->getNodes()[6].Weight = std::max(m_animGraph->getNodes()[6].Weight - 2.f * dt, 0.f);
	}

	float right = glm::dot(m_parent.getTransform().right(), direction);

	if (right < 0.f) {
		m_animGraph->getNodes()[4].Weight = std::min(m_animGraph->getNodes()[4].Weight + 2.f * dt, std::abs(right));
	}
	else {
		m_animGraph->getNodes()[4].Weight = std::max(m_animGraph->getNodes()[4].Weight - 2.f * dt, 0.f);
	}

	if (right > 0.f) {
		m_animGraph->getNodes()[5].Weight = std::min(m_animGraph->getNodes()[5].Weight + 2.f * dt, std::abs(right));
	}
	else {
		m_animGraph->getNodes()[5].Weight = std::max(m_animGraph->getNodes()[5].Weight - 2.f * dt, 0.f);
	}

	if (forward || right || m_attack) {
		m_animGraph->getNodes()[0].Weight = std::max(m_animGraph->getNodes()[0].Weight - 2.f * dt, 0.f);
	}
	else {
		m_animGraph->getNodes()[0].Weight = std::min(m_animGraph->getNodes()[0].Weight + 2.f * dt, 1.f);
	}
}

double CCharacterComponent::wrapAngle(double angle) const
{
	double twoPi = 2.0 * 3.141592865358979;
	return angle - twoPi * floor(angle / twoPi);
}

void CCharacterComponent::onShutdown()
{

}

#ifdef DA_REVIEW
void CCharacterComponent::onDebugRender()
{
	if (ImGui::Begin("Anim Test")) {
		da::core::FComponentRef<da::core::CSkeletalMeshComponent> skMesh = m_parent.getComponent<da::core::CSkeletalMeshComponent>();
		skMesh->getSkeletalAnimator()->debugRenderJoints(skMesh->getTransform());
		std::vector<da::graphics::FSkeletalAnimGraphNode>& nodes = m_animGraph->getNodes();
		for (int i = 0; i < nodes.size(); i++) {
			ImGui::LabelText("##label", "%s: ", nodes[i].Animator->getCurrentAnim()->getAnimName().c_str());
			ImGui::InputFloat(std::to_string(i).c_str(), &nodes[i].Weight, .025, 0.f);

			if (i == 3) {
				nodes[i].Weight = std::clamp(nodes[i].Weight, 0.f, .9f);


				if (nodes[i].Weight >= .90f) {
					nodes[i].Animator->setTimeScale(2.0);
				}
				else {
					nodes[i].Animator->setTimeScale(0.0);
				}
			}


		}
	}
	ImGui::End();
}
#endif