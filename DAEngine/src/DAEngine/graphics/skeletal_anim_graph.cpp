

#include "skeletal_anim_graph.h"
#include "skeletal_animation.h"
#include "skeletal_animator.h"
#include "graphics_skmesh.h"

namespace da
{

	CSkeletalAnimGraph::CSkeletalAnimGraph(CSkeletalMesh* mesh, const std::vector<FSkeletalAnimGraphNode>& nodes) : m_Mesh(mesh), m_Nodes(nodes)
	{
		ASSERT(nodes.size());
		m_AnimBase = new CSkeletalAnimator(nullptr);
		m_AnimBase->copyFinalBoneMatrices(nodes[0].Animator);
	}

	void CSkeletalAnimGraph::update(float dt)
	{
		PROFILE()
		CSkeletalAnimator* baseAnim = nullptr;
		float maxWeight = -1.f;
		bool requiresUpdate = false;

		for (int i = 0; i < m_Nodes.size(); i++)
		{
			PROFILE("CSkeletalAnimGraph::update::nodeLoop1")
			const FSkeletalAnimGraphNode& node = m_Nodes[i];
			requiresUpdate = node.Animator->updateAnimation(dt, node.Weight <= 0.f) || requiresUpdate;

			if (node.Weight > maxWeight) {
				maxWeight = node.Weight;
				baseAnim = node.Animator;
			}
		}

		if (!baseAnim || !requiresUpdate) return;

		{
			PROFILE("CSkeletalAnimGraph::update::copyBaseAnim")
			m_AnimBase->copyFinalBoneMatrices(baseAnim);
		}

		for (int i = 0; i < m_Nodes.size(); i++)
		{
			PROFILE("CSkeletalAnimGraph::update::nodeLoop2")
			const FSkeletalAnimGraphNode& node = m_Nodes[i];

			if (node.Animator == baseAnim) continue;
			if (node.Weight <= 0.f) continue;

			m_AnimBase->interpFinalBoneMatrices(node.Animator, dt, node.Weight);
		}
	}

	std::vector<da::FSkeletalAnimGraphNode>& CSkeletalAnimGraph::getNodes()
	{
		return m_Nodes;
	}

	const da::CSkeletalAnimator* CSkeletalAnimGraph::getAnim() const
	{
		return m_AnimBase;
	}

	da::CSkeletalMesh* CSkeletalAnimGraph::getMesh()
	{
		return m_Mesh;
	}

}