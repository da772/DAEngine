#pragma once
#include "dastd.h"

namespace da::graphics
{
	struct FSkeletalVertexBindingDescription {
		unsigned int binding, stride, inputRate;
	};

	struct FSkeletalVertexInputAttributeDescription {
		unsigned int location, binding, format, offset;
	};

#define MAX_BONE_INFLUENCE 4
#define MAX_BONE_WEIGHTS 128

	struct FSkeletalVertexBase
	{
	public:
		Vector3f Pos;
		Vector3f Normal;
		Vector3f Tangent;
		Vector2f TexCoord;
		int m_BoneIDs[MAX_BONE_INFLUENCE];
		float m_Weights[MAX_BONE_INFLUENCE];
	};
}

namespace std {
	template<> struct hash<da::graphics::FSkeletalVertexBase> {
		size_t operator()(da::graphics::FSkeletalVertexBase const& vertex) const {
			return CHashString((const char*)&vertex, sizeof(da::graphics::FSkeletalVertexBase)).hash();
		}
	};
};
