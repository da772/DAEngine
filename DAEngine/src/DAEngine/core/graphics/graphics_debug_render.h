#pragma once
#if defined(DA_DEBUG) || defined(DA_RELEASE)
#include "DAEngine\core\graphics\graphics_smesh.h"

namespace da::graphics
{
	class CDebugRender {
	public:
		virtual void drawCube(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame = true, bool xray = true) = 0;
		virtual void drawSphere(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame = true, bool xray = true) = 0;
		virtual void drawCapsule(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame = true, bool xray = true) = 0;
		virtual void drawCone(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame = true, bool xray = true) = 0;
		virtual void drawPlane(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame = true, bool xray = true) = 0;
		virtual void drawMesh(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, da::graphics::CStaticMesh* mesh, const glm::vec4& color, bool wireFrame = true, bool xray = true) = 0;
		virtual void drawLine(const glm::vec3& startPosition, const glm::vec3& endPosition, float width, const glm::vec4& color, bool wireFrame = true, bool xray = true) = 0;


		static CDebugRender* getInstance();
		static void setInstance(CDebugRender* renderer);
	private:
		static CDebugRender* s_renderer;
	};
}
#endif