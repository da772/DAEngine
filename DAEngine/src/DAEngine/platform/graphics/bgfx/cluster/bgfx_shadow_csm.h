#pragma once
#include <bgfx/bgfx.h>

#define SHADOW_MAP_RENDER_TARGETS 4
#define SHADOW_MAP_RENDER_TARGET_SIZE 4096

namespace da::core
{
	class CCamera;
}

namespace da::platform
{
	class CBgfxGraphicsMaterial;

	class CBgfxShadowCSM
	{
	public:
		void initialize();
		void render(uint8_t view, const glm::vec3& lightDir, float width, float height);
		void shutdown();

		void setRenderFunc(const std::function<void(uint8_t id, CBgfxGraphicsMaterial* shader, uint64_t state)>& f);
		const float* getShadowMat(uint8_t index) const;
		::bgfx::TextureHandle getShadowMap(uint8_t index);

		CBgfxGraphicsMaterial* TEST_GET_MAT() { return m_colorLighting; };

		void submitUniforms();
	private:
		void splitFrustum(float* _splits, uint8_t _numSplits, float _near, float _far, float _splitWeight = 0.75f);
		void worldSpaceFrustumCorners(float* _corners24f, float _near, float _far, float _projWidth, float _projHeight, const float* _invViewMtx);
		void convertLeftToRightHandedZUp(float* matrix);

#if defined(DA_DEBUG) || defined(DA_RELEASE)
		void renderDebug();
#endif

	private:
		union
		{
			struct
			{
				float m_ambientPass;
				float m_lightingPass;
				float m_unused00;
				float m_unused01;
			};

			float m_params0[4];
		};


		union
		{
			struct
			{
				float m_shadowMapBias;
				float m_shadowMapOffset;
				float m_shadowMapParam0;
				float m_shadowMapParam1;
			};

			float m_params1[4];
		};

		union
		{
			struct
			{
				float m_XNum;
				float m_YNum;
				float m_XOffset;
				float m_YOffset;
			};

			float m_paramsBlur[4];
		};

		union
		{
			struct
			{
				float m_depthValuePow;
				float m_showSmCoverage;
				float m_shadowMapTexelSize;
				float m_unused23;
			};

			float m_params2[4];
		};

		struct RenderState
		{
			enum Enum
			{
				Default = 0,

				ShadowMap_PackDepth,
				ShadowMap_PackDepthHoriz,
				ShadowMap_PackDepthVert,

				Custom_BlendLightTexture,
				Custom_DrawPlaneBottom,

				Count
			};

			uint64_t m_state;
			uint32_t m_blendFactorRgba;
			uint32_t m_fstencil;
			uint32_t m_bstencil;
		};

		const RenderState m_renderStates[RenderState::Count] =
		{
			{ // Default
				0
				| BGFX_STATE_WRITE_RGB
				| BGFX_STATE_WRITE_A
				| BGFX_STATE_DEPTH_TEST_LESS
				| BGFX_STATE_WRITE_Z
				| BGFX_STATE_CULL_CCW
				| BGFX_STATE_MSAA
				, UINT32_MAX
				, BGFX_STENCIL_NONE
				, BGFX_STENCIL_NONE
			},
			{ // ShadowMap_PackDepth
				0
				| BGFX_STATE_WRITE_RGB
				| BGFX_STATE_WRITE_A
				| BGFX_STATE_WRITE_Z
				| BGFX_STATE_DEPTH_TEST_LESS
				| BGFX_STATE_CULL_CCW
				| BGFX_STATE_MSAA
				, UINT32_MAX
				, BGFX_STENCIL_NONE
				, BGFX_STENCIL_NONE
			},
			{ // ShadowMap_PackDepthHoriz
				0
				| BGFX_STATE_WRITE_RGB
				| BGFX_STATE_WRITE_A
				| BGFX_STATE_WRITE_Z
				| BGFX_STATE_DEPTH_TEST_LESS
				| BGFX_STATE_CULL_CCW
				| BGFX_STATE_MSAA
				, UINT32_MAX
				, BGFX_STENCIL_TEST_EQUAL
				| BGFX_STENCIL_FUNC_REF(1)
				| BGFX_STENCIL_FUNC_RMASK(0xff)
				| BGFX_STENCIL_OP_FAIL_S_KEEP
				| BGFX_STENCIL_OP_FAIL_Z_KEEP
				| BGFX_STENCIL_OP_PASS_Z_KEEP
				, BGFX_STENCIL_NONE
			},
			{ // ShadowMap_PackDepthVert
				0
				| BGFX_STATE_WRITE_RGB
				| BGFX_STATE_WRITE_A
				| BGFX_STATE_WRITE_Z
				| BGFX_STATE_DEPTH_TEST_LESS
				| BGFX_STATE_CULL_CCW
				| BGFX_STATE_MSAA
				, UINT32_MAX
				, BGFX_STENCIL_TEST_EQUAL
				| BGFX_STENCIL_FUNC_REF(0)
				| BGFX_STENCIL_FUNC_RMASK(0xff)
				| BGFX_STENCIL_OP_FAIL_S_KEEP
				| BGFX_STENCIL_OP_FAIL_Z_KEEP
				| BGFX_STENCIL_OP_PASS_Z_KEEP
				, BGFX_STENCIL_NONE
			},
			{ // Custom_BlendLightTexture
				BGFX_STATE_WRITE_RGB
				| BGFX_STATE_WRITE_A
				| BGFX_STATE_WRITE_Z
				| BGFX_STATE_DEPTH_TEST_LESS
				| BGFX_STATE_BLEND_FUNC(BGFX_STATE_BLEND_SRC_COLOR, BGFX_STATE_BLEND_INV_SRC_COLOR)
				| BGFX_STATE_CULL_CCW
				| BGFX_STATE_MSAA
				, UINT32_MAX
				, BGFX_STENCIL_NONE
				, BGFX_STENCIL_NONE
			},
			{ // Custom_DrawPlaneBottom
				BGFX_STATE_WRITE_RGB
				| BGFX_STATE_CULL_CW
				| BGFX_STATE_MSAA
				, UINT32_MAX
				, BGFX_STENCIL_NONE
				, BGFX_STENCIL_NONE
			},
		};

	private:
		::bgfx::UniformHandle u_params0;
		::bgfx::UniformHandle u_params1;
		::bgfx::UniformHandle u_params2;
		::bgfx::UniformHandle u_color;
		::bgfx::UniformHandle u_smSamplingParams;
		::bgfx::UniformHandle u_csmFarDistances;

		::bgfx::UniformHandle u_lightMtx;
		::bgfx::FrameBufferHandle m_rtBlur;

		float m_csmFarDistances[4];

		float m_shadowMapMtx[SHADOW_MAP_RENDER_TARGETS][16];

		CBgfxGraphicsMaterial * m_drawDepth;
		CBgfxGraphicsMaterial* m_vBlur;
		CBgfxGraphicsMaterial* m_hBlur;
		CBgfxGraphicsMaterial* m_packDepth;
		CBgfxGraphicsMaterial* m_colorLighting;

		::bgfx::FrameBufferHandle m_rtShadowMap[SHADOW_MAP_RENDER_TARGETS];

		std::function<void(uint8_t id, CBgfxGraphicsMaterial* shader, uint64_t state)> m_renderFunc;

		float m_near, m_far;
		float m_cascadePerc;

		bool m_showDebug;

	};
}