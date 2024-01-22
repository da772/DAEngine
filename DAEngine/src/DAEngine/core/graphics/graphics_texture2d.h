#pragma once
#include "DAEngine/Core/containers.h"
#include "DAEngine/core/graphics/graphics_api.h"

namespace da::graphics
{
	class CGraphicsTexture2D
	{
	public:
		CGraphicsTexture2D(const std::string& path);
		CGraphicsTexture2D(const std::string& name, uint32_t width, uint32_t height);
		inline virtual ~CGraphicsTexture2D() {};
		inline const void* getTextureNative() const { return m_textureNative; }

	protected:
		std::string m_path;
		void* m_textureNative;
		uint32_t m_width, m_height, m_channels;
	};



}