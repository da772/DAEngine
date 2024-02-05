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
		bool getBoneLocalTransform(CHashString name, glm::mat4& out) const;
		bool getBoneWorldTransform(CHashString name, const glm::mat4& modelMat, glm::mat4& out) const;
		bool getBoneWorldPosition(CHashString name, const glm::mat4& modelMat, glm::vec3& out) const;
		bool getBoneWorldRotation(CHashString name, const glm::mat4& modelMat, glm::quat& out) const;
		float getTimeScale() const;
		void setTimeScale(float timeScale);
		float getPlayTime() const;
		void setPlayTime(float time);
		float getMaxPlayTime() const;

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
		float m_timeScale = 1.f;
	};
}