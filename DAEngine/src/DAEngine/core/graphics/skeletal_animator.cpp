#include "dapch.h"
#include "skeletal_animator.h"
#include "animated_bone.h"
#include "skeletal_animation.h"
#include <glm/gtx/matrix_decompose.hpp>

#include <assimp/scene.h>
#include "graphics_debug_render.h"
namespace da::graphics
{

	CSkeletalAnimator::CSkeletalAnimator(CSkeletalAnimation* animation, bool looping) : m_looping(looping)
	{
		m_CurrentTime = 0.0;
		m_CurrentAnimation = animation;

		if (!animation) return;

		for (int j = 0; j < animation->getMeshCount(); j++) {
			m_FinalBoneMatrices.push_back({});
			m_FinalBoneMatrices[j].reserve(128);
			for (int i = 0; i < 128; i++)
				m_FinalBoneMatrices[j].push_back(glm::mat4(1.0f));
		}
		
	}


	void CSkeletalAnimator::calculateBoneTransform(const FAssimpNodeData* n, size_t idx)
	{
		std::vector<FNodeInfo> nodeQ;
		nodeQ.push_back({ n, glm::mat4(1), true });

		while (!nodeQ.empty())
		{
			const FNodeInfo node = nodeQ.back();
			nodeQ.pop_back();

			if (!node.Node) {
				continue;
			}

			glm::mat4 nodeTransform = node.Node->transformation;

			CAnimatedBone* bone = m_CurrentAnimation->FindBone(node.Node->name, idx);
			
			if (bone)
			{
				bone->Update(m_CurrentTime);
				nodeTransform = bone->GetLocalTransform();
			}

			glm::mat4 globalTransformation = node.Transform * nodeTransform;
			const std::unordered_map<CHashString, FBoneInfo>::const_iterator& it = m_CurrentAnimation->getBoneIDMap(idx).find(node.Node->name);

			if (it != m_CurrentAnimation->getBoneIDMap(idx).end())
			{
				int index = it->second.id;
				glm::mat4 offset = it->second.offset;
				m_FinalBoneMatrices[idx][index] = globalTransformation * offset;
			}

			for (int i = 0; i < node.Node->childrenCount; i++)
			{
				nodeQ.push_back({ &node.Node->children[i], globalTransformation, node.Use });
			}
		}
			
	}

	void CSkeletalAnimator::interpolateBoneTransformsInternal(CSkeletalAnimation* animation, const FAssimpNodeData* n, size_t idx, float interpolation, bool updateBone, const std::vector<FBoneSelector>& boneSelector)
	{
		std::vector<FNodeInfo> nodeQ;	
		const std::vector<FBoneSelector>::const_iterator& it = std::find_if(boneSelector.begin(), boneSelector.end(), [&n](const FBoneSelector& other) { return other.Bone == n->name; });
		
		bool selected = boneSelector.empty() || it != boneSelector.end();
		bool selectChildren = boneSelector.empty();

		if (selected && it != boneSelector.end()) {
			selectChildren = it->Children;
		}

		nodeQ.push_back({ n, glm::mat4(1), selected, selectChildren });

		ASSERT(animation);

		while (!nodeQ.empty())
		{
			const FNodeInfo node = nodeQ.back();
			nodeQ.pop_back();

			if (!node.Node) {
				continue;
			}

			glm::mat4 nodeTransform = node.Node->transformation;

			CAnimatedBone* bone = animation->FindBone(node.Node->name, idx);

			if (bone)
			{
				if (updateBone) bone->Update(0.f);
				nodeTransform = bone->GetLocalTransform();
			}

			const std::unordered_map<CHashString, FBoneInfo>& animBoneMap = animation->getBoneIDMap(idx);
			const std::unordered_map<CHashString, FBoneInfo>::const_iterator& it = animBoneMap.find(node.Node->name);

			glm::mat4 globalTransformation = node.Transform * nodeTransform;

			if (it != animBoneMap.end())
			{
				int index = it->second.id;
				ASSERT(index < 128);

				glm::mat4 offset = it->second.offset;

				const std::unordered_map<CHashString, FBoneInfo>::const_iterator& it = m_CurrentAnimation->getBoneIDMap(idx).find(node.Node->name);

				glm::mat4 finalMat = globalTransformation;

				if (it != m_CurrentAnimation->getBoneIDMap(idx).end() && node.Use) {
					glm::vec3 scale2;
					glm::quat rotation2;
					glm::vec3 translation2;
					glm::vec3 skew2;
					glm::vec4 perspective2;
					glm::decompose(finalMat, scale2, rotation2, translation2, skew2, perspective2);

					glm::vec3 scale1;
					glm::quat rotation1;
					glm::vec3 translation1;
					glm::vec3 skew1;
					glm::vec4 perspective1;
					glm::decompose(m_FinalBoneMatrices[idx][it->second.id] * glm::inverse(it->second.offset), scale1, rotation1, translation1, skew1, perspective1);

					translation1 = glm::mix(translation1, translation2, interpolation);
					rotation1 = glm::slerp(rotation1, rotation2, interpolation);
					scale1 = glm::mix(scale1, scale1, interpolation);

					finalMat = glm::translate(glm::mat4(1.f), translation1) * glm::toMat4(glm::normalize(rotation1)) * glm::scale(glm::mat4(1.f), scale1);;

				}

				if (node.Use) {
					m_FinalBoneMatrices[idx][index] = finalMat * offset;
				}
				else {
					m_FinalBoneMatrices[idx][index] = (m_FinalBoneMatrices[idx][it->second.id] * glm::inverse(it->second.offset)) * offset;
				}
			}

			for (int i = 0; i < node.Node->childrenCount; i++)
			{
				const std::vector<FBoneSelector>::const_iterator& it = std::find_if(boneSelector.begin(), boneSelector.end(), [&node, i](const FBoneSelector& other) { return other.Bone == node.Node->children[i].name; });
				
				bool selected = boneSelector.empty() || it != boneSelector.end();
				bool selectChildren = boneSelector.empty() || node.UseChildren || (selected && it->Children);

				nodeQ.push_back({ &node.Node->children[i], globalTransformation, selected || selectChildren, selectChildren});
			}
		}
	}

