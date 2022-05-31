#pragma once
#include "platform/platform.h"

#ifdef DA_GRAPHICS_VULKAN
#include "graphics_api.h"
#include "vulkan/vulkan.h"

namespace da::platform::graphics
{
	class CVulkanGraphicsApi : public CGraphicsApi
	{
	public:
		CVulkanGraphicsApi(const CWindow& windowModule);

		virtual void initalize() override;
		virtual void update() override;
		virtual void shutdown() override;


	};
}
#endif