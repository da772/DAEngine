#include "dapch.h"
#include "bgfx_graphics_test_03.h"

#ifdef DA_GRAPHICS_BGFX

#include <chrono>
#include <bx/math.h>
#include "DAEngine/core/graphics/graphics_smesh.h"
#include "DAEngine/platform/graphics/bgfx/bgfx_graphics_material.h"
#include "DAEngine/platform/graphics/bgfx/pipeline/bgfx_pipeline_gbuffer.h"
#include "DAEngine/platform/graphics/bgfx/pipeline/bgfx_pipeline_shadow.h"
#include "DAEngine/platform/graphics/bgfx/pipeline/bgfx_pipeline_light.h"
#include <imgui.h>
#include <stb_image.h>
#include <bimg/bimg.h>
#include "DAEngine/core/graphics/camera.h"

// Render passes
#define RENDER_PASS_COMBINE      3  // Directional light and final result


// Shadow map has multiple render targets
#define SHADOW_RT_RSM   0        // In this algorithm, shadows write lighting info as well.
#define SHADOW_RT_DEPTH 1        // Shadow maps always write a depth

// Random meshes we draw
#define MODEL_COUNT 222  // In this demo, a model is a mesh plus a transform and a color

#define SHADOW_MAP_DIM 512
#define LIGHT_DIST 50.0f

namespace da::platform {
	
	namespace impl {
	struct PosTexCoord0Vertex
	{
		float m_x;
		float m_y;
		float m_z;
		float m_u;
		float m_v;

		static void init()
		{
			ms_layout
				.begin()
				.add(bgfx::Attrib::Position,  3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
				.end();
		}

		static bgfx::VertexLayout ms_layout;
	};
	bgfx::VertexLayout PosTexCoord0Vertex::ms_layout;

	}

	using namespace da::platform::impl;
    void CBgfxGraphicsTest03::updateLightDir()
	{
		float el = m_lightElevation * (bx::kPi/180.0f);
		float az = m_lightAzimuth   * (bx::kPi/180.0f);
		m_lightDir[0] = bx::cos(el)*bx::cos(az);
		m_lightDir[2] = bx::cos(el)*bx::sin(az);
		m_lightDir[1] = bx::sin(el);
		m_lightDir[3] = 0.0f;
	}

	void CBgfxGraphicsTest03::Initialize(da::core::CWindow* window)
	{
		m_start = (double)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 1e3f;
		m_cam = new da::core::CCamera();
		m_lightCam = new da::core::CCamera();
		PosTexCoord0Vertex::init();
        const uint64_t tsFlags = 0
			| BGFX_TEXTURE_RT
			| BGFX_SAMPLER_MIN_POINT
			| BGFX_SAMPLER_MAG_POINT
			| BGFX_SAMPLER_MIP_POINT
			| BGFX_SAMPLER_U_CLAMP
			| BGFX_SAMPLER_V_CLAMP
			;

		// Labeling for renderdoc captures, etc
		bgfx::setViewName(RENDER_PASS_COMBINE,      "post combine");

		// Create uniforms
		u_tint          = bgfx::createUniform("u_tint",          bgfx::UniformType::Vec4);  // Tint for when you click on items
		u_lightDir      = bgfx::createUniform("u_lightDir",      bgfx::UniformType::Vec4);  // Single directional light for entire scene
		u_sphereInfo    = bgfx::createUniform("u_sphereInfo",    bgfx::UniformType::Vec4);  // Info for RSM
		u_invMvp        = bgfx::createUniform("u_invMvp",        bgfx::UniformType::Mat4);  // Matrix needed in light buffer
		u_invMvpShadow  = bgfx::createUniform("u_invMvpShadow",  bgfx::UniformType::Mat4);  // Matrix needed in light buffer
		u_lightMtx      = bgfx::createUniform("u_lightMtx",      bgfx::UniformType::Mat4);  // Matrix needed to use shadow map (world to shadow space)
		u_shadowDimsInv = bgfx::createUniform("u_shadowDimsInv", bgfx::UniformType::Vec4);  // Used in PCF
		u_rsmAmount     = bgfx::createUniform("u_rsmAmount",     bgfx::UniformType::Vec4);  // How much RSM to use vs directional light

		// Create texture sampler uniforms (used when we bind textures)
		s_normal    = bgfx::createUniform("s_normal",    bgfx::UniformType::Sampler);  // Normal gbuffer
		s_depth     = bgfx::createUniform("s_depth",     bgfx::UniformType::Sampler);  // Normal gbuffer
		s_color     = bgfx::createUniform("s_color",     bgfx::UniformType::Sampler);  // Color (albedo) gbuffer
		s_light     = bgfx::createUniform("s_light",     bgfx::UniformType::Sampler);  // Light buffer
		s_shadowMap = bgfx::createUniform("s_shadowMap", bgfx::UniformType::Sampler);  // Shadow map
		s_rsm       = bgfx::createUniform("s_rsm",       bgfx::UniformType::Sampler);  // Reflective shadow map
		s_texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);	   // Texture color
		s_texNormal = bgfx::createUniform("s_texNormal", bgfx::UniformType::Sampler);	   // Normal color

