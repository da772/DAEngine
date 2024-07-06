#pragma once

#include "skeletal_animation.h"

namespace da::graphics {

	class CSkeletalAnimation;

	struct FInterpolatedAnimation {
		CSkeletalAnimation* pAnimation;
		float fInterpolateAmt;
		float fInterpolation = 0.f;
	};

	struct FNodeInfo {
		const FAssimpNodeData* Node;
		glm::mat4 Transform;
		bool Use;
	};

	class CSkeletalAnimator
	{
	public:
		CSkeletalAnimator(CSkeletalAnimation* animation);

		void updateAnimation(float dt);
		void playAnimation(CSkeletalAnimation* pAnimation, float interpolation = 0.f);
		bool getBoneLocalTransform(CHashString name, glm::mat4& out) const;
		bool getBoneWorldTransform(CHashString name, const glm::mat4& modelMat, glm::mat4& out) const;
		bool getBoneWorldPosition(CHashString name, const glm::mat4& modelMat, glm::vec3& out) const;
		bool getBoneWorldRotation(CHashString name, const glm::mat4& modelMat, glm::quat& out) const;
		float getTimeScale() const;
		void setTimeScale(float timeScale);
		float getPlayTime() const;
		void setPlayTime(float time);
		float getMaxPlayTime() const;
		void SetBoneSelector(CHashString boneName);
		const CSkeletalAnimation* getCurrentAnim() const;

#if defined(DA_DEBUG) || defined(DA_RELEASE)
		void debugRenderJoints(const glm::mat4& modelMat);
#endif

		const std::vector<glm::mat4>& getFinalBoneMatrices(size_t index) const
		{
			return m_FinalBoneMatrices[index];
		}

		void copyFinalBoneMatrices(const CSkeletalAnimator* animator);
		void interpFinalBoneMatrices(const CSkeletalAnimator* animator, float dt, float weight);
	private:
		void calculateBoneTransform(const FAssimpNodeData* node, size_t index);
		void interpolateBoneTransforms(const FAssimpNodeData* node, size_t index, float interpolation, bool updateBone);
		void interpolateBoneTransformsInternal(CSkeletalAnimation* anim, const FAssimpNodeData* node, size_t index, float interpolation, bool updateBone, const CHashString& boneSelector);
		const FAssimpNodeData* FindNodeByName(const CHashString& name);

	private:
		std::vector<std::vector<glm::mat4>> m_FinalBoneMatrices;
		CSkeletalAnimation* m_CurrentAnimation;
		std::queue<FInterpolatedAnimation> m_AnimationQueue;
		CHashString m_BoneSelector = HASHSTR("");
		float m_CurrentTime;
		float m_DeltaTime;
		float m_timeScale = 1.f;
		float m_tickTime = 0.f;
	};
}