#include "dapch.h"
#include "bgfx_graphics_test_03.h"

#ifdef DA_GRAPHICS_BGFX

#include <chrono>
#include <bx/math.h>
#include "DAEngine/core/graphics/graphics_smesh.h"
#include "DAEngine/platform/graphics/bgfx/bgfx_graphics_material.h"
#include <imgui.h>
#include <stb_image.h>
#include <bimg/bimg.h>

// Render passes
#define RENDER_PASS_GBUFFER      0  // GBuffer for normals and albedo
#define RENDER_PASS_SHADOW_MAP   1  // Draw into the shadow map (RSM and regular shadow map at same time)
#define RENDER_PASS_LIGHT_BUFFER 2  // Light buffer for point lights
#define RENDER_PASS_COMBINE      3  // Directional light and final result

// Gbuffer has multiple render targets
#define GBUFFER_RT_NORMAL 0
#define GBUFFER_RT_COLOR  1
#define GBUFFER_RT_DEPTH  2

// Shadow map has multiple render targets
#define SHADOW_RT_RSM   0        // In this algorithm, shadows write lighting info as well.
#define SHADOW_RT_DEPTH 1        // Shadow maps always write a depth

// Random meshes we draw
#define MODEL_COUNT 222  // In this demo, a model is a mesh plus a transform and a color

#define SHADOW_MAP_DIM 512
#define LIGHT_DIST 10.0f


namespace da::platform {

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

        const uint64_t tsFlags = 0
			| BGFX_TEXTURE_RT
			| BGFX_SAMPLER_MIN_POINT
			| BGFX_SAMPLER_MAG_POINT
			| BGFX_SAMPLER_MIP_POINT
			| BGFX_SAMPLER_U_CLAMP
			| BGFX_SAMPLER_V_CLAMP
			;

        		// Labeling for renderdoc captures, etc
		bgfx::setViewName(RENDER_PASS_GBUFFER,      "gbuffer"     );
		bgfx::setViewName(RENDER_PASS_SHADOW_MAP,   "shadow map"  );
		bgfx::setViewName(RENDER_PASS_LIGHT_BUFFER, "light buffer");
		bgfx::setViewName(RENDER_PASS_COMBINE,      "post combine");

        // Set up screen clears
		bgfx::setViewClear(RENDER_PASS_GBUFFER
			, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
			, 0
			, 1.0f
			, 0
			);

		bgfx::setViewClear(RENDER_PASS_LIGHT_BUFFER
			, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
			, 0
			, 1.0f
			, 0
			);

		bgfx::setViewClear(RENDER_PASS_SHADOW_MAP
			, BGFX_CLEAR_COLOR|BGFX_CLEAR_DEPTH
			, 0
			, 1.0f
			, 0
			);

		// Create uniforms
		u_tint          = bgfx::createUniform("u_tint",          bgfx::UniformType::Vec4);  // Tint for when you click on items
		u_tint          = bgfx::createUniform("u_tint",          bgfx::UniformType::Vec4);  // Tint for when you click on items
		u_lightDir      = bgfx::createUniform("u_lightDir",      bgfx::UniformType::Vec4);  // Single directional light for entire scene
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

		// Create program from shaders.
		m_gbufferProgram = new CBgfxGraphicsMaterial("shaders/rsm/macosx/vs_rsm_gbuffer.sc.vk", "shaders/rsm/macosx/fs_rsm_gbuffer.sc.vk");  // Gbuffer
		m_shadowProgram  = new CBgfxGraphicsMaterial("shaders/rsm/macosx/vs_rsm_shadow.sc.vk",  "shaders/rsm/macosx/fs_rsm_shadow.sc.vk"  ); // Drawing shadow map
		m_lightProgram   = new CBgfxGraphicsMaterial("shaders/rsm/macosx/vs_rsm_lbuffer.sc.vk", "shaders/rsm/macosx/fs_rsm_lbuffer.sc.vk");  // Light buffer
		m_combineProgram = new CBgfxGraphicsMaterial("shaders/rsm/macosx/vs_rsm_combine.sc.vk", "shaders/rsm/macosx/fs_rsm_combine.sc.vk");  // Combiner

