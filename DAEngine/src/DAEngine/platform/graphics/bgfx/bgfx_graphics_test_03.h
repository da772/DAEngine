#pragma once
#ifdef DA_GRAPHICS_BGFX
#include <bgfx/bgfx.h>
namespace da::core { class CWindow; class CStaticMesh; }

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
		void drawModels(int pass, bgfx::ProgramHandle handle, uint64_t state = 0);
		void screenSpaceQuad(float _textureWidth, float _textureHeight, float _texelHalf, bool _originBottomLeft, float _width = 1.0f, float _height = 1.0f);

	private:
		da::core::CStaticMesh* m_smesh;
		bgfx::VertexBufferHandle m_vbh;
		bgfx::IndexBufferHandle m_ibh;
		void* m_material;
		da::core::CWindow* m_window;
		double m_start = 0;
		bgfx::UniformHandle u_tint,u_lightDir,u_sphereInfo,u_invMvp,u_invMvpShadow,u_lightMtx,u_shadowDimsInv, u_rsmAmount;
		bgfx::UniformHandle s_normal,s_depth,s_color,s_light,s_shadowMap,s_rsm;
		CBgfxGraphicsMaterial* m_gbufferProgram, *m_shadowProgram,*m_lightProgram,*m_combineProgram;
		bgfx::TextureHandle m_gbufferTex[3];
		bgfx::TextureHandle m_shadowBufferTex[2];
		bgfx::TextureHandle m_lightBufferTex;
		bgfx::VertexLayout m_ms_layout;
		bgfx::FrameBufferHandle m_gbuffer,m_lightBuffer,m_shadowBuffer;

		// Light position;
		float m_lightDir[4];
		float m_lightElevation = 35.f;
		float m_lightAzimuth = 215.0f;

		float m_rsmAmount = .25f; // Amount of rsm
		float m_vplRadius = 3.0f; // Radius of virtual point light
		float m_texelHalf = 0.f;

		const bgfx::Caps* m_caps;
	};
}

#endif

