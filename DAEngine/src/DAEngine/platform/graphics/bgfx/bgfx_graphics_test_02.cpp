#include "dapch.h"
#include "bgfx_graphics_test_02.h"

#ifdef DA_GRAPHICS_BGFX

#include <chrono>
#include <bx/math.h>
#include "DAEngine/core/graphics/graphics_smesh.h"
#include "DAEngine/platform/graphics/bgfx/bgfx_graphics_material.h"
#include <imgui.h>
#include <stb_image.h>
#include <bimg/bimg.h>


namespace da::platform {

	static bgfx::VertexLayout ms_layout;
	struct FMSLayout
	{
		inline static void init()
		{
			ms_layout
				.begin()
				.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Color0, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Tangent, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Bitangent, 3, bgfx::AttribType::Float)
				.end();
		};
	};



	void CBgfxGraphicsTest02::Initialize(da::core::CWindow* window)
	{
		m_start = (double)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 1e3f;
		m_window = window;
		FMSLayout::init();
		m_smesh = new da::core::CStaticMesh("assets/viking_room.obj");

		// Create static vertex buffer.
		m_vbh = bgfx::createVertexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(m_smesh->getVertices().data(), m_smesh->getVertices().size()*sizeof(da::core::FVertexBase))
			, ms_layout
			, BGFX_BUFFER_COMPUTE_TYPE_FLOAT
		);

		// Create static index buffer for triangle list rendering.
		m_ibh = bgfx::createIndexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(m_smesh->getIndices().data(), sizeof(uint32_t)*m_smesh->getIndices().size())
			, BGFX_BUFFER_INDEX32
		);

		m_uniform = bgfx::createUniform("m_uniform", bgfx::UniformType::Sampler);
		int width = 1, height = 1, channels = 4;
		stbi_uc* pixels = stbi_load("assets/viking_room.png", (int*)&width, (int*)&height, (int*)&channels, STBI_rgb_alpha);
		channels = 4;
		const bgfx::Memory* mem = bgfx::copy(pixels, width * height * channels * sizeof(char));
		m_texture = bgfx::createTexture2D(width,height, false, 1, bgfx::TextureFormat::Enum::RGBA8, 0, mem);
		
		stbi_image_free(pixels);

#ifdef DA_PLATFORM_WINDOWS
		CBgfxGraphicsMaterial* mat = new CBgfxGraphicsMaterial("shaders/mesh/windows/vs_mesh.sc.dx", "shaders/mesh/windows/fs_mesh.sc.dx");
#elif defined(DA_PLATFORM_MACOSX)
		CBgfxGraphicsMaterial* mat = new CBgfxGraphicsMaterial("shaders/mesh/macosx/vs_mesh.sc.vk", "shaders/mesh/macosx/fs_mesh.sc.vk");
#endif
		mat->initialize();
		m_material = (void*)mat;
	}

	void CBgfxGraphicsTest02::Render()
	{
		double time = (double)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 1e3f - m_start;

		const bx::Vec3 at = { 0.0f, 0.0f,   0.0f };
		const bx::Vec3 eye = { 0.0f, 0.0f, -35.0f };

		uint32_t width = m_window->getWindowData().Width;
		uint32_t height = m_window->getWindowData().Height;

		// Set view and projection matrix for view 0.
		{
			float view[16];
			bx::mtxLookAt(view, eye, at);

			float proj[16];
			bx::mtxProj(proj, 60.f, float(width) / float(height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);
			bgfx::setViewTransform(0, view, proj);

			// Set view 0 default viewport.
			bgfx::setViewRect(0, 0, 0, uint16_t(width), uint16_t(height));
		}

		// This dummy draw call is here to make sure that view 0 is cleared
		// if no other draw calls are submitted to view 0.
		//bgfx::touch(0);

		bgfx::IndexBufferHandle ibh = m_ibh;
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
		bgfx::setTransform(mtx);

		// Set texture
		bgfx::setTexture(0, m_uniform, m_texture);

		// Set vertex and index buffer.
		bgfx::setVertexBuffer(0, m_vbh);
		bgfx::setIndexBuffer(ibh);

		// Set render states.
		bgfx::setState(state);

		// Submit primitive for rendering to view 0.
		bgfx::submit(0, { ((CBgfxGraphicsMaterial*)m_material)->getHandle() });
	}

	void CBgfxGraphicsTest02::Shutdown()
	{
		bgfx::destroy(m_ibh);
		bgfx::destroy(m_vbh);
		bgfx::destroy(m_uniform);
		bgfx::destroy(m_texture);
		((CBgfxGraphicsMaterial*)m_material)->shutdown();
		delete m_material;
	}

}


#endif