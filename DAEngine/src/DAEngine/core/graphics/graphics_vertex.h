#pragma once
#include "daengine/core/containers.h"

namespace da::graphics
{
	struct FVertexBindingDescription {
		unsigned int binding, stride, inputRate;
	};

	struct FVertexInputAttributeDescription {
		unsigned int location, binding, format, offset;
	};
	
	struct FVertexBase
	{
	public:
		Vector3f Pos;
		Vector3f Normal;
		Vector3f Tangent;
		Vector2f TexCoord;

        //Vector3f Color;
		//Vector3f BitTangent;

		inline static FVertexBindingDescription getBindingDescription()
		{
			FVertexBindingDescription result;
			result.binding = 0;
			result.stride = sizeof(FVertexBase);
			result.inputRate = 0;
			return result;
		}

		inline static std::vector<FVertexInputAttributeDescription> getAttributeDescription()
		{
			
			std::vector<FVertexInputAttributeDescription> attributeDescriptions(6);
			/*
			// Position
			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = 106; // VK_FORMAT_R32G32B32_SFLOAT = 106,
			attributeDescriptions[0].offset = offsetof(FVertexBase, Pos);

			// Color
			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = 106; // VK_FORMAT_R32G32B32_SFLOAT = 106
			attributeDescriptions[1].offset = offsetof(FVertexBase, Color);

			// Tex Coord
			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = 106; // VK_FORMAT_R32G32B32_SFLOAT = 106,
			attributeDescriptions[2].offset = offsetof(FVertexBase, TexCoord);

			// Normal
			attributeDescriptions[3].binding = 0;
			attributeDescriptions[3].location = 3;
			attributeDescriptions[3].format = 106; // VK_FORMAT_R32G32B32_SFLOAT = 106,
			attributeDescriptions[3].offset = offsetof(FVertexBase, Normal);

			// Tangent
			attributeDescriptions[4].binding = 0;
			attributeDescriptions[4].location = 4;
			attributeDescriptions[4].format = 106; // VK_FORMAT_R32G32B32_SFLOAT = 106,
			attributeDescriptions[4].offset = offsetof(FVertexBase, Tangent);

			// BitTangent
			attributeDescriptions[5].binding = 0;
			attributeDescriptions[5].location = 5;
			attributeDescriptions[5].format = 106; // VK_FORMAT_R32G32B32_SFLOAT = 106,
			attributeDescriptions[5].offset = offsetof(FVertexBase, BitTangent);
			*/
			return attributeDescriptions;
		}

		bool operator==(const FVertexBase& other) const {
			return false;//Pos == other.Pos && Color == other.Color && TexCoord == other.TexCoord && Normal == other.Normal;
		}
	};
}

namespace std {
	template<> struct hash<da::graphics::FVertexBase> {
		size_t operator()(da::graphics::FVertexBase const& vertex) const {
			return CHashString((const char*)&vertex, sizeof(da::graphics::FVertexBase)).hash();
		}
	};
};
