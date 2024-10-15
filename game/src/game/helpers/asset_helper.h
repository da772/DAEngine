#pragma once
#include "game/generated/model.generated.h"
#include "game/generated/texture.generated.h"
#include "game/generated/skeleton.generated.h"
#include "game/generated/material.generated.h"
#include <DAEngine/graphics.h>

class CAssetHelper
{
public:
	static da::CStaticMesh* create(Model model);
	static da::CGraphicsTexture2D* create(Texture texture);
	static da::CSkeletalMesh* create(Skeleton texture);
	static void set_material(Material material, da::CStaticMesh* mesh);
	static void set_material(Material material, da::CSkeletalMesh* mesh);

private:
	static void set_material_internal(Material materialEnum, std::function<da::FMaterialData*(uint32_t)> getMaterial);

};