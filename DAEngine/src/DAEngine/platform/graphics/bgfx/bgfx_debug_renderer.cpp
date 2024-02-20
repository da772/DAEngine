#include "dapch.h"
#include "bgfx_debug_renderer.h"

#if defined(DA_DEBUG) || defined(DA_RELEASE)

#include <bgfx/bgfx.h>
#include "bgfx_util.h"
#include "bgfx_static_mesh.h"
#include "bgfx_graphics_material.h"
#include "bgfx_line_mesh.h"

namespace da::platform
{
	void CBgfxDebugRenderer::initialize()
	{
		m_frameBuffer = createFrameBuffer();
		m_shader = new CBgfxGraphicsMaterial("shaders/cluster/vs_debug_geometry.sc", "shaders/cluster/fs_debug_geometry.sc");
		m_shader->initialize();
		m_uniform = ::bgfx::createUniform("u_Color", ::bgfx::UniformType::Vec4);

		m_shapes[EDebugShapes::Cube] = new CBgfxStaticMesh("assets/cube.fbx", false);
		m_shapes[EDebugShapes::Sphere] = new CBgfxStaticMesh("assets/sphere.fbx", false);
		m_shapes[EDebugShapes::Plane] = new CBgfxStaticMesh("assets/plane.fbx", false);
		m_shapes[EDebugShapes::Cone] = new CBgfxStaticMesh("assets/cone.fbx", false);
		m_shapes[EDebugShapes::Capsule] = new CBgfxStaticMesh("assets/capsule.fbx", false);
		m_shapes[EDebugShapes::Line] = new CBgfxLineMesh();
	}

	void CBgfxDebugRenderer::renderXRay(::bgfx::ViewId view)
	{
		::bgfx::setViewFrameBuffer(view, m_frameBuffer);
		for (int i = m_toDraw.size()-1; i >= 0; i--) {
			uint64_t state = BGFX_STATE_WRITE_R | BGFX_STATE_WRITE_G | BGFX_STATE_WRITE_B | BGFX_STATE_WRITE_A;
			const std::pair<std::function<bool()>, bool>& kv = m_toDraw[i];

			if (!kv.second) continue;

			if (kv.first()) {
				state |= BGFX_STATE_PT_LINES | BGFX_STATE_LINEAA | BGFX_STATE_BLEND_ALPHA;
			}
			
			::bgfx::setState(state);
			::bgfx::submit(view, { m_shader->getHandle() }, 0, ~BGFX_DISCARD_BINDINGS);
			m_toDraw.erase(m_toDraw.begin() + i);
		}

		CBgfxLineMesh* line = (CBgfxLineMesh*)m_shapes[EDebugShapes::Line];
		uint64_t state = BGFX_STATE_WRITE_R | BGFX_STATE_WRITE_G | BGFX_STATE_WRITE_B | BGFX_STATE_WRITE_A;
		glm::vec3 color(1.f);
		::bgfx::setUniform(m_uniform, glm::value_ptr(color));
		line->setBufferData();

		::bgfx::setState(state);
		::bgfx::submit(view, { m_shader->getHandle() }, 0, ~BGFX_DISCARD_BINDINGS);
	}

	void CBgfxDebugRenderer::render(::bgfx::ViewId view)
	{
		for (int i = m_toDraw.size() - 1; i >= 0; i--) {
			uint64_t state = BGFX_STATE_WRITE_R | BGFX_STATE_WRITE_G | BGFX_STATE_WRITE_B | BGFX_STATE_WRITE_A;
			const std::pair<std::function<bool()>, bool>& kv = m_toDraw[i];

			if (kv.second) continue;

			if (kv.first()) {
				state |= BGFX_STATE_PT_LINES | BGFX_STATE_LINEAA | BGFX_STATE_BLEND_ALPHA;
			}

			state |= BGFX_STATE_DEPTH_TEST_LESS;

			::bgfx::setState(state);
			::bgfx::submit(view, { m_shader->getHandle() }, 0, ~BGFX_DISCARD_BINDINGS);
			m_toDraw.erase(m_toDraw.begin() + i);
		}

	}

