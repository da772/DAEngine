#pragma once
#include "game/generated/model.generated.h"
#include "game/generated/texture.generated.h"
#include <DAEngine/graphics.h>

class CAssetHelper
{
public:
	static da::CStaticMesh* create(Model model);
	static da::CGraphicsTexture2D* create(Texture texture);
};