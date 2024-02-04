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

		const std::vector<glm::mat4>& getFinalBoneMatrices(size_t index) const
		{
			return m_FinalBoneMatrices[index];
		}
	private:
		void calculateBoneTransform(const FAssimpNodeData* node, size_t index);

	private:
		std::vector<std::vector<glm::mat4>> m_FinalBoneMatrices;
		CSkeletalAnimation* m_CurrentAnimation;
		float m_CurrentTime;
		float m_DeltaTime;
	};
}