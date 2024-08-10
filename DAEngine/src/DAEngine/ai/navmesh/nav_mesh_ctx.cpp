#include "nav_mesh_ctx.h"
#include <debug/graphics_debug_render.h>

namespace da::ai
{
	da::ai::CNavCtx CNavCtx::ms_ctx;
	da::ai::CNavDrawer CNavDrawer::ms_drawer;

	void CNavCtx::doLog(const rcLogCategory category, const char* msg, const int len)
	{
		switch (category)
		{
		case RC_LOG_PROGRESS:
			LOG_DEBUG(ELogChannel::AI, "%s", msg);
			break;
		case RC_LOG_WARNING:
			LOG_WARN(ELogChannel::AI, "%s", msg);
			break;
		case RC_LOG_ERROR:
			LOG_ERROR(ELogChannel::AI, "%s", msg);
			break;

		}
	}

#ifdef DA_REVIEW
	void CNavDrawer::depthMask(bool state)
	{

	}


	void CNavDrawer::texture(bool state)
	{

	}

	void CNavDrawer::begin(duDebugDrawPrimitives prim, float size)
	{
		m_type = prim;
	}

	void CNavDrawer::vertex(const float* pos, unsigned int color)
	{
		float r = (color >> 24) & 0xFF;
		float g = (color >> 16) & 0xFF;
		float b = (color >> 8) & 0xFF;
		float a = color & 0xFF;
		m_color = { r / 255.f, g / 255.f, b / 255.f, 0.f };
		m_vertices.push_back({ pos[0], pos[2], pos[1] });
	}


	void CNavDrawer::vertex(const float x, const float y, const float z, unsigned int color)
	{
		float r = (color >> 24) & 0xFF;
		float g = (color >> 16) & 0xFF;
		float b = (color >> 8) & 0xFF;
		float a = color & 0xFF;
		m_color = { r / 255.f, g / 255.f, b / 255.f, 0.f };
		m_vertices.push_back({ x, z, y });
	}


	void CNavDrawer::vertex(const float* pos, unsigned int color, const float* uv)
	{
		float r = (color >> 24) & 0xFF;
		float g = (color >> 16) & 0xFF;
		float b = (color >> 8) & 0xFF;
		float a = color & 0xFF;
		m_color = { r / 255.f, g / 255.f, b / 255.f, 0.f };
		m_vertices.push_back({ pos[0], pos[2], pos[1] });
	}


	void CNavDrawer::vertex(const float x, const float y, const float z, unsigned int color, const float u, const float v)
	{
		float r = (color >> 24) & 0xFF;
		float g = (color >> 16) & 0xFF;
		float b = (color >> 8) & 0xFF;
		float a = color & 0xFF;
		m_color = { r / 255.f, g / 255.f, b / 255.f, 0.f };
		m_vertices.push_back({ x, z, y });
	}


	void CNavDrawer::end()
	{
		if (m_vertices.size() == 0) {
			return;
		}

		for (int i = 0; i < m_vertices.size() - 1; i++) {
			da::debug::CDebugRender::drawLine(m_vertices[i], m_vertices[i + 1], .01f, m_color, false);
		}

		m_vertices = {};
	}
#endif
}