	void CSkeletalAnimator::interpolateBoneTransforms(const FAssimpNodeData* n, size_t idx, float interpolation, bool updateBone)
	{
		FInterpolatedAnimation& pair = m_AnimationQueue.front();
		CSkeletalAnimation* animation = pair.pAnimation;
		interpolateBoneTransformsInternal(animation, n, idx, interpolation, updateBone, { { HASHSTR(""), true} });
	}

	bool CSkeletalAnimator::updateAnimation(float dt, bool disablePlay)
	{
		PROFILE()
		if (!m_CurrentAnimation)
		{
			return false;
		}

		// sample the animations at 144 fps and interpolate between frames
		constexpr float minSampleRate = 60.f;
		float tickSampleRate = std::max(minSampleRate, m_CurrentAnimation->getTicksPerSecond());
		float tickTime = 1.0f / tickSampleRate;

		m_DeltaTime = dt * m_timeScale;
		m_tickTime += dt;

		if (!m_AnimationQueue.empty())
		{
			if (m_tickTime < tickTime) {
				return false;
			}

			FInterpolatedAnimation& anim = m_AnimationQueue.front();
			
			const float interpTime = 1.f / (anim.fInterpolateAmt / tickTime);

			if (!disablePlay) {
				for (size_t i = 0; i < m_CurrentAnimation->getMeshCount(); i++) {
					interpolateBoneTransforms(&anim.pAnimation->getRootNode(), i, interpTime, true);
				}
			}
			
			anim.fInterpolation += dt;
		
			if (anim.fInterpolation >= anim.fInterpolateAmt) {
				m_AnimationQueue.pop();
				m_CurrentTime = 0.f;
				m_CurrentAnimation = anim.pAnimation;
			}
			return true;
		}

		m_CurrentTime += m_CurrentAnimation->getTicksPerSecond() * m_DeltaTime;

		if (!m_looping && m_CurrentTime > m_CurrentAnimation->getDuration()) {
			m_CurrentTime = m_CurrentAnimation->getDuration();
			disablePlay = true;
		}
		else {
			m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->getDuration());
		}
	
		if (m_tickTime < tickTime) {
			return false;
		}

		m_tickTime = 0.f;

		if (disablePlay) {
			return false;
		}

		for (size_t i = 0; i < m_CurrentAnimation->getMeshCount(); i++) {
			calculateBoneTransform(&m_CurrentAnimation->getRootNode(), i);
		}

		return true;
	}

	void CSkeletalAnimator::playAnimation(CSkeletalAnimation* pAnimation, float interpolation)
	{
		if (interpolation > 0.f) {
			m_AnimationQueue.push({ pAnimation, interpolation });
			return;
		}

		m_CurrentAnimation = pAnimation;
		m_CurrentTime = 0.0f;
	}


