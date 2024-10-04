#pragma once

namespace da {

	class IRenderable
	{
	public:
		inline virtual const std::vector<FVertexBase>& getVertices() const = 0 ;
		inline virtual const std::vector<uint32_t>& getIndices() const = 0;
		inline virtual void* getNativeVB() const = 0;
		inline virtual void* getNativeIB() const = 0;
	};

}
