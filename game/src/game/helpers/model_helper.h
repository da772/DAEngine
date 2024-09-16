#pragma once
#include "game/generated/model.generated.h"
#include <DAEngine/graphics.h>

class CModelHelper
{
public:
	static da::graphics::CStaticMesh* create(Model model);
};