#include "dapch.h"
#include "animation_manager.h"

#include "DAEngine/core/ecs/scene_manager.h"
#include "DAEngine/core/ecs/skeletal_mesh_component.h"
#include "DAEngine/core/threading/worker_pool.h"
#include "DAEngine/core/graphics/skeletal_animator.h"

namespace da::graphics
{
	std::mutex CAnimationManager::s_mutex;
	std::unordered_map<void*, bool> CAnimationManager::s_animators;

	void CAnimationManager::updateBegin(float deltaTime)
	{
		
		//const da::core::FComponentContainer& skeletalMeshcontainer = da::core::CSceneManager::getScene()->getComponents<da::core::CSkeletalMeshComponent>();
		
		//da::core::CWorkerPool::addJob([deltaTime] {
			CAnimationManager::s_mutex.lock();
			
			const da::core::FComponentContainer& skeletalMeshcontainer = da::core::CSceneManager::getScene()->getComponents<da::core::CSkeletalMeshComponent>();
			for (size_t x = 0; x < skeletalMeshcontainer.getCount(); x++) {
				da::core::CSkeletalMeshComponent* meshComponent = skeletalMeshcontainer.getComponentAtIndex<da::core::CSkeletalMeshComponent>(x);

				da::graphics::CSkeletalAnimator* animator = meshComponent->getSkeletalAnimator();

				if (s_animators.find((void*)animator) != s_animators.end()) {
					continue;
				}

				meshComponent->updateAnimation(deltaTime);
				s_animators[(void*)animator] = true;
			}

			CAnimationManager::s_mutex.unlock();
		//});
		
	}

	void CAnimationManager::updateEnd()
	{
		while (!s_mutex.try_lock())
		{

		}
		CAnimationManager::s_mutex.unlock();
		s_animators = {};
	}
}