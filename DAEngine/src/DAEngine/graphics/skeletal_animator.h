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
		bool UseChildren;
	};

	struct FBoneSelector
	{
		CHashString Bone;
		bool Children;
	};

	class CSkeletalAnimator
	{
	public:
		CSkeletalAnimator(CSkeletalAnimation* animation, bool looping = true);

		bool updateAnimation(float dt, bool disablePlay = false);
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
		void SetBoneSelector(const std::vector<FBoneSelector>& boneNames);
		const CSkeletalAnimation* getCurrentAnim() const;

#if defined(DA_DEBUG) || defined(DA_RELEASE)
		void debugRenderJoints(const glm::mat4& modelMat);
#endif

		const std::vector<glm::mat4>& getFinalBoneMatrices() const
		{
			return m_FinalBoneMatrices;
		}

		void copyFinalBoneMatrices(const CSkeletalAnimator* animator);
		void interpFinalBoneMatrices(const CSkeletalAnimator* animator, float dt, float weight);
	private:
		void calculateBoneTransform(const FAssimpNodeData* node);
		void interpolateBoneTransforms(const FAssimpNodeData* node, float interpolation, bool updateBone);
		void interpolateBoneTransformsInternal(CSkeletalAnimation* anim, const FAssimpNodeData* node, float interpolation, bool updateBone, const std::vector<FBoneSelector>& boneSelector);
		const FAssimpNodeData* FindNodeByName(const CHashString& name);

	private:
		std::vector<glm::mat4> m_FinalBoneMatrices;
		CSkeletalAnimation* m_CurrentAnimation;
		std::queue<FInterpolatedAnimation> m_AnimationQueue;
		std::vector<FBoneSelector> m_BoneSelectors;
		float m_CurrentTime;
		float m_DeltaTime;
		float m_timeScale = 1.f;
		float m_tickTime = 0.f;
		bool m_looping : 1 = true;
	};
}