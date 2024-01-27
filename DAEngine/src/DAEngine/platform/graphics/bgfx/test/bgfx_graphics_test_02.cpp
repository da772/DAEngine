#include "dapch.h"
#include "bgfx_graphics_test_02.h"

#ifdef DA_GRAPHICS_BGFX

#include <chrono>
#include <bx/math.h>
#include "DAEngine/core/graphics/graphics_smesh.h"
#include "DAEngine/platform/graphics/bgfx/bgfx_graphics_material.h"
#include "DAEngine/platform/graphics/bgfx/bgfx_static_mesh.h"
#include <imgui.h>
#include <stb_image.h>
#include <bimg/bimg.h>
#include "../bgfx_static_mesh.h"
#include <core/graphics/camera.h>


namespace da::platform {

	void CBgfxGraphicsTest02::Initialize(da::core::CWindow* window)
	{
		m_start = (double)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 1e3f;
		m_window = window;
		
		m_mesh = new CBgfxStaticMesh("assets/viking_room.obj", false);
		m_texture = bgfx::CBgfxTexture2D("assets/viking_room.png");
		m_uniform = bgfx::CBgfxUniform<uint16_t>(::bgfx::UniformType::Sampler, "m_uniform");//::bgfx::createUniform("m_uniform", ::bgfx::UniformType::Sampler);

		CBgfxGraphicsMaterial* mat = new CBgfxGraphicsMaterial("shaders/mesh/vs_mesh.sc", "shaders/mesh/fs_mesh.sc");
		mat->initialize();
		m_material = (void*)mat;

		da::core::CCamera& cam = *da::core::CCamera::getCamera();

		const glm::vec3 at = { 0.0f, 0.0f,   0.0f };
		const glm::vec3 eye = { 0.0f, 0.0f, -35.0f };

		cam.setPosition(eye);
		cam.lookAt(at);
	}

	void CBgfxGraphicsTest02::Render()
	{
		double time = (double)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 1e3f - m_start;
		/*
		const bx::Vec3 at = { 0.0f, 0.0f,   0.0f };
		const bx::Vec3 eye = { 0.0f, 0.0f, -35.0f };

		uint32_t width = m_window->getWindowData().Width;
		uint32_t height = m_window->getWindowData().Height;

		// Set view and projection matrix for view 0.
		{
			da::core::CCamera& cam = *da::core::CCamera::getCamera();
			glm::mat4 view = cam.matrix();

			float proj[16];
			bx::mtxProj(proj, cam.fov, float(width) / float(height), cam.zNear, cam.zFar, ::bgfx::getCaps()->homogeneousDepth, bx::Handedness::Left);
			::bgfx::setViewTransform(0, &view[0], proj);

			// Set view 0 default viewport.
			::bgfx::setViewRect(0, 0, 0, uint16_t(width), uint16_t(height));
		}
		*/
		// This dummy draw call is here to make sure that view 0 is cleared
		// if no other draw calls are submitted to view 0.
		//::bgfx::touch(0);

		::bgfx::IndexBufferHandle ibh = *(::bgfx::IndexBufferHandle*)m_mesh->getNativeIBIndex(0);
		::bgfx::VertexBufferHandle vbh = *(::bgfx::VertexBufferHandle*)m_mesh->getNativeVBIndex(0);
		uint64_t state = BGFX_STATE_WRITE_R
			| BGFX_STATE_WRITE_G
			| BGFX_STATE_WRITE_B
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_CULL_CCW
			| BGFX_STATE_MSAA;

		float mtx[16];
		bx::mtxRotateXY(mtx, time + 0 * 0.21f, time + 0 * 0.37f);
		//bx::mtxRotateXY(mtx, 0,0);

		static float xyz[3] = { 0,0,-25 };

		mtx[12] = xyz[0];
		mtx[13] = xyz[1];
		mtx[14] = xyz[2];

		ImGui::InputFloat3("POS", xyz);


		// Set model matrix for rendering.
		::bgfx::setTransform(mtx);
		// Set vertex and index buffer.
		m_mesh->setBuffers(0, 0);

		// Set texture
		::bgfx::setTexture(0, { m_uniform.getHandle() }, { m_texture.getHandle() });

		// Set render states.
		::bgfx::setState(state);

		// Submit primitive for rendering to view 0.
		::bgfx::submit(0, { ((CBgfxGraphicsMaterial*)m_material)->getHandle() });
	}

	void CBgfxGraphicsTest02::Shutdown()
	{
		delete m_mesh;
		m_uniform.destroy();
		m_texture.destroy();
		((CBgfxGraphicsMaterial*)m_material)->shutdown();
		delete m_material;
	}

}


#endif