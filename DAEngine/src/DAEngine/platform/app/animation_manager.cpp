#include "animation_manager.h"

#include "core/ecs/scene_manager.h"
#include "platform/app/ecs/skeletal_mesh_component.h"
#include "core/threading/worker_pool.h"
#include "graphics/skeletal_animator.h"

namespace da
{
	std::mutex CAnimationManager::s_mutex;
	std::unordered_map<void*, bool> CAnimationManager::s_animators;

	void CAnimationManager::updateBegin(float deltaTime)
	{
		PROFILE()
		
		//const da::FComponentContainer& skeletalMeshcontainer = da::CSceneManager::getScene()->getComponents<da::CSkeletalMeshComponent>();
		
		if (da::CScene* scene = da::CSceneManager::getScene()) {
			//da::CWorkerPool::addJob([deltaTime] {
			CAnimationManager::s_mutex.lock();
			if (!da::CSceneManager::getScene()->hasComponents<da::CSkeletalMeshComponent>()) {
				CAnimationManager::s_mutex.unlock();
				return;
			}

			const da::FComponentContainer& skeletalMeshcontainer = da::CSceneManager::getScene()->getComponents<da::CSkeletalMeshComponent>();
			PROFILE_TAG("Count: ", skeletalMeshcontainer.getCount())
			for (size_t x = 0; x < skeletalMeshcontainer.getCount(); x++) {
				da::CSkeletalMeshComponent* meshComponent = skeletalMeshcontainer.getComponentAtIndex<da::CSkeletalMeshComponent>(x);
				da::CSkeletalAnimator* animator = meshComponent->getSkeletalAnimator();

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