#if defined(DA_DEBUG) || defined(DA_RELEASE)
	void CSkeletalAnimator::debugRenderJoints(const glm::mat4& modelMat)
	{
		if (!m_CurrentAnimation) {
			return;
		}

		std::queue<const FAssimpNodeData*> bones;
		bones.push(&m_CurrentAnimation->getRootNode());

		std::unordered_map<CHashString, glm::mat4> cachedBones;

		while (!bones.empty())
		{
			const FAssimpNodeData* b = bones.front();
			glm::vec3 baseTranslation;
			bones.pop();
			{

				const std::unordered_map<CHashString, FBoneInfo>::const_iterator& bIt = m_CurrentAnimation->getBoneIDMap(0).find(b->name);
				
				if (bIt == m_CurrentAnimation->getBoneIDMap(0).end()) {
					for (size_t i = 0; i < b->children.size(); i++) {
						bones.push(&b->children[i]);
					}
					continue;
				}

				const FBoneInfo& bInfo = bIt->second;

				glm::mat4 transform;
				const std::unordered_map<CHashString, glm::mat4>::iterator& it = cachedBones.find(b->name);

				if (it == cachedBones.end())
				{
					getBoneWorldTransform(b->name, modelMat, transform);
					cachedBones[b->name] = transform;
				}
				else {
					transform = cachedBones[b->name];
				}

				glm::vec3 scale;
				glm::quat rotation;

				glm::vec3 skew;
				glm::vec4 perspective;
				glm::decompose(transform, scale, rotation, baseTranslation, skew, perspective);
			}

			for (size_t i = 0; i < b->children.size(); i++) {

				const std::unordered_map<CHashString, FBoneInfo>::const_iterator& cbIt = m_CurrentAnimation->getBoneIDMap(0).find(b->children[i].name);

				if (cbIt == m_CurrentAnimation->getBoneIDMap(0).end()) {
					bones.push(&b->children[i]);
					continue;
				}

				const FBoneInfo& bInfo = m_CurrentAnimation->getBoneIDMap(0).at(b->children[i].name);

				glm::mat4 transform;
				const std::unordered_map<CHashString, glm::mat4>::iterator& it = cachedBones.find(b->children[i].name);

				if (it == cachedBones.end())
				{
					getBoneWorldTransform(b->children[i].name, modelMat, transform);
					cachedBones[b->children[i].name] = transform;
				}
				else {
					transform = cachedBones[b->children[i].name];
				}

				glm::vec3 scale;
				glm::quat rotation;
				glm::vec3 translation;
				glm::vec3 skew;
				glm::vec4 perspective;
				glm::decompose(transform, scale, rotation, translation, skew, perspective);
				da::graphics::CDebugRender::drawCone(translation, rotation, {.0001f,.0001f,.0001f}, { 1.f, 0.f, 1.f, 1.f }, false, true);
				da::graphics::CDebugRender::drawLine(baseTranslation, translation, .025f, { 1.f,0.f,0.f,1.f }, false, true);

				bones.push(&b->children[i]);
			}
		}
	}
#endif

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

	bool CSkeletalAnimator::getBoneWorldTransform(CHashString name, const glm::mat4& modelMat, glm::mat4& out) const
	{
		PROFILE();
		ASSERT(m_CurrentAnimation->getMeshCount() != 0);

		for (size_t i = 0; i < m_CurrentAnimation->getMeshCount(); i++) {
			const std::unordered_map<CHashString, FBoneInfo>::const_iterator& it = m_CurrentAnimation->getBoneIDMap(i).find(name);

			if (it == m_CurrentAnimation->getBoneIDMap(i).end())
			{
				continue;
			}

			out = modelMat * (m_FinalBoneMatrices[i][it->second.id] * glm::inverse(it->second.offset));

			if (glm::all(glm::isnan(out[3])))
			{
				continue;
			}

			
			return true;
		}

		

		return false;
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

	const da::graphics::CSkeletalAnimation* CSkeletalAnimator::getCurrentAnim() const
	{
		return m_CurrentAnimation;
	}

	void CSkeletalAnimator::copyFinalBoneMatrices(const CSkeletalAnimator* animator)
	{
		m_CurrentAnimation = animator->m_CurrentAnimation;
		m_FinalBoneMatrices = animator->m_FinalBoneMatrices;
	}

	void CSkeletalAnimator::interpFinalBoneMatrices(const CSkeletalAnimator* animator, float dt, float weight)
	{
		for (size_t i = 0; i < m_CurrentAnimation->getMeshCount(); i++) {
			interpolateBoneTransformsInternal(const_cast<CSkeletalAnimation*> (animator->getCurrentAnim()), &animator->getCurrentAnim()->getRootNode(), i, weight, false, animator->m_BoneSelectors);
		}
	}

	const da::graphics::FAssimpNodeData* CSkeletalAnimator::FindNodeByName(const CHashString& name)
	{
		std::queue<const FAssimpNodeData*> nodes;
		nodes.push(&m_CurrentAnimation->getRootNode());
		while (!nodes.empty()) {
			const FAssimpNodeData* node = nodes.front();
			nodes.pop();

			if (node->name == name) {
				return node;
			}

			for (const FAssimpNodeData& child : node->children)
			{
				nodes.push(&child);
			}
		}

		return nullptr;
	}

	void CSkeletalAnimator::SetBoneSelector(const std::vector<FBoneSelector>& boneName)
	{
		m_BoneSelectors = boneName;
	}

}