		m_window = window;
		ms_layout
				.begin()
				.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Color0, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Normal, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Tangent, 3, bgfx::AttribType::Float)
				.add(bgfx::Attrib::Bitangent, 3, bgfx::AttribType::Float)
				.end();
		m_smesh = new da::core::CStaticMesh("assets/bolt.fbx");

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

        m_gbufferTex[GBUFFER_RT_NORMAL] = bgfx::createTexture2D(bgfx::BackbufferRatio::Equal, false, 1, bgfx::TextureFormat::BGRA8, tsFlags);
		m_gbufferTex[GBUFFER_RT_COLOR]  = bgfx::createTexture2D(bgfx::BackbufferRatio::Equal, false, 1, bgfx::TextureFormat::BGRA8, tsFlags);
		m_gbufferTex[GBUFFER_RT_DEPTH]  = bgfx::createTexture2D(bgfx::BackbufferRatio::Equal, false, 1, bgfx::TextureFormat::D32F,  tsFlags);
		m_gbuffer = bgfx::createFrameBuffer(BX_COUNTOF(m_gbufferTex), m_gbufferTex, true);

        // Make light buffer
		m_lightBufferTex = bgfx::createTexture2D(bgfx::BackbufferRatio::Equal, false, 1, bgfx::TextureFormat::BGRA8, tsFlags);
		bgfx::TextureHandle lightBufferRTs[] =  {
			m_lightBufferTex
		};
		m_lightBuffer = bgfx::createFrameBuffer(BX_COUNTOF(lightBufferRTs), lightBufferRTs, true);

        // Make shadow buffer
		const uint64_t rsmFlags = 0
			| BGFX_TEXTURE_RT
			| BGFX_SAMPLER_MIN_POINT
			| BGFX_SAMPLER_MAG_POINT
			| BGFX_SAMPLER_MIP_POINT
			| BGFX_SAMPLER_U_CLAMP
			| BGFX_SAMPLER_V_CLAMP
			;

		// Reflective shadow map
		m_shadowBufferTex[SHADOW_RT_RSM] = bgfx::createTexture2D(
				  SHADOW_MAP_DIM
				, SHADOW_MAP_DIM
				, false
				, 1
				, bgfx::TextureFormat::BGRA8
				, rsmFlags
				);

		// Typical shadow map
		m_shadowBufferTex[SHADOW_RT_DEPTH] = bgfx::createTexture2D(
				  SHADOW_MAP_DIM
				, SHADOW_MAP_DIM
				, false
				, 1
				, bgfx::TextureFormat::D16
				, BGFX_TEXTURE_RT /* | BGFX_SAMPLER_COMPARE_LEQUAL*/
				);  // Note I'm not setting BGFX_SAMPLER_COMPARE_LEQUAL.  Why?
					// Normally a PCF shadow map such as this requires a compare.  However, this sample also
					// reads from this texture in the lighting pass, and only uses the PCF capabilities in
					// the combine pass, so the flag is disabled by default.

		m_shadowBuffer = bgfx::createFrameBuffer(BX_COUNTOF(m_shadowBufferTex), m_shadowBufferTex, true);

        updateLightDir();
	}

	void CBgfxGraphicsTest03::Render()
	{
		double time = (double)std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count() / 1e3f - m_start;

		const bx::Vec3 at = { 0.0f, 0.0f,   0.0f };
		const bx::Vec3 eye = { 0.0f, 0.0f, -35.0f };

		uint32_t width = m_window->getWindowData().Width;
		uint32_t height = m_window->getWindowData().Height;

		// Set view and projection matrix for view 0.
		float view[16];
		bx::mtxLookAt(view, eye, at);

		float proj[16];
		bx::mtxProj(proj, 60.f, float(width) / float(height), 0.1f, 100.0f, bgfx::getCaps()->homogeneousDepth);

        bgfx::setViewRect(RENDER_PASS_GBUFFER, 0, 0, uint16_t(width), uint16_t(height));
        bgfx::setViewTransform(RENDER_PASS_GBUFFER, view, proj);
        // Make sure when we draw it goes into gbuffer and not backbuffer
        bgfx::setViewFrameBuffer(RENDER_PASS_GBUFFER, m_gbuffer);

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

		// Set vertex and index buffer.
		bgfx::setVertexBuffer(RENDER_PASS_GBUFFER, m_vbh);
		bgfx::setIndexBuffer(ibh);

		// Set render states.
		bgfx::setState(state);
        float v4[] = {1.f,1.f,1.f,1.f};
        bgfx::setUniform(u_tint, v4);

		bgfx::submit(RENDER_PASS_GBUFFER, {m_gbufferProgram->getHandle()});

        // Set up transforms for shadow map
        float smView[16], smProj[16], lightEye[3], lightAt[3];
        lightEye[0] = m_lightDir[0]*LIGHT_DIST;
        lightEye[1] = m_lightDir[1]*LIGHT_DIST;
        lightEye[2] = m_lightDir[2]*LIGHT_DIST;

        lightAt[0] = 0.0f;
        lightAt[1] = 0.0f;
        lightAt[2] = 0.0f;

        bx::mtxLookAt(smView, bx::load<bx::Vec3>(lightEye), bx::load<bx::Vec3>(lightAt) );
        const float area = 10.0f;
        const bgfx::Caps* caps = bgfx::getCaps();
        bx::mtxOrtho(smProj, -area, area, -area, area, -100.0f, 100.0f, 0.0f, caps->homogeneousDepth);
        bgfx::setViewTransform(RENDER_PASS_SHADOW_MAP, smView, smProj);
        bgfx::setViewFrameBuffer(RENDER_PASS_SHADOW_MAP, m_shadowBuffer);
        bgfx::setViewRect(RENDER_PASS_SHADOW_MAP, 0, 0, SHADOW_MAP_DIM, SHADOW_MAP_DIM);


        // Set model matrix for rendering.
		bgfx::setTransform(mtx);

		// Set vertex and index buffer.
		bgfx::setVertexBuffer(RENDER_PASS_GBUFFER, m_vbh);
		bgfx::setIndexBuffer(ibh);

		// Set render states.
		bgfx::setState(state);
        bgfx::setUniform(u_tint, v4);

		bgfx::submit(RENDER_PASS_GBUFFER, {m_shadowProgram->getHandle()});

        // Set up matrices for light buffer
			bgfx::setViewRect(RENDER_PASS_LIGHT_BUFFER, 0, 0, uint16_t(width), uint16_t(height));
			bgfx::setViewTransform(RENDER_PASS_LIGHT_BUFFER, view, proj);  // Notice, same view and proj as gbuffer
			// Set drawing into light buffer
			bgfx::setViewFrameBuffer(RENDER_PASS_LIGHT_BUFFER, m_lightBuffer);

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
			const uint32_t kMaxSpheres = 32;
			for (uint32_t i = 0; i < kMaxSpheres; i++)
			{
				for (uint32_t j = 0; j < kMaxSpheres; j++)
				{
					// These are used in the fragment shader
					bgfx::setTexture(0, s_normal, bgfx::getTexture(m_gbuffer, GBUFFER_RT_NORMAL) );  // Normal for lighting calculations
					bgfx::setTexture(1, s_depth,  bgfx::getTexture(m_gbuffer, GBUFFER_RT_DEPTH) );   // Depth to reconstruct world position

					// Thse are used in the vert shader
					bgfx::setTexture(2, s_shadowMap, bgfx::getTexture(m_shadowBuffer, SHADOW_RT_DEPTH) );  // Used to place sphere
					bgfx::setTexture(3, s_rsm,       bgfx::getTexture(m_shadowBuffer, SHADOW_RT_RSM) );    // Used to scale/color sphere

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

					//Draw lights
				}
            }

	}

	void CBgfxGraphicsTest03::Shutdown()
	{
	}

}


#endif