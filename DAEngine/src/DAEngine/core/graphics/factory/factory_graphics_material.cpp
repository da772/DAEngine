#include "dapch.h"
#include "factory_graphics_material.h"
#include "DAEngine/core/graphics/graphics_pipeline.h"
#include "DAEngine/core/core.h"

#ifdef DA_GRAPHICS_VULKAN
#include "DAEngine/platform/graphics/vulkan/material/vulkan_graphics_material_pbr.h"
#include "DAEngine/platform/graphics/vulkan/material/vulkan_graphics_material_cubemap.h"
#endif

namespace da::core
{

	da::core::CMaterial* CMaterialFactory::CreatePBR(CGraphicsPipeline& pipeline
		, const std::string& albedo
		, const std::string& normal
		, const std::string& roughness
		, const std::string& metallic
		, const std::string& ao
	)
	{
#ifdef DA_GRAPHICS_VULKAN
		return new da::platform::CVulkanGraphicsMaterialPBR(pipeline, albedo, normal, roughness, metallic, ao);
#endif
		return nullptr;
	}

	da::core::CMaterial* CMaterialFactory::CreateCubeMap(CGraphicsPipeline& pipeline, const std::string& albedo)
	{
#ifdef DA_GRAPHICS_VULKAN
		return new da::platform::CVulkanGraphicsMaterialCubeMap(pipeline, albedo);
#endif

		return nullptr;
	}

}