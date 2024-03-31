#include "character.h"
#include <DAEngine.h>
#include <DAEngine/physics/physics_shape.h>
#include <DAEngine/physics/physics_motion_state.h>
#include <DAEngine/core/graphics/factory/factory_graphics_texture2d.h>
#include <daengine/core/graphics/graphics_skmesh.h>

#include <DAEngine\platform\graphics\bgfx\bgfx_skeletal_mesh.h>
#include <DAEngine\core\graphics\skeletal_animation.h>
#include <DAEngine\core\graphics\skeletal_animator.h>

#include <DAEngine/physics/physics.h>
#include <DAEngine/physics/physics_shape.h>
#include <DAEngine/physics/physics_rigid_body.h>
#include <DAEngine/core/ecs/rigid_body_component.h>

void CCharacter::initialize()
{
	if (da::core::CScene* scene = da::core::CSceneManager::getScene()) {
		m_entity = scene->createEntity();

		da::platform::CBgfxSkeletalMesh* mesh = new da::platform::CBgfxSkeletalMesh("assets/skeletons/mannequin/SwordRun.fbx", false);
		mesh->getMaterial(0).baseColorFactor = { .45f,0.45f,0.45f,1.f };
		mesh->getMaterial(1).setBaseColorTexture(da::graphics::CTexture2DFactory::Create("assets/skeletons/mannequin/alpha_body_mat.png"));
		mesh->getMaterial(1).setNormalTexture(nullptr);
		mesh->getMaterial(1).metallicFactor = .1500f;
		mesh->getMaterial(1).roughnessFactor = 0.f;
		//da::graphics::CSkeletalAnimation* animation = new da::graphics::CSkeletalAnimation("assets/mannequin/SwordSlash.fbx", mesh);
		da::graphics::CSkeletalAnimation* animation = new da::graphics::CSkeletalAnimation("assets/skeletons/mannequin/SwordRun.fbx", mesh);
		da::graphics::CSkeletalAnimator* animator = new da::graphics::CSkeletalAnimator(animation);
		
		da::core::FComponentRef<da::core::CSkeletalMeshComponent> cc = m_entity->addComponent<da::core::CSkeletalMeshComponent>(mesh, animation, animator);
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
	//component->getSkeletalAnimator()->debugRenderJoints(component->getTransform());
	//da::graphics::CDebugRender::getInstance()->drawLine({ 0.f, 0.f, -10.f }, { 0.f,0.f, 10.f }, 1.f, { 1.f,0.f,0.f,1.f });
#endif
}

void CCharacter::shutdown()
{
	da::core::CSceneManager::getScene()->removeEntity(m_entity);
}
