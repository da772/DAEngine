
#include "graphics_debug_render.h"
#if defined(DA_DEBUG) || defined(DA_RELEASE)
namespace da
{
	da::IDebugRenderer* CDebugRender::s_renderer;

	void CDebugRender::setInstance(IDebugRenderer* renderer)
	{
		s_renderer = renderer;
	}

	void CDebugRender::drawCube(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame /*= true*/, bool xray /*= true*/)
	{
		ASSERT(s_renderer);
		s_renderer->drawCube(position, rot, scale, color, wireFrame, xray);
	}

	void CDebugRender::drawSphere(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame /*= true*/, bool xray /*= true*/)
	{
		ASSERT(s_renderer);
		s_renderer->drawSphere(position, rot, scale, color, wireFrame, xray);
	}

	void CDebugRender::drawCapsule(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame /*= true*/, bool xray /*= true*/)
	{
		ASSERT(s_renderer);
		s_renderer->drawCapsule(position, rot, scale, color, wireFrame, xray);
	}

	void CDebugRender::drawCone(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame /*= true*/, bool xray /*= true*/)
	{
		ASSERT(s_renderer);
		s_renderer->drawCone(position, rot, scale, color, wireFrame, xray);
	}

	void CDebugRender::drawPlane(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame /*= true*/, bool xray /*= true*/)
	{
		ASSERT(s_renderer);
		s_renderer->drawPlane(position, rot, scale, color, wireFrame, xray);
	}

	void CDebugRender::drawMesh(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, da::CStaticMesh* mesh, const glm::vec4& color, bool wireFrame /*= true*/, bool xray /*= true*/)
	{
		ASSERT(s_renderer);
		s_renderer->drawMesh(position, rot, scale, mesh, color, wireFrame, xray);
	}

	void CDebugRender::drawLine(const glm::vec3& startPosition, const glm::vec3& endPosition, float width, const glm::vec4& color, bool wireFrame /*= true*/, bool xray /*= true*/)
	{
		ASSERT(s_renderer);
		s_renderer->drawLine(startPosition, endPosition, width, color, wireFrame, xray);
	}

	void CDebugRender::clear()
	{
		ASSERT(s_renderer);
		s_renderer->clear();
	}

}
#endif