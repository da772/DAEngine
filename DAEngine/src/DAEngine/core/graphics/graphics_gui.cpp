#include "dapch.h"

#include "graphics_gui.h"
#include "DAEngine/core/graphics/camera.h"
#include "daengine/core/maths/transform.h"
#include "DAEngine/core/graphics/graphics_api.h"

namespace da::graphics
{

	CGui::CGui(const CGraphicsApi& graphics) : m_graphics(graphics)
	{

	}

	void CGui::render(float dt)
	{
		onRender(dt);
	}

	glm::vec2 CGui::worldPosToScreenSpace(const glm::vec3& pos) const
	{
		return m_graphics.worldPosToScreenSpace(pos);
	}

}