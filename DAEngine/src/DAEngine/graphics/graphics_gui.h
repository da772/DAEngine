#pragma once
#include "daglm.h"

namespace da::core
{
	class CCamera;
}

namespace da::graphics
{
	class CGraphicsApi;
	

	class CGui
	{
	public:
		CGui(const CGraphicsApi& graphics);
		void render(float dt);

	protected:
		glm::vec2 worldPosToScreenSpace(const glm::vec3& pos) const;
		virtual void onRender(float dt) = 0;

	private:
		const CGraphicsApi& m_graphics;
	};
}