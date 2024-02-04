#pragma once

#include "skeletal_animation.h"

namespace da::graphics {

	class CSkeletalAnimation;

	class CSkeletalAnimator
	{
	public:
		CSkeletalAnimator(CSkeletalAnimation* animation);

		void updateAnimation(float dt);
		void playAnimation(CSkeletalAnimation* pAnimation);

		const std::vector<glm::mat4>& getFinalBoneMatrices() const
		{
			return m_FinalBoneMatrices;
		}
	private:
		void calculateBoneTransform(const FAssimpNodeData* node);

	private:
		std::vector<glm::mat4> m_FinalBoneMatrices;
		CSkeletalAnimation* m_CurrentAnimation;
		float m_CurrentTime;
		float m_DeltaTime;
	};
}