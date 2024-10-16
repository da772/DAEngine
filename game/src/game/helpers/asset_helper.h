#pragma once
#include "game/generated/model.generated.h"
#include "game/generated/texture.generated.h"
#include "game/generated/skeleton.generated.h"
#include "game/generated/material.generated.h"
#include "game/generated/animation.generated.h"
#include <DAEngine/graphics.h>

class CAssetHelper
{
public:
	static da::CStaticMesh* create(EModel model);
	static da::CGraphicsTexture2D* create(ETexture texture);
	static da::CSkeletalMesh* create(ESkeleton skeleton);
	static da::CSkeletalAnimation* create(EAnimation animation, da::CSkeletalMesh* skeleton);
	static da::CSkeletalAnimator* create_animator(EAnimation animation, da::CSkeletalMesh* skeleton);
	static void set_material(EMaterial material, da::CStaticMesh* mesh);
	static void set_material(EMaterial material, da::CSkeletalMesh* mesh);

private:
	static void set_material_internal(EMaterial materialEnum, std::function<da::FMaterialData*(uint32_t)> getMaterial);

};