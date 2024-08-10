#pragma once
#if defined(DA_DEBUG) || defined(DA_RELEASE)
#include "daglm.h"
#include "graphics\graphics_smesh.h"

namespace da::debug
{

	class IDebugRenderer
	{
	public:
		virtual void drawCube(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame = true, bool xray = true) = 0;
		virtual void drawSphere(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame = true, bool xray = true) = 0;
		virtual void drawCapsule(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame = true, bool xray = true) = 0;
		virtual void drawCone(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame = true, bool xray = true) = 0;
		virtual void drawPlane(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame = true, bool xray = true) = 0;
		virtual void drawMesh(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, da::graphics::CStaticMesh* mesh, const glm::vec4& color, bool wireFrame = true, bool xray = true) = 0;
		virtual void drawLine(const glm::vec3& startPosition, const glm::vec3& endPosition, float width, const glm::vec4& color, bool wireFrame = true, bool xray = true) = 0;

		virtual void clear() = 0;
	};

	class CDebugRender {
	public:
		static void drawCube(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame = true, bool xray = true);
		static void drawSphere(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame = true, bool xray = true);
		static void drawCapsule(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame = true, bool xray = true);
		static void drawCone(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame = true, bool xray = true);
		static void drawPlane(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame = true, bool xray = true);
		static void drawMesh(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, da::graphics::CStaticMesh* mesh, const glm::vec4& color, bool wireFrame = true, bool xray = true);
		static void drawLine(const glm::vec3& startPosition, const glm::vec3& endPosition, float width, const glm::vec4& color, bool wireFrame = true, bool xray = true);

		static void clear();
		static void setInstance(IDebugRenderer* renderer);
	private:
		static IDebugRenderer* s_renderer;
	};

}
#endif