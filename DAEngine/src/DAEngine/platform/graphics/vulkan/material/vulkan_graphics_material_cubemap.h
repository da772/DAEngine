#pragma once
#include "DAEngine/platform/graphics/vulkan/vulkan_graphics_material.h"
#ifdef DA_GRAPHICS_VULKAN
#include "daengine/platform/graphics/vulkan/vulkan_graphics_texturecube.h"

namespace da::platform
{
	class CVulkanGraphicsMaterialCubeMap: public CVulkanGraphicsMaterial
	{
	public:
		CVulkanGraphicsMaterialCubeMap(da::core::CGraphicsPipeline& pipeline
			, const CBasicString <da::memory::CGraphicsAllocator>& texture);

		virtual void initialize() override;
		virtual void shutdown() override;
		virtual void update(int frame) override;

		virtual TArray<VkDescriptorPoolSize, memory::CGraphicsAllocator> getDescriptorPools() override;
		virtual void getDescriptorSet(int frame) override;

	private:
		CVulkanGraphicsTextureCube m_texture;
	};
}

#endif