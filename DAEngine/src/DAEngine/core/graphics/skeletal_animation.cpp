#include "dapch.h"
#include "skeletal_animation.h"


#if !defined(DA_TEST)
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/material.h>
#include <assimp/pbrmaterial.h>
#include <glm/fwd.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/gtx/quaternion.hpp>
#include "assimp_conversion_helpers.h"
#endif
#include <asset/asset.h>

namespace da::graphics
{

	CSkeletalAnimation::CSkeletalAnimation(const std::string& animationPath, CSkeletalMesh* model) : m_AnimName(animationPath.c_str())
	{
		CAsset asset(animationPath);
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFileFromMemory(asset.data(), asset.size(),
			  aiProcess_Triangulate
			| aiProcess_GenSmoothNormals
			| aiProcess_CalcTangentSpace
			| aiProcess_FixInfacingNormals
			| aiProcess_PreTransformVertices
			| aiProcess_TransformUVCoords
			| aiProcess_FlipUVs
			| aiProcess_FlipWindingOrder);
		ASSERT(scene && scene->mRootNode);
		auto animation = scene->mAnimations[0];
		m_Duration = animation->mDuration;
		m_TicksPerSecond = animation->mTicksPerSecond;
		ReadHeirarchyData(m_RootNode, scene->mRootNode);
		ReadMissingBones(animation, *model);
	}

	CSkeletalAnimation::CSkeletalAnimation(const std::string& name, const CSkeletalAnimation* copy) : m_AnimName(name.c_str())
	{
		m_Bones = copy->m_Bones;;
		m_RootNode = copy->m_RootNode;
		m_BoneInfoMap = copy->m_BoneInfoMap;
	}

	void CSkeletalAnimation::ReadMissingBones(const aiAnimation* animation, CSkeletalMesh& mesh)
	{
		int size = animation->mNumChannels;
		const std::unordered_map<CHashString, FBoneInfo>& boneInfoMap = mesh.getBoneMap();//getting m_BoneInfoMap from Model class

		//reading channels(bones engaged in an animation and their keyframes)
		for (int i = 0; i < size; i++)
		{
			auto channel = animation->mChannels[i];
			CHashString boneName = channel->mNodeName.data;

			bool boneMapped = m_BoneInfoMap.find(boneName) != m_BoneInfoMap.end();

			const std::unordered_map<CHashString, FBoneInfo>::const_iterator& it = boneInfoMap.find(boneName);
			const FBoneInfo& boneInfo = it == boneInfoMap.end() ? mesh.addBone(boneName) : it->second;

			if (!boneMapped)
			{
				m_BoneInfoMap[boneName].id = boneInfo.id;
				m_BoneInfoMap[boneName].offset = boneInfo.offset;
			}

			if (m_Bones.find(boneName) != m_Bones.end())
			{
				continue;
			}

			m_Bones[boneName] = CAnimatedBone(channel->mNodeName.data, boneInfo.id, channel);
		}
	}

	void CSkeletalAnimation::ReadHeirarchyData(FAssimpNodeData& dest, const aiNode* src)
	{
		ASSERT(src);

		dest.name = src->mName.data;
		dest.transformation = AssimpGLMHelpers::ConvertMatrixToGLMFormat(src->mTransformation);
		dest.childrenCount = src->mNumChildren;

		for (int i = 0; i < src->mNumChildren; i++)
		{
			FAssimpNodeData newData;
			ReadHeirarchyData(newData, src->mChildren[i]);
			dest.children.push_back(newData);
		}
	}
	

}