		// Create program from shaders.
		m_gbufferPipeline = new CBgfxPipelineGBuffer();
		m_shadowPipline = new CBgfxPipelineShadow();
		m_lightPipeline = new CBgfxPipelineLight();

		m_combineProgram = new CBgfxGraphicsMaterial("shaders/rsm/vs_rsm_combine.sc", "shaders/rsm/fs_rsm_combine.sc");  // Combiner

		m_gbufferPipeline->initialize();
		m_shadowPipline->initialize();
		m_lightPipeline->initialize();
		m_combineProgram->initialize();

		m_window = window;
		m_smesh = new da::core::CStaticMesh("assets/bolt.fbx");
		m_sphereMesh = new da::core::CStaticMesh("assets/sphere.obj");
		m_cubeMesh = new da::core::CStaticMesh("assets/cube.obj");

       

		{
			int width = 1, height = 1, channels = 4;
			stbi_uc* pixels = stbi_load("assets/boltA.jpg", (int*)&width, (int*)&height, (int*)&channels, STBI_rgb_alpha);
			channels = 4;
			const bgfx::Memory* mem = bgfx::copy(pixels, width * height * channels * sizeof(char));
			m_colorTex = bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::Enum::RGBA8, 0, mem);
		}

		{
			int width = 1, height = 1, channels = 4;
			stbi_uc* pixels = stbi_load("assets/boltN.png", (int*)&width, (int*)&height, (int*)&channels, STBI_rgb_alpha);
			channels = 4;
			const bgfx::Memory* mem = bgfx::copy(pixels, width * height * channels * sizeof(char));
			m_normalTex = bgfx::createTexture2D(width, height, false, 1, bgfx::TextureFormat::Enum::RGBA8, 0, mem);
		}

        // Make shadow buffer
		const uint64_t rsmFlags = 0
			| BGFX_TEXTURE_RT
			| BGFX_SAMPLER_MIN_POINT
			| BGFX_SAMPLER_MAG_POINT
			| BGFX_SAMPLER_MIP_POINT
			| BGFX_SAMPLER_U_CLAMP
			| BGFX_SAMPLER_V_CLAMP
			;

		m_caps = bgfx::getCaps();
        updateLightDir();

