#pragma once
#include "daengine/core/containers.h"

namespace da::core
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
        // Smaller first
		Vector2f TexCoord;
        Vector3f Pos;
        Vector3f Color;

		inline static FVertexBindingDescription getBindingDescription()
		{
			FVertexBindingDescription result;
			result.binding = 0;
			result.stride = sizeof(FVertexBase);
			result.inputRate = 0;
			return result;
		}

		inline static TArray<FVertexInputAttributeDescription> getAttributeDescription()
		{
			TArray<FVertexInputAttributeDescription> attributeDescriptions(3);

			attributeDescriptions[0].binding = 0;
			attributeDescriptions[0].location = 0;
			attributeDescriptions[0].format = 106; // VK_FORMAT_R32G32B32_SFLOAT = 106,
			attributeDescriptions[0].offset = offsetof(FVertexBase, Pos);

			attributeDescriptions[1].binding = 0;
			attributeDescriptions[1].location = 1;
			attributeDescriptions[1].format = 106; // VK_FORMAT_R32G32B32_SFLOAT = 106
			attributeDescriptions[1].offset = offsetof(FVertexBase, Color);

			attributeDescriptions[2].binding = 0;
			attributeDescriptions[2].location = 2;
			attributeDescriptions[2].format = 106; // VK_FORMAT_R32G32B32_SFLOAT = 106,
			attributeDescriptions[2].offset = offsetof(FVertexBase, TexCoord);

			return attributeDescriptions;
		}

		bool operator==(const FVertexBase& other) const {
			return Pos == other.Pos && Color == other.Color && TexCoord == other.TexCoord;
		}
	};
}

namespace std {
	template<> struct hash<da::core::FVertexBase> {
		size_t operator()(da::core::FVertexBase const& vertex) const {
			return CHashString((const char*)&vertex, sizeof(da::core::FVertexBase)).hash();
		}
	};
};
