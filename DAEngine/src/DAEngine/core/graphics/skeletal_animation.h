#pragma once
#include "animated_bone.h"
#include "graphics_skmesh.h"

class aiAnimation;
class aiNode;

namespace da::graphics
{
	struct FAssimpNodeData
	{
		glm::mat4 transformation;
		CHashString name;
		int childrenCount;
		std::vector<FAssimpNodeData> children;
	};

	class CSkeletalAnimation
	{
	public:
		CSkeletalAnimation(const std::string& animationPath, FSkeletalMesh* model);

		inline CAnimatedBone* FindBone(const CHashString& name)
		{
			const std::unordered_map<CHashString, CAnimatedBone>::iterator& it = m_Bones.find(name);

			if (it == m_Bones.end())
			{
				return nullptr;
			}

			return &it->second;
		}


		inline float getTicksPerSecond() const { return m_TicksPerSecond; }

		inline float getDuration() const { return m_Duration; }

		inline const FAssimpNodeData& getRootNode() const { return m_RootNode; }

		inline const std::unordered_map<CHashString, FBoneInfo>& getBoneIDMap() const
		{
			return m_BoneInfoMap;
		}

	private:
		void ReadMissingBones(const aiAnimation* animation, FSkeletalMesh& mesh);
		void ReadHeirarchyData(FAssimpNodeData& dest, const aiNode* src);

	private:
		float m_Duration;
		int m_TicksPerSecond;
		std::unordered_map<CHashString, CAnimatedBone> m_Bones;
		FAssimpNodeData m_RootNode;
		std::unordered_map<CHashString, FBoneInfo> m_BoneInfoMap;
	};
}