	void CBgfxDebugRenderer::shutdown()
	{
		for (const std::pair<EDebugShapes, da::graphics::CStaticMesh*>& kv : m_shapes) {
			delete kv.second;
		}
		m_shapes = {};

		m_shader->shutdown();
		delete m_shader;

		BGFXTRYDESTROY(m_frameBuffer);
		BGFXTRYDESTROY(m_uniform);
	}

	void CBgfxDebugRenderer::drawCube(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame, bool xray)
	{
		m_toDraw.push_back({ [position, rot, scale, color, this, wireFrame] {
			glm::mat4 transform = glm::translate(glm::mat4(1.f), position) * glm::toMat4(rot) * glm::scale(glm::mat4(1.f), scale);

			bgfx::setTransform(glm::value_ptr(transform));

			::bgfx::setUniform(m_uniform, glm::value_ptr(color));
			::bgfx::setVertexBuffer(0, *((::bgfx::VertexBufferHandle*)m_shapes[EDebugShapes::Cube]->getNativeVBIndex(0)));
			::bgfx::setIndexBuffer(*((::bgfx::IndexBufferHandle*)m_shapes[EDebugShapes::Cube]->getNativeIBIndex(0)));
			return wireFrame;
		}, xray });
	}

	void CBgfxDebugRenderer::drawSphere(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame, bool xray)
	{
		m_toDraw.push_back({ [position, rot, scale, color, this, wireFrame] {
			glm::mat4 transform = glm::translate(glm::mat4(1.f), position) * glm::toMat4(rot) * glm::scale(glm::mat4(1.f), scale);

			bgfx::setTransform(glm::value_ptr(transform));

			::bgfx::setUniform(m_uniform, glm::value_ptr(color));
			::bgfx::setVertexBuffer(0, *((::bgfx::VertexBufferHandle*)m_shapes[EDebugShapes::Sphere]->getNativeVBIndex(0)));
			::bgfx::setIndexBuffer(*((::bgfx::IndexBufferHandle*)m_shapes[EDebugShapes::Sphere]->getNativeIBIndex(0)));
			return wireFrame;
			}, xray });
	}

	void CBgfxDebugRenderer::drawCapsule(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame, bool xray)
	{
		m_toDraw.push_back({ [position, rot, scale, color, this, wireFrame] {
			glm::mat4 transform = glm::translate(glm::mat4(1.f), position) * glm::toMat4(rot) * glm::scale(glm::mat4(1.f), scale);

			bgfx::setTransform(glm::value_ptr(transform));

			::bgfx::setUniform(m_uniform, glm::value_ptr(color));
			::bgfx::setVertexBuffer(0, *((::bgfx::VertexBufferHandle*)m_shapes[EDebugShapes::Capsule]->getNativeVBIndex(0)));
			::bgfx::setIndexBuffer(*((::bgfx::IndexBufferHandle*)m_shapes[EDebugShapes::Capsule]->getNativeIBIndex(0)));
			return wireFrame;
			}, xray });
	}

	void CBgfxDebugRenderer::drawCone(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame, bool xray)
	{
		m_toDraw.push_back({ [position, rot, scale, color, this, wireFrame] {
			glm::mat4 transform = glm::translate(glm::mat4(1), position) * glm::toMat4(rot) * glm::scale(glm::mat4(1), scale);

			bgfx::setTransform(glm::value_ptr(transform));

			::bgfx::setUniform(m_uniform, glm::value_ptr(color));
			::bgfx::setVertexBuffer(0, *((::bgfx::VertexBufferHandle*)m_shapes[EDebugShapes::Cone]->getNativeVBIndex(0)));
			::bgfx::setIndexBuffer(*((::bgfx::IndexBufferHandle*)m_shapes[EDebugShapes::Cone]->getNativeIBIndex(0)));
			return wireFrame;
			}, xray });
	}

