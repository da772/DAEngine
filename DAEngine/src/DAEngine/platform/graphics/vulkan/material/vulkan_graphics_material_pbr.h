#pragma once
#include "DAEngine/platform/graphics/vulkan/vulkan_graphics_material.h"
#ifdef DA_GRAPHICS_VULKAN

namespace da::platform
{
	class CVulkanGraphicsMaterialPBR : public CVulkanGraphicsMaterial
	{
	public:
		CVulkanGraphicsMaterialPBR(da::core::CGraphicsPipeline& pipeline
			, const CBasicString <da::memory::CGraphicsAllocator>& albedo
			, const CBasicString <da::memory::CGraphicsAllocator>& normal
			, const CBasicString <da::memory::CGraphicsAllocator>& roughness
			, const CBasicString <da::memory::CGraphicsAllocator>& metallic
			, const CBasicString <da::memory::CGraphicsAllocator>& ao);

		virtual void initialize() override;
		virtual void shutdown() override;
		virtual void update(int frame) override;

		virtual TArray<VkDescriptorPoolSize, memory::CGraphicsAllocator> getDescriptorPools() override;
		virtual void getDescriptorSet(int frame) override;

	private:
		CVulkanGraphicsTexture2D m_albedo;
		CVulkanGraphicsTexture2D m_normal;
		CVulkanGraphicsTexture2D m_roughness;
		CVulkanGraphicsTexture2D m_metallic;
		CVulkanGraphicsTexture2D m_ao;
	};
}

#endif