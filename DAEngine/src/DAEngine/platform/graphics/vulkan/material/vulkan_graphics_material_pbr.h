#pragma once
#include "DAEngine/platform/graphics/vulkan/vulkan_graphics_material.h"
#ifdef DA_GRAPHICS_VULKAN

namespace da::platform
{
	class CVulkanGraphicsMaterialPBR : public CVulkanGraphicsMaterial
	{
	public:
		CVulkanGraphicsMaterialPBR(da::graphics::CGraphicsPipeline& pipeline
			, const std::string& albedo
			, const std::string& normal
			, const std::string& roughness
			, const std::string& metallic
			, const std::string& ao);

		virtual void initialize() override;
		virtual void shutdown() override;
		virtual void update(int frame) override;

		virtual std::vector<VkDescriptorPoolSize> getDescriptorPools() override;
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