#pragma once

namespace da::core {

	class IRenderable
	{
	public:
		inline virtual const TArray<FVertexBase, memory::CGraphicsAllocator>& getVertices() const = 0 ;
		inline virtual const TArray<uint32_t, memory::CGraphicsAllocator>& getIndices() const = 0;
	};

}
