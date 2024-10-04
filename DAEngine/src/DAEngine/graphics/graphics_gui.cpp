

#include "graphics_gui.h"
#include "graphics/camera.h"
#include "stl/maths/transform.h"
#include "graphics/graphics_api.h"

namespace da
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