#include "dapch.h"
#include "skeletal_animator.h"
#include "animated_bone.h"
#include "skeletal_animation.h"

#include <assimp/scene.h>

namespace da::graphics
{

	CSkeletalAnimator::CSkeletalAnimator(CSkeletalAnimation* animation)
	{
		m_CurrentTime = 0.0;
		m_CurrentAnimation = animation;

		m_FinalBoneMatrices.reserve(128);

		for (int i = 0; i < 128; i++)
			m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
	}


	void CSkeletalAnimator::calculateBoneTransform(const FAssimpNodeData* node, glm::mat4 parentTransform)
	{
		CHashString nodeName = node->name;
		glm::mat4 nodeTransform = node->transformation;

		CAnimatedBone* Bone = m_CurrentAnimation->FindBone(nodeName);

		if (Bone)
		{
			Bone->Update(m_CurrentTime);
			nodeTransform = Bone->GetLocalTransform();
		}

		glm::mat4 globalTransformation = parentTransform * nodeTransform;

		const std::unordered_map<CHashString, FBoneInfo>& boneInfoMap = m_CurrentAnimation->getBoneIDMap();
		auto it = boneInfoMap.find(nodeName);

		if (it != boneInfoMap.end())
		{
			int index = it->second.id;
			glm::mat4 offset = it->second.offset;
			m_FinalBoneMatrices[index] = globalTransformation * offset;
		}

		for (int i = 0; i < node->childrenCount; i++)
			calculateBoneTransform(&node->children[i], globalTransformation);
	}

	void CSkeletalAnimator::updateAnimation(float dt)
	{
		m_DeltaTime = dt;
		if (m_CurrentAnimation)
		{
			m_CurrentTime += m_CurrentAnimation->getTicksPerSecond() * dt;
			m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->getDuration());
			calculateBoneTransform(&m_CurrentAnimation->getRootNode(), glm::mat4(1.0f));
		}
	}

	void CSkeletalAnimator::playAnimation(CSkeletalAnimation* pAnimation)
	{
		m_CurrentAnimation = pAnimation;
		m_CurrentTime = 0.0f;
	}


}