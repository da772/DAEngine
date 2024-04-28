#pragma once

#include <bgfx/bgfx.h>
#include "core/graphics/graphics_material.h"

namespace da::platform {


	struct ViewState
	{
		ViewState(uint16_t _width = 1280, uint16_t _height = 720)
			: m_width(_width)
			, m_height(_height)
		{
		}

		uint16_t m_width;
		uint16_t m_height;

		float m_view[16];
		float m_proj[16];
	};

	struct ClearValues
	{
		ClearValues(uint32_t _clearRgba = 0x30303000
			, float    _clearDepth = 1.0f
			, uint8_t  _clearStencil = 0
		)
			: m_clearRgba(_clearRgba)
			, m_clearDepth(_clearDepth)
			, m_clearStencil(_clearStencil)
		{
		}

		uint32_t m_clearRgba;
		float    m_clearDepth;
		uint8_t  m_clearStencil;
	};

	struct PosColorTexCoord0Vertex
	{
		float m_x;
		float m_y;
		float m_z;
		uint32_t m_rgba;
		float m_u;
		float m_v;

		static void init()
		{
			ms_layout
				.begin()
				.add(::bgfx::Attrib::Position, 3, ::bgfx::AttribType::Float)
				.add(::bgfx::Attrib::Color0, 4, ::bgfx::AttribType::Uint8, true)
				.add(::bgfx::Attrib::TexCoord0, 2, ::bgfx::AttribType::Float)
				.end();
		}

		static ::bgfx::VertexLayout ms_layout;
	};


	struct LightType
	{
		enum Enum
		{
			DirectionalLight,

			Count
		};

	};

	struct DepthImpl
	{
		enum Enum
		{
			InvZ,
			Linear,

			Count
		};
	};

	struct PackDepth
	{
		enum Enum
		{
			RGBA,
			VSM,

			Count
		};
	};

	struct SmImpl
	{
		enum Enum
		{
			Hard,
			PCF,
			VSM,
			ESM,

			Count
		};
	};

	struct SmType
	{
		enum Enum
		{
			Cascade,

			Count
		};
	};

	struct TetrahedronFaces
	{
		enum Enum
		{
			Green,
			Yellow,
			Blue,
			Red,

			Count
		};
	};

	struct ProjType
	{
		enum Enum
		{
			Horizontal,
			Vertical,

			Count
		};
	};

	struct ShadowMapRenderTargets
	{
		enum Enum
		{
			First,
			Second,
			Third,
			Fourth,

			Count
		};
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


	struct PosNormalTexcoordVertex
	{
		float    m_x;
		float    m_y;
		float    m_z;
		uint32_t m_normal;
		float    m_u;
		float    m_v;
	};
	struct ShadowMapSettings
	{
#define IMGUI_FLOAT_PARAM(_name) float _name, _name##Min, _name##Max, _name##Step
		IMGUI_FLOAT_PARAM(m_sizePwrTwo);
		IMGUI_FLOAT_PARAM(m_depthValuePow);
		IMGUI_FLOAT_PARAM(m_near);
		IMGUI_FLOAT_PARAM(m_far);
		IMGUI_FLOAT_PARAM(m_bias);
		IMGUI_FLOAT_PARAM(m_normalOffset);
		IMGUI_FLOAT_PARAM(m_customParam0);
		IMGUI_FLOAT_PARAM(m_customParam1);
		IMGUI_FLOAT_PARAM(m_xNum);
		IMGUI_FLOAT_PARAM(m_yNum);
		IMGUI_FLOAT_PARAM(m_xOffset);
		IMGUI_FLOAT_PARAM(m_yOffset);
		bool m_doBlur;
		da::graphics::CMaterial* m_progPack;
		da::graphics::CMaterial* m_progPackSk;
		da::graphics::CMaterial* m_progPackInstance;
		da::graphics::CMaterial* m_progDraw;
#undef IMGUI_FLOAT_PARAM
	};

	struct SceneSettings
	{
		LightType::Enum m_lightType;
		DepthImpl::Enum m_depthImpl;
		SmImpl::Enum m_smImpl;
		float m_spotOuterAngle;
		float m_spotInnerAngle;
		float m_fovXAdjust;
		float m_fovYAdjust;
		float m_coverageSpotL;
		float m_splitDistribution;
		int   m_numSplits;
		bool m_updateLights;
		bool m_updateScene;
		bool m_drawDepthBuffer;
		bool m_showSmCoverage;
		bool m_stencilPack;
		bool m_stabilize;
	};