	void CBgfxDebugRenderer::drawPlane(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, const glm::vec4& color, bool wireFrame, bool xray)
	{
		m_toDraw.push_back({ [position, rot, scale, color, this, wireFrame] {
			glm::mat4 transform = glm::translate(glm::mat4(1), position) * glm::toMat4(rot) * glm::scale(glm::mat4(1), scale);

			bgfx::setTransform(glm::value_ptr(transform));

			::bgfx::setUniform(m_uniform, glm::value_ptr(color));
			::bgfx::setVertexBuffer(0, *((::bgfx::VertexBufferHandle*)m_shapes[EDebugShapes::Plane]->getNativeVBIndex(0)));
			::bgfx::setIndexBuffer(*((::bgfx::IndexBufferHandle*)m_shapes[EDebugShapes::Plane]->getNativeIBIndex(0)));
			return wireFrame;
			}, xray });
	}

	void CBgfxDebugRenderer::drawLine(const glm::vec3& startPosition, const glm::vec3& endPosition, float width, const glm::vec4& color, bool wireFrame /*= true*/, bool xray /*= true*/)
	{
		CBgfxLineMesh* line = (CBgfxLineMesh*)m_shapes[EDebugShapes::Line];
		line->addTransientLine(startPosition, endPosition, color, width);
	}

	void CBgfxDebugRenderer::drawMesh(const glm::vec3& position, const glm::quat& rot, const glm::vec3& scale, da::graphics::CStaticMesh* mesh, const glm::vec4& color, bool wireFrame /*= true*/, bool xray /*= true*/)
	{
		m_toDraw.push_back({ [position, rot, scale, color, mesh, this, wireFrame] {
			glm::mat4 transform = glm::translate(glm::mat4(1), position) * glm::toMat4(rot) * glm::scale(glm::mat4(1), scale);

			bgfx::setTransform(glm::value_ptr(transform));

			::bgfx::setUniform(m_uniform, glm::value_ptr(color));
			::bgfx::setVertexBuffer(0, *((::bgfx::VertexBufferHandle*)mesh->getNativeVBIndex(0)));
			::bgfx::setIndexBuffer(*((::bgfx::IndexBufferHandle*)mesh->getNativeIBIndex(0)));
			return wireFrame;
			}, xray });
	}

	::bgfx::FrameBufferHandle CBgfxDebugRenderer::createFrameBuffer()
	{
		bgfx::TextureHandle textures[1];
		uint8_t attachments = 0;

		const uint64_t samplerFlags = BGFX_SAMPLER_MIN_POINT | BGFX_SAMPLER_MAG_POINT | BGFX_SAMPLER_MIP_POINT |
			BGFX_SAMPLER_U_CLAMP | BGFX_SAMPLER_V_CLAMP | BGFX_TEXTURE_RT;

		bgfx::TextureFormat::Enum format = bgfx::TextureFormat::BGRA8;
		assert(bgfx::isTextureValid(0, false, 1, format, samplerFlags));
		textures[attachments++] =
			bgfx::createTexture2D(bgfx::BackbufferRatio::Equal, false, 1, format, samplerFlags);


		bgfx::FrameBufferHandle fb = bgfx::createFrameBuffer(attachments, textures, true);

		return fb;
	}

	void CBgfxDebugRenderer::onReset(size_t width, size_t height)
	{
		if (!::bgfx::isValid(m_frameBuffer)) {
			m_frameBuffer = createFrameBuffer();
		}
	}

	::bgfx::FrameBufferHandle CBgfxDebugRenderer::getFrameBuffer() const
	{
		return m_frameBuffer;
	}

	void CBgfxDebugRenderer::clear()
	{
		CBgfxLineMesh* line = (CBgfxLineMesh*)m_shapes[EDebugShapes::Line];
		line->clearAll();
	}

}

#endif