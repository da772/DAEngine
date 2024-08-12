#include "animation_manager.h"

#include "core/ecs/scene_manager.h"
#include "platform/app/ecs/skeletal_mesh_component.h"
#include "core/threading/worker_pool.h"
#include "graphics/skeletal_animator.h"

namespace da::graphics
{
	std::mutex CAnimationManager::s_mutex;
	std::unordered_map<void*, bool> CAnimationManager::s_animators;

	void CAnimationManager::updateBegin(float deltaTime)
	{
		PROFILE()
		
		//const da::core::FComponentContainer& skeletalMeshcontainer = da::core::CSceneManager::getScene()->getComponents<da::core::CSkeletalMeshComponent>();
		
		if (da::core::CScene* scene = da::core::CSceneManager::getScene()) {
			//da::core::CWorkerPool::addJob([deltaTime] {
			CAnimationManager::s_mutex.lock();
			if (!da::core::CSceneManager::getScene()->hasComponents<da::core::CSkeletalMeshComponent>()) {
				CAnimationManager::s_mutex.unlock();
				return;
			}

			const da::core::FComponentContainer& skeletalMeshcontainer = da::core::CSceneManager::getScene()->getComponents<da::core::CSkeletalMeshComponent>();
			PROFILE_TAG("Count: ", skeletalMeshcontainer.getCount())
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
		}
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