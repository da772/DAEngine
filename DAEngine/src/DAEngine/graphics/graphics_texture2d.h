#pragma once
#include "core/containers.h"
#include "graphics/graphics_api.h"
#include "core/factory.h"

namespace da::graphics
{
	class CGraphicsTexture2D
	{
	public:
		const std::string& getPath() const;
		inline const void* getTextureNative() const { return m_textureNative; }

	protected:
		CGraphicsTexture2D(const std::string& path);
		CGraphicsTexture2D(const std::string& name, uint32_t width, uint32_t height);
		virtual ~CGraphicsTexture2D();
		std::string m_path;
		void* m_textureNative;
		uint32_t m_width, m_height, m_channels;

		friend class da::core::CFactory<CGraphicsTexture2D>;
	};



}