#pragma once

namespace da::core {

	class IRenderable
	{
	public:
		inline virtual const std::vector<FVertexBase>& getVertices() const = 0 ;
		inline virtual const std::vector<uint32_t>& getIndices() const = 0;
	};

}
