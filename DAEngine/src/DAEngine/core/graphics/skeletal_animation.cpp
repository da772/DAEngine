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

namespace da::graphics
{

	CSkeletalAnimation::CSkeletalAnimation(const std::string& animationPath, FSkeletalMesh* model)
	{
		Assimp::Importer importer;
		const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate
			| aiProcess_GenSmoothNormals
			| aiProcess_ConvertToLeftHanded
			| aiProcess_CalcTangentSpace
			| aiProcess_PreTransformVertices
			| aiProcess_TransformUVCoords
			| aiProcess_FlipUVs);
		assert(scene && scene->mRootNode);
		auto animation = scene->mAnimations[0];
		m_Duration = animation->mDuration;
		m_TicksPerSecond = animation->mTicksPerSecond;
		ReadHeirarchyData(m_RootNode, scene->mRootNode);
		ReadMissingBones(animation, *model);
	}

	void CSkeletalAnimation::ReadMissingBones(const aiAnimation* animation, FSkeletalMesh& mesh)
	{
		int size = animation->mNumChannels;

		auto& boneInfoMap = mesh.BoneMap;//getting m_BoneInfoMap from Model class
		int& boneCount = mesh.BoneCounter; //getting the m_BoneCounter from Model class

		//reading channels(bones engaged in an animation and their keyframes)
		for (int i = 0; i < size; i++)
		{
			auto channel = animation->mChannels[i];
			CHashString boneName = channel->mNodeName.data;

			if (boneInfoMap.find(boneName) == boneInfoMap.end())
			{
				boneInfoMap[boneName].id = boneCount;
				boneCount++;
			}
			m_Bones.push_back(CAnimatedBone(channel->mNodeName.data,
				boneInfoMap[channel->mNodeName.data].id, channel));
		}

		m_BoneInfoMap = boneInfoMap;
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