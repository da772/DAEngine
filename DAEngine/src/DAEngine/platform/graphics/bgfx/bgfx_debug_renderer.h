#pragma once

#if defined(DA_DEBUG) || defined(DA_RELEASE)

#include <bgfx/bgfx.h>
#include "debug/graphics_debug_render.h"
#include <graphics/graphics_vertex.h>


namespace da::graphics
{
	class CStaticMesh;
	class CMaterial;
}

namespace da::platform
{
	class CBgfxGraphicsMaterial;

	enum class EDebugShapes
	{
		Cube,
		Sphere,
		Capsule,
		Cone,
		Plane,
		Line
	};

	class CBgfxDebugRenderer : public da::debug::IDebugRenderer
	{
	public:
		virtual void drawCube(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame = true, bool xray = true) override;
		virtual void drawSphere(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame = true, bool xray = true) override;
		virtual void drawCapsule(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame = true, bool xray = true) override;
		virtual void drawCone(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame = true, bool xray = true) override;
		virtual void drawPlane(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame = true, bool xray = true) override;
		virtual void drawMesh(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, da::graphics::CStaticMesh* mesh, const glm::vec4& color, bool wireFrame = true, bool xray = true) override;
		virtual void drawLine(const glm::vec3& startPosition, const glm::vec3& endPosition, float width, const glm::vec4& color, bool wireFrame = true, bool xray = true) override;

		void initialize();
		void renderXRay(::bgfx::ViewId view);
		void render(::bgfx::ViewId view);
		void shutdown();

		void onReset(size_t width, size_t height);
		::bgfx::FrameBufferHandle getFrameBuffer() const;


		void clear() override;

	private:
		::bgfx::FrameBufferHandle createFrameBuffer();
		

	private:
		::bgfx::FrameBufferHandle m_frameBuffer;
		::bgfx::UniformHandle m_uniform;
		da::graphics::CMaterial* m_shader;
		std::unordered_map<EDebugShapes, da::graphics::CStaticMesh*> m_shapes;
		std::vector<std::pair<std::function<bool()>, bool>> m_toDraw;
	};



}

#endif