#pragma once
#include "dastd.h"

namespace da
{

	class CSkeletalAnimator;
	class CSkeletalAnimation;
	class CSkeletalMesh;

	struct FSkeletalAnimGraphNode
	{
		CSkeletalAnimator* Animator;
		float Weight;
	};

	class CSkeletalAnimGraph
	{
	public:
		CSkeletalAnimGraph(CSkeletalMesh* mesh, const std::vector<FSkeletalAnimGraphNode>& nodes);
		void update(float dt);
		std::vector<FSkeletalAnimGraphNode>& getNodes();
		const CSkeletalAnimator* getAnim() const;
		CSkeletalMesh* getMesh();

	private:
		std::vector<FSkeletalAnimGraphNode> m_Nodes;
		CSkeletalAnimator* m_AnimBase;
		CSkeletalMesh* m_Mesh;

	};

}