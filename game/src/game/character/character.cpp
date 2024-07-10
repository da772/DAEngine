#include "character.h"
#include <DAEngine/core.h>
#include <DAEngine/components.h>
#include <DAEngine/physics.h>
#include <DAEngine/graphics.h>

#include <DAEngine\platform\graphics\bgfx\bgfx_skeletal_mesh.h>
#include <DAEngine\debug.h>

void CCharacter::initialize()
{
	if (da::core::CScene* scene = da::core::CSceneManager::getScene()) {
		m_entity = scene->createEntity();

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

		m_anim1 = new da::graphics::CSkeletalAnimation("assets/skeletons/archer/idle.fbx", mesh);
		m_anim2 = new da::graphics::CSkeletalAnimation("assets/skeletons/archer/jog.fbx", mesh);
		m_anim3 = new da::graphics::CSkeletalAnimation("assets/skeletons/archer/sprint.fbx", mesh);
		m_anim4 = new da::graphics::CSkeletalAnimation("assets/skeletons/archer/button.fbx", mesh);

		da::graphics::CSkeletalAnimator* animator1 = new da::graphics::CSkeletalAnimator(m_anim1);
		da::graphics::CSkeletalAnimator* animator2 = new da::graphics::CSkeletalAnimator(m_anim2);
		da::graphics::CSkeletalAnimator* animator3 = new da::graphics::CSkeletalAnimator(m_anim3);
		da::graphics::CSkeletalAnimator* animator4 = new da::graphics::CSkeletalAnimator(m_anim4);

		//animator2->SetBoneSelector({ 
		//	//, {HASHSTR("mixamorig:Hips"), false}
		//	//, {HASHSTR("mixamorig:LeftUpLeg"), true}
		//	//, {HASHSTR("mixamorig:RightUpLeg"), true }
		//	});
		
		animator4->SetBoneSelector(
			{ 
				 {HASHSTR("mixamorig:Hips"), false}
				, { HASHSTR("mixamorig:Spine"), true }
			});

		//animator3->SetBoneSelector(HASHSTR("mixamorig_Spine2"));

		m_animGraph = new da::graphics::CSkeletalAnimGraph(mesh, { {animator1, 1.f}, {animator2, .0f}, {animator3, .0f},  {animator4, .0f} });
		
		da::core::FComponentRef<da::core::CSkeletalMeshComponent> cc = m_entity->addComponent<da::core::CSkeletalMeshComponent>(m_animGraph);
		m_entity->addComponent<da::core::CScriptComponent>("scripts/build/camera_component.lua", "CameraComponent");

		//cc->getSkeletalMesh()->getMaterial(0).baseColorFactor = { 0.0f,0.0f,0.8f,1.f };

		m_entity->getTransform().setPosition({ 0,5, 5.f });
		m_entity->setTag(HASHSTR("Character"));

		glm::mat4 offset = glm::translate(glm::mat4(1.f), { 0.f,0.f, -1.15f }) * glm::toMat4(glm::quat(glm::radians(glm::vec3(0.f, 0.f, 180.f))));

		cc->setTransform(offset);

		da::core::FComponentRef<da::core::CRigidBodyComponent> rb = m_entity->addComponent<da::core::CRigidBodyComponent>(
			da::physics::IPhysicsRigidBody::create(da::physics::CPhysicsShapeCapsule::create(.3f, 1.7f)
				, da::physics::CPhysicsEntityMotionState::create(m_entity)
				, 500.f
				, { 0.f,0.f,0.f }));


		rb->getPhysicsBody()->setAngularFactor({ 0.f,0.f,0.f });
	}
}

void CCharacter::update(float dt)
{
	da::core::FComponentRef<da::core::CSkeletalMeshComponent> component = m_entity->getComponent<da::core::CSkeletalMeshComponent>();

#if defined(DA_DEBUG) || defined(DA_RELEASE)
	if (ImGui::Begin("Anim Test")) {
		da::core::FComponentRef<da::core::CSkeletalMeshComponent> skMesh = m_entity->getComponent<da::core::CSkeletalMeshComponent>();
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

	//component->getSkeletalAnimator()->debugRenderJoints(component->getTransform());
#endif
}

void CCharacter::shutdown()
{
	da::core::CSceneManager::getScene()->removeEntity(m_entity);
}
