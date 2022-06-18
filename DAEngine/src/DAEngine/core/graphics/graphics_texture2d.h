#pragma once
#include "Core/containers.h"
#include "core/graphics/graphics_api.h"

namespace da::core
{
	class CGraphicsTexture2D
	{
	public:
		CGraphicsTexture2D(const CString& path, CGraphicsApi& graphicsApi);

		inline const void* getTextureNative() const { return m_textureNative; }

	protected:
		CString m_path;
		void* m_textureNative;
		CGraphicsApi& m_graphicsApi;
		uint32_t m_width, m_height, m_channels;
	};



}