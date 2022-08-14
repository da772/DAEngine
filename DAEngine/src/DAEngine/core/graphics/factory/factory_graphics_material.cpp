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
		, const CBasicString <da::memory::CGraphicsAllocator>& albedo
		, const CBasicString <da::memory::CGraphicsAllocator>& normal
		, const CBasicString <da::memory::CGraphicsAllocator>& roughness
		, const CBasicString <da::memory::CGraphicsAllocator>& metallic
		, const CBasicString <da::memory::CGraphicsAllocator>& ao
	)
	{
#ifdef DA_GRAPHICS_VULKAN
		return new da::platform::CVulkanGraphicsMaterialPBR(pipeline, albedo, normal, roughness, metallic, ao);
#endif
		return nullptr;
	}

	da::core::CMaterial* CMaterialFactory::CreateCubeMap(CGraphicsPipeline& pipeline, const CBasicString <da::memory::CGraphicsAllocator>& albedo)
	{
#ifdef DA_GRAPHICS_VULKAN
		return new da::platform::CVulkanGraphicsMaterialCubeMap(pipeline, albedo);
#endif

		return nullptr;
	}

}