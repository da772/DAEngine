#pragma once

#pragma once
#include "DAEngine/Core/containers.h"
#include "DAEngine/core/graphics/graphics_api.h"

namespace da::core
{
	class CGraphicsTextureCube
	{
	public:
		CGraphicsTextureCube(const CBasicString<memory::CGraphicsAllocator>& path, CGraphicsApi& graphicsApi);
		inline const void* getTextureNative() const { return m_textureNative; }

	protected:
		CBasicString<memory::CGraphicsAllocator> m_path;
		void* m_textureNative;
		CGraphicsApi& m_graphicsApi;
		uint32_t m_width, m_height, m_channels;
	};



}
