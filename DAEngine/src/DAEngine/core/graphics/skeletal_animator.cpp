#include "dapch.h"
#include "skeletal_animator.h"
#include "animated_bone.h"
#include "skeletal_animation.h"
#include <glm/gtx/matrix_decompose.hpp>

#include <assimp/scene.h>
#include "graphics_debug_render.h"
namespace da::graphics
{

	CSkeletalAnimator::CSkeletalAnimator(CSkeletalAnimation* animation)
	{
		m_CurrentTime = 0.0;
		m_CurrentAnimation = animation;

		

		for (int j = 0; j < animation->getMeshCount(); j++) {
			m_FinalBoneMatrices.push_back({});
			m_FinalBoneMatrices[j].reserve(128);
			for (int i = 0; i < 128; i++)
				m_FinalBoneMatrices[j].push_back(glm::mat4(1.0f));
		}
		
	}


	void CSkeletalAnimator::calculateBoneTransform(const FAssimpNodeData* n, size_t idx)
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

			CAnimatedBone* bone = m_CurrentAnimation->FindBone(node.first->name, idx);
			
			if (bone)
			{
				bone->Update(m_CurrentTime);
				nodeTransform = bone->GetLocalTransform();
			}

			glm::mat4 globalTransformation = node.second * nodeTransform;
			const std::unordered_map<CHashString, FBoneInfo>::const_iterator& it = m_CurrentAnimation->getBoneIDMap(idx).find(node.first->name);

			if (it != m_CurrentAnimation->getBoneIDMap(idx).end())
			{
				int index = it->second.id;
				glm::mat4 offset = it->second.offset;
				m_FinalBoneMatrices[idx][index] = globalTransformation * offset;
			}

			for (int i = 0; i < node.first->childrenCount; i++)
			{
				nodeQ.push_back({ &node.first->children[i], globalTransformation });
			}
		}
			
	}

	void CSkeletalAnimator::updateAnimation(float dt)
	{
		if (!m_CurrentAnimation)
		{
			return;
		}

		// sample the animations at 144 fps and interpolate between frames
		constexpr float minSampleRate = 144.f;
		float tickSampleRate = std::max(minSampleRate, m_CurrentAnimation->getTicksPerSecond());
		float tickTime = 1.0f / tickSampleRate;

		m_DeltaTime = dt * m_timeScale;
		m_tickTime += dt;

		m_CurrentTime += m_CurrentAnimation->getTicksPerSecond() * m_DeltaTime;
		m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->getDuration());

		if (m_tickTime < tickTime) {
			return;
		}

		m_tickTime = 0.f;
		for (size_t i = 0; i < m_CurrentAnimation->getMeshCount(); i++) {
			calculateBoneTransform(&m_CurrentAnimation->getRootNode(), i);
		}
	
	}

	void CSkeletalAnimator::playAnimation(CSkeletalAnimation* pAnimation)
	{
		m_CurrentAnimation = pAnimation;
		m_CurrentTime = 0.0f;
	}


	bool CSkeletalAnimator::getBoneLocalTransform(CHashString name, glm::mat4& out) const
	{
		ASSERT(m_CurrentAnimation->getMeshCount() != 0);

		const std::unordered_map<CHashString, FBoneInfo>::const_iterator& it = m_CurrentAnimation->getBoneIDMap(0).find(name);

		if (it == m_CurrentAnimation->getBoneIDMap(0).end())
		{
			return false;
		}

		out = m_FinalBoneMatrices[0][it->second.id] * glm::inverse(it->second.offset);

		return true;
	}

#if defined(DA_DEBUG) || defined(DA_RELEASE)
	void CSkeletalAnimator::debugRenderJoints(const glm::mat4& modelMat)
	{
		for (const std::pair<CHashString, FBoneInfo>& kv : m_CurrentAnimation->getBoneIDMap(0)) {
			glm::mat4 transform = modelMat * (m_FinalBoneMatrices[0][kv.second.id] * glm::inverse(kv.second.offset));
			glm::vec3 scale;
			glm::quat rotation;
			glm::vec3 translation;
			glm::vec3 skew;
			glm::vec4 perspective;
			glm::decompose(transform, scale, rotation, translation, skew, perspective);

			da::graphics::CDebugRender::getInstance()->drawCone(translation, glm::inverse(rotation), scale, { 1.f, 0.f, 1.f, .5f }, false);
		}
	}
#endif

	bool CSkeletalAnimator::getBoneWorldTransform(CHashString name, const glm::mat4& modelMat, glm::mat4& out) const
	{
		ASSERT(m_CurrentAnimation->getMeshCount() != 0);

		const std::unordered_map<CHashString, FBoneInfo>::const_iterator& it = m_CurrentAnimation->getBoneIDMap(0).find(name);

		if (it == m_CurrentAnimation->getBoneIDMap(0).end())
		{
			return false;
		}

		out = modelMat * (m_FinalBoneMatrices[0][it->second.id] * glm::inverse(it->second.offset));

		return true;
	}


	bool CSkeletalAnimator::getBoneWorldPosition(CHashString name, const glm::mat4& modelMat, glm::vec3& out) const
	{
		glm::mat4 m;

		if (!getBoneWorldTransform(name, modelMat, m))
		{
			return false;
		}

		out = m[3];
		return true;
	}

	bool CSkeletalAnimator::getBoneWorldRotation(CHashString name, const glm::mat4& modelMat, glm::quat& out) const
	{
		glm::mat4 transform;
		if (!getBoneWorldTransform(name, modelMat, transform)) {
			return false;
		}

		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(transform, scale, rotation, translation, skew, perspective);

		out = rotation;

		return true;
	}

	float CSkeletalAnimator::getTimeScale() const
	{
		return m_timeScale;
	}

	void CSkeletalAnimator::setTimeScale(float timeScale)
	{
		m_timeScale = timeScale;
	}

	float CSkeletalAnimator::getPlayTime() const
	{
		return m_CurrentTime;
	}

	void CSkeletalAnimator::setPlayTime(float time)
	{
		m_CurrentTime = time;
		if (m_CurrentTime > getMaxPlayTime()) {
			m_CurrentTime = getMaxPlayTime();
		}
	}

	float CSkeletalAnimator::getMaxPlayTime() const
	{
		if (m_CurrentAnimation)
		{
			return m_CurrentAnimation->getDuration();
		}
		return -1.f;
	}




}