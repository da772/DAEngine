#include "dapch.h"

#include "bgfx_graphics_test_01.h"

#ifdef DA_GRAPHICS_BGFX

#include <bgfx/bgfx.h>
#include <bx/bx.h>
#include <bx/timer.h>
#include <bx/math.h>
#include <chrono>

#include "DAEngine/core/window/window.h"
#include "DAEngine/asset/asset.h"
#include "DAEngine/platform/graphics/bgfx/bgfx_graphics_material.h"
#include <core/graphics/camera.h>



static const char* s_meshPaths[] =
{
	"assets/cube.bin",
	"assets/orb.bin",
	"assets/column.bin",
	"assets/bunny.bin",
	"assets/tree.bin",
	"assets/hollowcube.bin"
};

struct PosColorVertex
{
	float m_x;
	float m_y;
	float m_z;
	uint32_t m_abgr;

	static void init()
	{
		ms_layout
			.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Color0, 4, bgfx::AttribType::Uint8, true)
			.end();
	};

	static bgfx::VertexLayout ms_layout;
};

bgfx::VertexLayout PosColorVertex::ms_layout;

static PosColorVertex s_cubeVertices[] =
{
	{-1.0f,  1.0f,  1.0f, 0xff000000 },
	{ 1.0f,  1.0f,  1.0f, 0xff0000ff },
	{-1.0f, -1.0f,  1.0f, 0xff00ff00 },
	{ 1.0f, -1.0f,  1.0f, 0xff00ffff },
	{-1.0f,  1.0f, -1.0f, 0xffff0000 },
	{ 1.0f,  1.0f, -1.0f, 0xffff00ff },
	{-1.0f, -1.0f, -1.0f, 0xffffff00 },
	{ 1.0f, -1.0f, -1.0f, 0xffffffff },
};

static const uint16_t s_cubeTriList[] =
{
	0, 1, 2, // 0
	1, 3, 2,
	4, 6, 5, // 2
	5, 6, 7,
	0, 2, 4, // 4
	4, 2, 6,
	1, 5, 3, // 6
	5, 7, 3,
	0, 4, 1, // 8
	4, 5, 1,
	2, 3, 6, // 10
	6, 3, 7,
};

static const uint16_t s_cubeTriStrip[] =
{
	0, 1, 2,
	3,
	7,
	1,
	5,
	0,
	4,
	2,
	6,
	7,
	4,
	5,
};

static const uint16_t s_cubeLineList[] =
{
	0, 1,
	0, 2,
	0, 4,
	1, 3,
	1, 5,
	2, 3,
	2, 6,
	3, 7,
	4, 5,
	4, 6,
	5, 7,
	6, 7,
};

static const uint16_t s_cubeLineStrip[] =
{
	0, 2, 3, 1, 5, 7, 6, 4,
	0, 2, 6, 4, 5, 7, 3, 1,
	0,
};

static const uint16_t s_cubePoints[] =
{
	0, 1, 2, 3, 4, 5, 6, 7
};


namespace da::platform
{

	double start = 0;
	void CBgfxGraphicsTest01::Initialize(da::core::CWindow* window)
	{
        start = (double)std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch()).count()/1e3f;
		m_window = window;
		
		// Create vertex stream declaration.
		PosColorVertex::init();

		// Create static vertex buffer.
		m_vbh = bgfx::createVertexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(s_cubeVertices, sizeof(s_cubeVertices))
			, PosColorVertex::ms_layout
		);

		// Create static index buffer for triangle list rendering.
		m_ibh = bgfx::createIndexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(s_cubeTriList, sizeof(s_cubeTriList))
		);

		// Create program from shaders.
		CBgfxGraphicsMaterial* mat = new CBgfxGraphicsMaterial("shaders/cubes/vs_cubes.sc", "shaders/cubes/fs_cubes.sc");

		mat->initialize();
		m_material = (void*)mat;

		da::core::CCamera& cam = *da::core::CCamera::getCamera();

		const glm::vec3 at = { 0.0f, 0.0f,   0.0f };
		const glm::vec3 eye = { 0.0f, 0.0f, -35.0f };
		cam.setPosition(eye);
		cam.lookAt(at);
	}

	void CBgfxGraphicsTest01::Render()
	{
        double time = (double)std::chrono::duration_cast<std::chrono::milliseconds >(std::chrono::system_clock::now().time_since_epoch()).count()/1e3f - start;

		

		uint32_t width = m_window->getWindowData().Width;
		uint32_t height = m_window->getWindowData().Height;

		// Set view and projection matrix for view 0.
		{
			da::core::CCamera& cam = *da::core::CCamera::getCamera();
			glm::mat4 view = cam.matrix();


			float proj[16];
			bx::mtxProj(proj, cam.fov, float(width) / float(height), cam.zNear, cam.zFar, bgfx::getCaps()->homogeneousDepth, bx::Handedness::Left);
			bgfx::setViewTransform(0, &view[0], proj);

			// Set view 0 default viewport.
			bgfx::setViewRect(0, 0, 0, uint16_t(width), uint16_t(height));
		}

		// This dummy draw call is here to make sure that view 0 is cleared
		// if no other draw calls are submitted to view 0.
		//bgfx::touch(0);

		bgfx::IndexBufferHandle ibh = m_ibh;
		uint64_t state = 0
			| (m_r ? BGFX_STATE_WRITE_R : 0)
			| (m_g ? BGFX_STATE_WRITE_G : 0)
			| (m_b ? BGFX_STATE_WRITE_B : 0)
			| (m_a ? BGFX_STATE_WRITE_A : 0)
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_CULL_CW
			| BGFX_STATE_MSAA
			| 0
			;

		// Submit 11x11 cubes.
		for (uint32_t yy = 0; yy < 11; ++yy)
		{
			for (uint32_t xx = 0; xx < 11; ++xx)
			{
				float mtx[16];
				bx::mtxRotateXY(mtx, time + xx * 0.21f, time + yy * 0.37f);
				mtx[12] = -15.0f + float(xx) * 3.0f;
				mtx[13] = -15.0f + float(yy) * 3.0f;
				mtx[14] = 0.0f;

				// Set model matrix for rendering.
				bgfx::setTransform(mtx);

				// Set vertex and index buffer.
				bgfx::setVertexBuffer(0, m_vbh);
				bgfx::setIndexBuffer(ibh);

				// Set render states.
				bgfx::setState(state);

				// Submit primitive for rendering to view 0.
				bgfx::submit(0, { ((CBgfxGraphicsMaterial*)m_material)->getHandle() });
			}
		}
        
        m_timeOffset = bx::getHPCounter();
	}

	void CBgfxGraphicsTest01::Shutdown()
	{
		bgfx::destroy(m_ibh);
		bgfx::destroy(m_vbh);
		((CBgfxGraphicsMaterial*)m_material)->shutdown();
		delete m_material;
	}

}


#endif
