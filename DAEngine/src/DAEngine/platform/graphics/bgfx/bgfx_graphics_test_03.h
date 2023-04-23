#pragma once
#ifdef DA_GRAPHICS_BGFX
#include <bgfx/bgfx.h>
namespace da::core { class CWindow; class CStaticMesh; class CCamera; }

namespace da::platform
{
	class CBgfxGraphicsMaterial;

	class CBgfxGraphicsTest03
	{
	public:
		void Initialize(da::core::CWindow* window);
		void Render();
		void Shutdown();
		void updateLightDir();
		void drawModels(int pass, bgfx::ProgramHandle handle, Vector3f pos = { 0.f,0.f,0.f }, Vector3f scale = { 1.f,1.f,1.f }, float time = 0.f, uint64_t state = 0, bool submit = true);
		void drawModels(int pass, bgfx::ProgramHandle handle, bgfx::VertexBufferHandle vbh, bgfx::IndexBufferHandle ibh, Vector3f pos = { 0.f,0.f,0.f }, Vector3f scale = { 1.f,1.f,1.f }, float time = 0.f, uint64_t state = 0, bool submit = true);
		void screenSpaceQuad(float _textureWidth, float _textureHeight, float _texelHalf, bool _originBottomLeft, float _width = 1.0f, float _height = 1.0f);

	private:
		da::core::CStaticMesh* m_smesh;
		da::core::CStaticMesh* m_sphereMesh;
		da::core::CStaticMesh* m_cubeMesh;
		bgfx::VertexBufferHandle m_vbh, m_spvbh, m_cbcvh;
		bgfx::IndexBufferHandle m_ibh, m_spibh, m_cbibh;

		void* m_material;
		da::core::CWindow* m_window;
		double m_start = 0;
		bgfx::UniformHandle u_tint,u_lightDir,u_sphereInfo,u_invMvp,u_invMvpShadow,u_lightMtx,u_shadowDimsInv, u_rsmAmount;
		bgfx::UniformHandle s_normal,s_depth,s_color,s_light,s_shadowMap,s_rsm,s_texColor,s_texNormal;
		CBgfxGraphicsMaterial* m_gbufferProgram, *m_shadowProgram,*m_lightProgram,*m_combineProgram;
		bgfx::TextureHandle m_gbufferTex[3];
		bgfx::TextureHandle m_shadowBufferTex[2];
		bgfx::TextureHandle m_lightBufferTex;
		bgfx::TextureHandle m_colorTex,m_normalTex;
		bgfx::VertexLayout m_ms_layout;
		bgfx::FrameBufferHandle m_gbuffer,m_lightBuffer,m_shadowBuffer;

		// Light position;
		float m_lightDir[4];
		float m_lightElevation = 55.f;
		float m_lightAzimuth = 200.0f;

		float m_rsmAmount = .05f; // Amount of rsm
		float m_vplRadius = 100.f; // Radius of virtual point light
		float m_texelHalf = 0.f;

		Vector3f m_camPos = { 0.f,0.f,0.f };
		Vector3f m_camRot = { 0.f,0.f,0.f };
		Vector3f m_lightPos = { 0.f,0.f,0.f };
		Vector3f m_lightRot = { 0.f,0.f,0.f };


		da::core::CCamera* m_cam,* m_lightCam;

		const bgfx::Caps* m_caps;
	};
}

#endif