		m_ms_layout
				.begin()
				.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Color0, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Tangent, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Bitangent, 3, bgfx::AttribType::Float)
				.end();

					// Create static vertex buffer.
		m_vbh = bgfx::createVertexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(m_smesh->getVertices().data(), m_smesh->getVertices().size()*sizeof(da::core::FVertexBase))
			, m_ms_layout
			, BGFX_BUFFER_COMPUTE_TYPE_FLOAT
		);

		m_spvbh = bgfx::createVertexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(m_sphereMesh->getVertices().data(), m_sphereMesh->getVertices().size() * sizeof(da::core::FVertexBase))
			, m_ms_layout
			, BGFX_BUFFER_COMPUTE_TYPE_FLOAT
		);

		m_cbcvh = bgfx::createVertexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(m_cubeMesh->getVertices().data(), m_cubeMesh->getVertices().size() * sizeof(da::core::FVertexBase))
			, m_ms_layout
			, BGFX_BUFFER_COMPUTE_TYPE_FLOAT
		);

		// Create static index buffer for triangle list rendering.
		m_ibh = bgfx::createIndexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(m_smesh->getIndices().data(), sizeof(uint32_t)*m_smesh->getIndices().size())
			, BGFX_BUFFER_INDEX32
		);

		// Create static index buffer for triangle list rendering.
		m_spibh = bgfx::createIndexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(m_sphereMesh->getIndices().data(), sizeof(uint32_t) * m_sphereMesh->getIndices().size())
			, BGFX_BUFFER_INDEX32
		);

		// Create static index buffer for triangle list rendering.
		m_cbibh= bgfx::createIndexBuffer(
			// Static data can be passed with bgfx::makeRef
			bgfx::makeRef(m_cubeMesh->getIndices().data(), sizeof(uint32_t) * m_cubeMesh->getIndices().size())
			, BGFX_BUFFER_INDEX32
		);
	}

	void CBgfxGraphicsTest03::drawModels(int pass, bgfx::ProgramHandle program, Vector3f pos, Vector3f scale, float time, uint64_t state, bool submit)
	{
		drawModels(pass, program, m_vbh, m_ibh, pos, scale, time, state, submit);
	}

	void CBgfxGraphicsTest03::drawModels(int pass, bgfx::ProgramHandle program, bgfx::VertexBufferHandle vbh, bgfx::IndexBufferHandle ibh, Vector3f pos /*= { 0.f,0.f,0.f }*/, Vector3f scale, float time /*= 10.8f*/, uint64_t state /*= 0*/, bool submit)
	{
		state = state == 0 ? BGFX_STATE_WRITE_R
			| BGFX_STATE_WRITE_G
			| BGFX_STATE_WRITE_B
			| BGFX_STATE_WRITE_A
			| BGFX_STATE_WRITE_Z
			| BGFX_STATE_DEPTH_TEST_LESS
			| BGFX_STATE_CULL_CCW
			| BGFX_STATE_MSAA : state;

		float mtx[16];
		//bx::mtxRotateXY(mtx, 10.8f, 10.8f);
		bx::mtxRotateXY(mtx, time + 0 * 0.21f, time + 0 * 0.37f);

		float mtxScale[16];
		bx::mtxScale(mtxScale
			, scale.x
			, scale.y
			, scale.z
		);
		float mtxTrans[16];
		bx::mtxTranslate(mtxTrans
			, 0.0f
			, -10.0f
			, 0.0f
		);

		mtx[12] = pos.x;
		mtx[13] = pos.y;
		mtx[14] = pos.z;

		float mtxRes[16];
		bx::mtxMul(mtxRes, mtxScale, mtx);

		float v4[] = { 1.f,1.f,1.f,1.f };
		bgfx::setUniform(u_tint, v4);

		// Set model matrix for rendering.
		bgfx::setTransform(mtxRes);

		// Set render states.
		bgfx::setState(state);

		// Set vertex and index buffer.
		bgfx::setIndexBuffer(ibh);
		bgfx::setVertexBuffer(0, vbh);
		if (submit)
			bgfx::submit(pass, program, 0);
	}

	

	void CBgfxGraphicsTest03::Render()
	{
		double time = (double)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 1e3f - m_start;

		const bx::Vec3 at = {0.f,0.f,0.f};
		const bx::Vec3 eye = { m_camPos.x, m_camPos.y, -35.0f };

		uint32_t width = m_window->getWindowData().Width;
		uint32_t height = m_window->getWindowData().Height;

		// Set view and projection matrix for view 0.
		
		const float* view = m_cam->getMatrix()->Mtx;
		//bx::mtxLookAt(view, eye, at);

		float proj[16];
		bx::mtxProj(proj, 60.f, float(width) / float(height), 0.1f, 100000.0f, bgfx::getCaps()->homogeneousDepth);

		bgfx::setViewRect(m_gbufferPipeline->renderId(), 0, 0, uint16_t(width), uint16_t(height));
		bgfx::setViewTransform(m_gbufferPipeline->renderId(), view, proj);
		// Make sure when we draw it goes into gbuffer and not backbuffer
		bgfx::setViewFrameBuffer(m_gbufferPipeline->renderId(), m_gbufferPipeline->getFrameBufferHandle());

		// This dummy draw call is here to make sure that view 0 is cleared
		// if no other draw calls are submitted to view 0.
		//bgfx::touch(0);
		//drawModels(RENDER_PASS_GBUFFER, {m_gbufferProgram->getHandle()});
		bgfx::setTexture(0, s_texColor, m_colorTex);
		bgfx::setTexture(1, s_texNormal, m_normalTex);
		drawModels(m_gbufferPipeline->renderId(), { m_gbufferPipeline->getMaterial()->getHandle() }, { 0.f, 0.f, -25.f });
		bgfx::setTexture(0, s_texColor, m_normalTex);
		bgfx::setTexture(1, s_texNormal, m_normalTex);
		drawModels(m_gbufferPipeline->renderId(), { m_gbufferPipeline->getMaterial()->getHandle() }, { 0.f, -1.f, -20.f }, { 1.f,1.f,1.f }, time);
		bgfx::setTexture(0, s_texColor, m_colorTex);
		bgfx::setTexture(1, s_texNormal, m_normalTex);
		drawModels(m_gbufferPipeline->renderId(), { m_gbufferPipeline->getMaterial()->getHandle() }, m_spvbh, m_spibh, { 0.f, 0.f, -10.f });
		bgfx::setTexture(0, s_texColor, m_colorTex);
		bgfx::setTexture(1, s_texNormal, m_normalTex);
		drawModels(m_gbufferPipeline->renderId(), { m_gbufferPipeline->getMaterial()->getHandle() }, m_cbcvh, m_cbibh, { -25.f, -50.f, -25.f }, { 100.f, 100.f, 1.f }, 0.f);
		bgfx::setTexture(0, s_texColor, m_colorTex);
		bgfx::setTexture(1, s_texNormal, m_normalTex);
		drawModels(m_gbufferPipeline->renderId(), { m_gbufferPipeline->getMaterial()->getHandle() }, m_spvbh, m_spibh, m_lightCam->getPosition());

        // Set up transforms for shadow map
        float smProj[16], lightEye[3], lightAt[3];
		
		const float *smView = m_lightCam->getMatrix()->Mtx;
        lightEye[0] = m_lightDir[0] * LIGHT_DIST;
        lightEye[1] = m_lightDir[1]*LIGHT_DIST;
        lightEye[2] = m_lightDir[2]*LIGHT_DIST;
		

        lightAt[0] = m_lightCam->getPosition().x;
        lightAt[1] = m_lightCam->getPosition().y;
        lightAt[2] = m_lightCam->getPosition().z;

        //bx::mtxLookAt(smView, bx::load<bx::Vec3>(lightEye), bx::load<bx::Vec3>(lightAt) );
        const float area = 20.0f;
        const bgfx::Caps* caps = bgfx::getCaps();
        bx::mtxOrtho(smProj, -area, area, -area, area, -100.0f, 100.0f, 0.0f, caps->homogeneousDepth);
        bgfx::setViewTransform(m_shadowPipline->renderId(), smView, smProj);
        bgfx::setViewFrameBuffer(m_shadowPipline->renderId(), m_shadowPipline->getFrameBufferHandle());
        bgfx::setViewRect(m_shadowPipline->renderId(), 0, 0, SHADOW_MAP_DIM, SHADOW_MAP_DIM);

		drawModels(m_shadowPipline->renderId(), { m_shadowPipline->getMaterial()->getHandle()}, { 0.f, 0.f, -25.f });
		drawModels(m_shadowPipline->renderId(), { m_shadowPipline->getMaterial()->getHandle() }, { 0.f, -1.f, -20.f }, {1.f,1.f,1.f}, time);
		drawModels(m_shadowPipline->renderId(), { m_shadowPipline->getMaterial()->getHandle() }, m_spvbh, m_spibh, { 0.f, 0.f, -10.f});
		drawModels(m_shadowPipline->renderId(), { m_shadowPipline->getMaterial()->getHandle() }, m_cbcvh, m_cbibh, { -50.f, -50.f, -25.f }, { 100.f, 100.f, 1.f }, 0.f);


        // Set up matrices for light buffer
		bgfx::setViewRect(m_lightPipeline->renderId(), 0, 0, uint16_t(width), uint16_t(height));
		bgfx::setViewTransform(m_lightPipeline->renderId(), view, proj);  // Notice, same view and proj as gbuffer
		// Set drawing into light buffer
		bgfx::setViewFrameBuffer(m_lightPipeline->renderId(), m_lightPipeline->getFrameBufferHandle());

			// Inverse view projection is needed in shader so set that up
			float vp[16], invMvp[16];
			bx::mtxMul(vp, view, proj);
			bx::mtxInverse(invMvp, vp);

			// Light matrix used in combine pass and inverse used in light pass
			float lightMtx[16]; // World space to light space (shadow map space)
			bx::mtxMul(lightMtx, smView, smProj);
			float invMvpShadow[16];
			bx::mtxInverse(invMvpShadow, lightMtx);

			// Draw some lights (these should really be instanced but for this example they aren't...)
			const uint32_t kMaxSpheres = 16;
			for (uint32_t i = 0; i < kMaxSpheres; i++)
			{
				for (uint32_t j = 0; j < kMaxSpheres; j++)
				{
					// These are used in the fragment shader
					bgfx::setTexture(0, s_normal,m_gbufferPipeline->getNormalTexture());  // Normal for lighting calculations
					bgfx::setTexture(1, s_depth, m_gbufferPipeline->getDepthTexture());   // Depth to reconstruct world position

					// Thse are used in the vert shader
					bgfx::setTexture(2, s_shadowMap, m_shadowPipline->getDepthTexture());  // Used to place sphere
					bgfx::setTexture(3, s_rsm,	     m_shadowPipline->getRSMTexture());    // Used to scale/color sphere

					bgfx::setUniform(u_invMvp, invMvp);
					bgfx::setUniform(u_invMvpShadow, invMvpShadow);
					float sphereInfo[4];
					sphereInfo[0] = ((float)i/(kMaxSpheres-1));
					sphereInfo[1] = ((float)j/(kMaxSpheres-1));
					sphereInfo[2] = m_vplRadius;
					sphereInfo[3] = 0.0;  // Unused
					bgfx::setUniform(u_sphereInfo, sphereInfo);

					const uint64_t lightDrawState = 0
						| BGFX_STATE_WRITE_RGB
						| BGFX_STATE_BLEND_ADD   // <===  Overlapping lights contribute more
						| BGFX_STATE_WRITE_A
						| BGFX_STATE_CULL_CW     // <===  If we go into the lights, there will be problems, so we draw the far back face.
						;
					bgfx::touch(0);
					//drawModels(RENDER_PASS_LIGHT_BUFFER, { m_lightProgram->getHandle() }, m_spvbh, m_spibh, { (float)i*20, (float)j * 20,  -20.f}, {1.f,1.f,1.f}, 10.8f, lightDrawState);
					//drawModels(RENDER_PASS_GBUFFER, { m_gbufferProgram->getHandle() }, m_spvbh, m_spibh, { (float)i * 20, (float)j * 20, -18.f }, { 1.f,1.f,1.f }, 10.8f);
				}
            }

			// Draw combine pass

			// Texture inputs for combine pass
			bgfx::setTexture(0, s_normal, m_gbufferPipeline->getNormalTexture());
			bgfx::setTexture(1, s_color, m_gbufferPipeline->getColorTexture());
			bgfx::setTexture(2, s_light,    m_lightPipeline->getTexture() );
			bgfx::setTexture(3, s_depth, m_gbufferPipeline->getDepthTexture());
			bgfx::setTexture(4, s_shadowMap, m_shadowPipline->getDepthTexture(), BGFX_SAMPLER_COMPARE_LEQUAL);

			// Uniforms for combine pass
			bgfx::setUniform(u_lightDir, m_lightDir);
			bgfx::setUniform(u_invMvp, invMvp);
			bgfx::setUniform(u_lightMtx, lightMtx);
			const float invDim[4] = {1.0f/SHADOW_MAP_DIM, 0.0f, 0.0f, 0.0f};
			bgfx::setUniform(u_shadowDimsInv, invDim);
			float rsmAmount[4] = {m_rsmAmount,m_rsmAmount,m_rsmAmount,m_rsmAmount};
			bgfx::setUniform(u_rsmAmount, rsmAmount);

			// Set up state for combine pass
			// point of this is to avoid doing depth test, which is in the default state
			bgfx::setState(0
				| BGFX_STATE_WRITE_RGB
				| BGFX_STATE_WRITE_A
				);


			// Set up transform matrix for fullscreen quad
			float orthoProj[16];
			bx::mtxOrtho(orthoProj, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 100.0f, 0.0f, caps->homogeneousDepth);
			bgfx::setViewTransform(RENDER_PASS_COMBINE, NULL, orthoProj);
			bgfx::setViewRect(RENDER_PASS_COMBINE, 0, 0, uint16_t(width), uint16_t(height) );
			screenSpaceQuad((float)width, (float)height, m_texelHalf, m_caps->originBottomLeft);
			bgfx::submit(RENDER_PASS_COMBINE, {m_combineProgram->getHandle()});

			updateLightDir();

			if (ImGui::Begin("Cam")) {

				if (ImGui::SliderFloat("X", &m_camPos.x, -100.f, 100.f))
				{
					m_cam->setPosition(m_camPos);
				}
				if (ImGui::SliderFloat("Y", &m_camPos.y, -100.f, 100.f))
				{
					m_cam->setPosition(m_camPos);
				}
				if (ImGui::SliderFloat("Z", &m_camPos.z, -100.f, 100.f))
				{
					m_cam->setPosition(m_camPos);
				}

				if (ImGui::SliderFloat("Pitch", &m_camRot.x, -100.f, 100.f))
				{
					m_cam->setRotation(m_camRot);
				}
				if (ImGui::SliderFloat("Yaw", &m_camRot.y, -100.f, 100.f))
				{
					m_cam->setRotation(m_camRot);
				}
				if (ImGui::SliderFloat("Roll", &m_camRot.z, -100.f, 100.f))
				{
					m_cam->setRotation(m_camRot);
				}
			}
			ImGui::End();

			if (ImGui::Begin("Light1")) {

				if (ImGui::SliderFloat("X##Light", &m_lightPos.x, -100.f, 100.f))
				{
					m_lightCam->setPosition(m_lightPos);
				}
				if (ImGui::SliderFloat("Y##Light", &m_lightPos.y, -100.f, 100.f))
				{
					m_lightCam->setPosition(m_lightPos);
				}
				if (ImGui::SliderFloat("Z##Light", &m_lightPos.z, -100.f, 100.f))
				{
					m_lightCam->setPosition(m_lightPos);
				}

				if (ImGui::SliderFloat("Pitch##Light", &m_lightRot.x, -100.f, 100.f))
				{
					m_lightCam->setRotation(m_lightRot);
				}
				if (ImGui::SliderFloat("Yaw##Light", &m_lightRot.y, -100.f, 100.f))
				{
					m_lightCam->setRotation(m_lightRot);
				}
				if (ImGui::SliderFloat("Roll##Light", &m_lightRot.z, -100.f, 100.f))
				{
					m_lightCam->setRotation(m_lightRot);
				}
			}
			

			ImGui::End();

			if (ImGui::Begin("Light"))
			{
				ImGui::InputFloat("Elevation", &m_lightElevation);
				ImGui::InputFloat("Azimuth", &m_lightAzimuth);
			}

			ImGui::End();

	}

	void CBgfxGraphicsTest03::screenSpaceQuad(float _textureWidth, float _textureHeight, float _texelHalf, bool _originBottomLeft, float _width, float _height)
	{
		if (3 == bgfx::getAvailTransientVertexBuffer(3, PosTexCoord0Vertex::ms_layout) )
		{
			bgfx::TransientVertexBuffer vb;
			bgfx::allocTransientVertexBuffer(&vb, 3, PosTexCoord0Vertex::ms_layout);
			PosTexCoord0Vertex* vertex = (PosTexCoord0Vertex*)vb.data;

			const float minx = -_width;
			const float maxx =  _width;
			const float miny = 0.0f;
			const float maxy = _height*2.0f;

			const float texelHalfW = _texelHalf/_textureWidth;
			const float texelHalfH = _texelHalf/_textureHeight;
			const float minu = -1.0f + texelHalfW;
			const float maxu =  1.0f + texelHalfH;

			const float zz = 0.0f;

			float minv = texelHalfH;
			float maxv = 2.0f + texelHalfH;

			if (_originBottomLeft)
			{
				float temp = minv;
				minv = maxv;
				maxv = temp;

				minv -= 1.0f;
				maxv -= 1.0f;
			}

			vertex[0].m_x = minx;
			vertex[0].m_y = miny;
			vertex[0].m_z = zz;
			vertex[0].m_u = minu;
			vertex[0].m_v = minv;

			vertex[1].m_x = maxx;
			vertex[1].m_y = miny;
			vertex[1].m_z = zz;
			vertex[1].m_u = maxu;
			vertex[1].m_v = minv;

			vertex[2].m_x = maxx;
			vertex[2].m_y = maxy;
			vertex[2].m_z = zz;
			vertex[2].m_u = maxu;
			vertex[2].m_v = maxv;

			bgfx::setVertexBuffer(0, &vb);
		}
	}

	void CBgfxGraphicsTest03::Shutdown()
	{
		if (m_gbufferPipeline)
		{
			m_gbufferPipeline->shutdown();
			delete m_gbufferPipeline;
		}

		if (m_shadowPipline)
		{
			m_shadowPipline->shutdown();
			delete m_shadowPipline;
		}


		if (m_lightPipeline)
		{
			m_lightPipeline->shutdown();
			delete m_lightPipeline;
		}

		if (m_combineProgram)
		{
			m_combineProgram->shutdown();
			delete m_combineProgram;
		}

		if (m_smesh)
		{
			delete m_smesh;
		}

		bgfx::destroy(u_tint);
		bgfx::destroy(u_lightDir);
		bgfx::destroy(u_sphereInfo);
		bgfx::destroy(u_invMvp);
		bgfx::destroy(u_invMvpShadow);
		bgfx::destroy(u_lightMtx);
		bgfx::destroy(u_shadowDimsInv);
		bgfx::destroy(u_rsmAmount);
		bgfx::destroy(s_normal);
		bgfx::destroy(s_depth);
		bgfx::destroy(s_color);
		bgfx::destroy(s_light);
		bgfx::destroy(s_shadowMap);
		bgfx::destroy(s_rsm);

		bgfx::destroy(m_vbh);
		bgfx::destroy(m_ibh);
		bgfx::destroy(m_spibh);
		bgfx::destroy(m_spvbh);
		bgfx::destroy(m_cbcvh);
		bgfx::destroy(m_cbibh);
	}

}


#endif
