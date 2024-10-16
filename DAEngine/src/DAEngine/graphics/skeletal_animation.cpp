
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
#include <core/asset/asset.h>

namespace da
{

	CSkeletalAnimation::CSkeletalAnimation(const std::string& animationPath, CSkeletalMesh* model, bool binary) : m_AnimName(animationPath.c_str())
	{
		if (binary)
		{
			std::fstream in;
			in.open(animationPath.c_str(), std::ios::in | std::ios::binary);

			in.read((char*)&m_Duration, sizeof(float));
			in.read((char*)&m_TicksPerSecond, sizeof(float));

			std::deque<da::FAssimpNodeData*> queue = { &m_RootNode };

			while (!queue.empty())
			{
				da::FAssimpNodeData* data = queue.front();
				size_t nameSize = 0;
				char buffer[1024];
				queue.erase(queue.begin());
				in.read((char*)&data->transformation, sizeof(glm::mat4));
				in.read((char*)&nameSize, sizeof(nameSize));
				in.read((char*)buffer, nameSize);
				buffer[nameSize] = '\0';
				data->name = CHashString(buffer, nameSize);
				in.read((char*)&data->childrenCount, sizeof(int));
				data->children.resize(data->childrenCount);

				for (int i = 0; i < data->children.size(); i++)
				{
					queue.push_front(&data->children[i]);
				}
			}

			uint64_t validityCheck = 0;
			in.read((char*)&validityCheck, sizeof(uint64_t));
			ASSERT(validityCheck == ANIM_PARSER_VALIDITY_SEC_01);

			size_t boneInfoMapSize = 0;
			in.read((char*)&boneInfoMapSize, sizeof(size_t));

			for (size_t i = 0; i < boneInfoMapSize; i++)
			{
				size_t nameSize = 0;
				in.read((char*)&nameSize, sizeof(nameSize));

				char buffer[1024];
				in.read(buffer, nameSize);
				buffer[nameSize] = 0;

				CHashString str = CHashString(buffer, nameSize);
				da::FBoneInfo boneInfo;
				in.read((char*)&boneInfo, sizeof(da::FBoneInfo));

				const FBoneInfo& newBone = model->addBone(str);
				boneInfo.id = newBone.id;
				m_BoneInfoMap[str] = newBone;
			}

			validityCheck = 0;
			in.read((char*)&validityCheck, sizeof(uint64_t));
			ASSERT(validityCheck == ANIM_PARSER_VALIDITY_SEC_02);

			size_t boneSize = 0;;
			in.read((char*)&boneSize, sizeof(size_t));

			for (size_t i = 0; i < boneSize; i++)
			{
				da::CAnimatedBone bone;
				size_t nameSize = 0;
				char buffer[1024];
				in.read((char*)&nameSize, sizeof(nameSize));
				in.read(buffer, nameSize);
				buffer[nameSize] = 0;

				bone.m_Name = CHashString(buffer, nameSize);

				in.read((char*)&bone.m_ID, sizeof(int));
				in.read((char*)&bone.m_LocalTransform, sizeof(glm::mat4));

				in.read((char*)&bone.m_NumPositions, sizeof(int));
				bone.m_Positions.resize(bone.m_NumPositions);
				for (int i = 0; i < bone.m_NumPositions; i++)
				{
					in.read((char*)&bone.m_Positions[i], sizeof(da::FKeyPosition));
				}

				in.read((char*)&bone.m_NumRotations, sizeof(int));
				bone.m_Rotations.resize(bone.m_NumRotations);
				for (int i = 0; i < bone.m_NumRotations; i++)
				{
					in.read((char*)&bone.m_Rotations[i], sizeof(da::FKeyRotation));
				}

				in.read((char*)&bone.m_NumScalings, sizeof(int));
				bone.m_Scales.resize(bone.m_NumScalings);
				for (int i = 0; i < bone.m_NumScalings; i++)
				{
					in.read((char*)&bone.m_Scales[i], sizeof(da::FKeyScale));
				}

				bone.m_ID = m_BoneInfoMap[bone.m_Name].id;
				m_Bones[bone.m_Name] = std::move(bone);
			}

			validityCheck = 0;
			in.read((char*)&validityCheck, sizeof(uint64_t));
			ASSERT(validityCheck == ANIM_PARSER_VALIDITY_SEC_03);

			in.close();
			return;
		}

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

	bool CSkeletalAnimation::compare(const CSkeletalAnimation* animations) const
	{
		for (const std::pair<CHashString, CAnimatedBone>& kv : m_Bones)
		{
			const std::unordered_map<CHashString, CAnimatedBone>::const_iterator& it = animations->m_Bones.find(kv.first);
			
			if (it->second != kv.second)
			{
				return false;
			}
		}

		for (const std::pair<CHashString, FBoneInfo>& kv : m_BoneInfoMap)
		{
			const std::unordered_map<CHashString, FBoneInfo>::const_iterator& it = animations->m_BoneInfoMap.find(kv.first);
			if (it->second != kv.second)
			{
				break;
				//return false;
			}
		}

		std::queue<std::pair<const FAssimpNodeData*, const FAssimpNodeData*>> queue;
		queue.push({ &m_RootNode, &animations->m_RootNode });


		while (!queue.empty())
		{
			std::pair<const FAssimpNodeData*, const FAssimpNodeData*> node = queue.front();
			queue.pop();

			if (*node.first != *node.second)
			{
				return false;
			}

			for (int i = 0; i < node.first->childrenCount; i++)
			{
				queue.push({ &node.first->children[i],&node.second->children[i] });
			}
		}

		return true;
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