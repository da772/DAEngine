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


	void CSkeletalAnimator::calculateBoneTransform(const FAssimpNodeData* n)
	{
		std::vector<std::pair<const FAssimpNodeData*, glm::mat4>> nodeQ;
		nodeQ.push_back({ n, glm::mat4(1) });

		while (!nodeQ.empty())
		{
			const std::pair<const FAssimpNodeData*, glm::mat4> node = nodeQ.back();
			nodeQ.pop_back();

			if (!node.first) {
				continue;
			}

			glm::mat4 nodeTransform = node.first->transformation;

			CAnimatedBone* bone = m_CurrentAnimation->FindBone(node.first->name);
			
			if (bone)
			{
				bone->Update(m_CurrentTime);
				nodeTransform = bone->GetLocalTransform();
			}

			glm::mat4 globalTransformation = node.second * nodeTransform;
			const std::unordered_map<CHashString, FBoneInfo>::const_iterator& it = m_CurrentAnimation->getBoneIDMap().find(node.first->name);

			if (it != m_CurrentAnimation->getBoneIDMap().end())
			{
				int index = it->second.id;
				glm::mat4 offset = it->second.offset;
				m_FinalBoneMatrices[index] = globalTransformation * offset;
			}

			for (int i = 0; i < node.first->childrenCount; i++)
			{
				nodeQ.push_back({ &node.first->children[i], globalTransformation });
			}
		}
			
	}

	void CSkeletalAnimator::updateAnimation(float dt)
	{
		m_DeltaTime = dt;
		if (m_CurrentAnimation)
		{
			m_CurrentTime += m_CurrentAnimation->getTicksPerSecond() * dt;
			m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->getDuration());
			calculateBoneTransform(&m_CurrentAnimation->getRootNode());
		}
	}

	void CSkeletalAnimator::playAnimation(CSkeletalAnimation* pAnimation)
	{
		m_CurrentAnimation = pAnimation;
		m_CurrentTime = 0.0f;
	}


}