	class CBgfxShadowCsm {
	public:
		void initialize();
		void update(uint8_t viewId, glm::vec3& lightDir, uint16_t width, uint16_t height);
		void shutdown();
		
		void setRenderFunc(const std::function<void(uint8_t, da::graphics::CMaterial*, da::graphics::CMaterial*, da::graphics::CMaterial*, RenderState)>& func);
		void submitUniforms();

	private:
		struct FPrograms
		{
			void init();
			void destroy();

		
			da::graphics::CMaterial* m_black;
			da::graphics::CMaterial* m_texture;
			da::graphics::CMaterial* m_colorTexture;
			da::graphics::CMaterial* m_vBlur[PackDepth::Count];
			da::graphics::CMaterial* m_hBlur[PackDepth::Count];
			da::graphics::CMaterial* m_drawDepth[PackDepth::Count];
			da::graphics::CMaterial* m_packDepth[DepthImpl::Count][PackDepth::Count];
			da::graphics::CMaterial* m_packDepthSk[DepthImpl::Count][PackDepth::Count];
			da::graphics::CMaterial* m_packDepthInstance[DepthImpl::Count][PackDepth::Count];
			da::graphics::CMaterial* m_colorLighting[SmType::Count][DepthImpl::Count][SmImpl::Count];
		};

		struct FUniforms
		{
			void init();

			void setPtrs(float* _colorPtr, float* _lightMtxPtr, float* _shadowMapMtx0, float* _shadowMapMtx1, float* _shadowMapMtx2, float* _shadowMapMtx3);

			// Call this once at initialization.
			void submitConstUniforms();

			// Call this once per frame.
			void submitPerFrameUniforms();

			// Call this before each draw call.
			void submitPerDrawUniforms();

			void destroy();

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
					float m_depthValuePow;
					float m_showSmCoverage;
					float m_shadowMapTexelSize;
					float m_unused23;
				};

				float m_params2[4];
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

			float m_tetraNormalGreen[3];
			float m_tetraNormalYellow[3];
			float m_tetraNormalBlue[3];
			float m_tetraNormalRed[3];
			float m_csmFarDistances[4];

			float* m_lightMtxPtr;
			float* m_colorPtr;
			float* m_shadowMapMtx0;
			float* m_shadowMapMtx1;
			float* m_shadowMapMtx2;
			float* m_shadowMapMtx3;

		public:
			::bgfx::UniformHandle u_params0;
			::bgfx::UniformHandle u_params1;
			::bgfx::UniformHandle u_params2;
			::bgfx::UniformHandle u_color;
			::bgfx::UniformHandle u_smSamplingParams;
			::bgfx::UniformHandle u_csmFarDistances;

			::bgfx::UniformHandle u_tetraNormalGreen;
			::bgfx::UniformHandle u_tetraNormalYellow;
			::bgfx::UniformHandle u_tetraNormalBlue;
			::bgfx::UniformHandle u_tetraNormalRed;

			::bgfx::UniformHandle u_shadowMapMtx0;
			::bgfx::UniformHandle u_shadowMapMtx1;
			::bgfx::UniformHandle u_shadowMapMtx2;
			::bgfx::UniformHandle u_shadowMapMtx3;

			::bgfx::UniformHandle u_lightMtx;
		};
	private:

		ViewState m_viewState;
		ClearValues m_clearValues;

		::bgfx::VertexLayout m_posLayout;

		float m_color[4];

		float m_lightMtx[16];
		float m_shadowMapMtx[ShadowMapRenderTargets::Count][16];

		ShadowMapSettings m_smSettings[LightType::Count][DepthImpl::Count][SmImpl::Count];
		SceneSettings m_settings;
		bgfx::UniformHandle m_shadowMap[ShadowMapRenderTargets::Count];
		bgfx::FrameBufferHandle m_rtShadowMap[ShadowMapRenderTargets::Count];

		uint16_t m_currentShadowMapSize;

		float m_timeAccumulatorLight;
		float m_timeAccumulatorScene;
#ifdef DA_REVIEW
		bool m_showDebug = false;
#endif

		FUniforms m_uniforms;
		FPrograms m_programs;

		static RenderState s_renderStates[RenderState::Count];

		std::function<void(uint8_t, da::graphics::CMaterial*, da::graphics::CMaterial*, da::graphics::CMaterial*, RenderState)> m_renderFunc;
	};

}