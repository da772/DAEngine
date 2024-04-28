#include "dapch.h"
#include "bgfx_shadow_csm.h"
#include <bx/timer.h>
#include <bx/math.h>
#include <bx/file.h>
#include <bx/pixelformat.h>
#include <core/graphics/graphics_material.h>
#include <imgui.h>
#include <core/graphics/camera.h>
#include "bgfx_samplers.h"

#ifdef DA_REVIEW
#include "debug/debug_menu_bar.h"
#endif

namespace da::platform {


#define RENDERVIEW_SHADOWMAP_1_ID 0 + viewId
#define RENDERVIEW_SHADOWMAP_2_ID 1 + viewId
#define RENDERVIEW_SHADOWMAP_3_ID 2 + viewId
#define RENDERVIEW_SHADOWMAP_4_ID 3 + viewId
#define RENDERVIEW_VBLUR_0_ID     4 + viewId
#define RENDERVIEW_HBLUR_0_ID     5	+ viewId
#define RENDERVIEW_VBLUR_1_ID     6	+ viewId
#define RENDERVIEW_HBLUR_1_ID     7	+ viewId
#define RENDERVIEW_VBLUR_2_ID     8	+ viewId
#define RENDERVIEW_HBLUR_2_ID     9 + viewId
#define RENDERVIEW_VBLUR_3_ID     10 + viewId
#define RENDERVIEW_HBLUR_3_ID     11 + viewId

	static const float s_texcoord = 5.0f;

	inline uint32_t encodeNormalRgba8(float _x, float _y = 0.0f, float _z = 0.0f, float _w = 0.0f)
	{
		const float src[] =
		{
			_x * 0.5f + 0.5f,
			_y * 0.5f + 0.5f,
			_z * 0.5f + 0.5f,
			_w * 0.5f + 0.5f,
		};
		uint32_t dst;
		bx::packRgba8(&dst, src);
		return dst;
	}


	static PosNormalTexcoordVertex s_hplaneVertices[] =
	{
		{ -1.0f, 0.0f,  1.0f, encodeNormalRgba8(0.0f, 1.0f, 0.0f), s_texcoord, s_texcoord },
		{  1.0f, 0.0f,  1.0f, encodeNormalRgba8(0.0f, 1.0f, 0.0f), s_texcoord, 0.0f       },
		{ -1.0f, 0.0f, -1.0f, encodeNormalRgba8(0.0f, 1.0f, 0.0f), 0.0f,       s_texcoord },
		{  1.0f, 0.0f, -1.0f, encodeNormalRgba8(0.0f, 1.0f, 0.0f), 0.0f,       0.0f       },
	};

	static PosNormalTexcoordVertex s_vplaneVertices[] =
	{
		{ -1.0f,  1.0f, 0.0f, encodeNormalRgba8(0.0f, 0.0f, -1.0f), 1.0f, 1.0f },
		{  1.0f,  1.0f, 0.0f, encodeNormalRgba8(0.0f, 0.0f, -1.0f), 1.0f, 0.0f },
		{ -1.0f, -1.0f, 0.0f, encodeNormalRgba8(0.0f, 0.0f, -1.0f), 0.0f, 1.0f },
		{  1.0f, -1.0f, 0.0f, encodeNormalRgba8(0.0f, 0.0f, -1.0f), 0.0f, 0.0f },
	};

	static const uint16_t s_planeIndices[] =
	{
		0, 1, 2,
		1, 3, 2,
	};

	static bool s_originBottomLeft = false;

	::bgfx::VertexLayout PosColorTexCoord0Vertex::ms_layout;
	static bgfx::UniformHandle s_texColor;
	static bgfx::FrameBufferHandle s_rtBlur;

	void mtxBillboard(
		float* _result
		, const float* _view
		, const float* _pos
		, const float* _scale
	)
	{
		_result[0] = _view[0] * _scale[0];
		_result[1] = _view[4] * _scale[0];
		_result[2] = _view[8] * _scale[0];
		_result[3] = 0.0f;
		_result[4] = _view[1] * _scale[1];
		_result[5] = _view[5] * _scale[1];
		_result[6] = _view[9] * _scale[1];
		_result[7] = 0.0f;
		_result[8] = _view[2] * _scale[2];
		_result[9] = _view[6] * _scale[2];
		_result[10] = _view[10] * _scale[2];
		_result[11] = 0.0f;
		_result[12] = _pos[0];
		_result[13] = _pos[1];
		_result[14] = _pos[2];
		_result[15] = 1.0f;
	}

	void mtxYawPitchRoll(float* _result
		, float _yaw
		, float _pitch
		, float _roll
	)
	{
		float sroll = bx::sin(_roll);
		float croll = bx::cos(_roll);
		float spitch = bx::sin(_pitch);
		float cpitch = bx::cos(_pitch);
		float syaw = bx::sin(_yaw);
		float cyaw = bx::cos(_yaw);

		_result[0] = sroll * spitch * syaw + croll * cyaw;
		_result[1] = sroll * cpitch;
		_result[2] = sroll * spitch * cyaw - croll * syaw;
		_result[3] = 0.0f;
		_result[4] = croll * spitch * syaw - sroll * cyaw;
		_result[5] = croll * cpitch;
		_result[6] = croll * spitch * cyaw + sroll * syaw;
		_result[7] = 0.0f;
		_result[8] = cpitch * syaw;
		_result[9] = -spitch;
		_result[10] = cpitch * cyaw;
		_result[11] = 0.0f;
		_result[12] = 0.0f;
		_result[13] = 0.0f;
		_result[14] = 0.0f;
		_result[15] = 1.0f;
	}

	struct Material
	{
		union Ambient
		{
			struct
			{
				float m_r;
				float m_g;
				float m_b;
				float m_unused;
			};

			float m_v[4];
		};

		union Diffuse
		{
			struct
			{
				float m_r;
				float m_g;
				float m_b;
				float m_unused;
			};

			float m_v[4];
		};

		union Specular
		{
			struct
			{
				float m_r;
				float m_g;
				float m_b;
				float m_ns;
			};

			float m_v[4];
		};

		Ambient m_ka;
		Diffuse m_kd;
		Specular m_ks;
	};

	struct Light
	{
		union Position
		{
			struct
			{
				float m_x;
				float m_y;
				float m_z;
				float m_w;
			};

			float m_v[4];
		};

		union LightRgbPower
		{
			struct
			{
				float m_r;
				float m_g;
				float m_b;
				float m_power;
			};

			float m_v[4];
		};

		union SpotDirectionInner
		{
			struct
			{
				float m_x;
				float m_y;
				float m_z;
				float m_inner;
			};

			float m_v[4];
		};

		union AttenuationSpotOuter
		{
			struct
			{
				float m_attnConst;
				float m_attnLinear;
				float m_attnQuadrantic;
				float m_outer;
			};

			float m_v[4];
		};

		void computeViewSpaceComponents(float* _viewMtx)
		{
			bx::vec4MulMtx(m_position_viewSpace, m_position.m_v, _viewMtx);

			float tmp[] =
			{
				  m_spotDirectionInner.m_x
				, m_spotDirectionInner.m_y
				, m_spotDirectionInner.m_z
				, 0.0f
			};
			bx::vec4MulMtx(m_spotDirectionInner_viewSpace, tmp, _viewMtx);
			m_spotDirectionInner_viewSpace[3] = m_spotDirectionInner.m_v[3];
		}

		Position              m_position;
		float				  m_position_viewSpace[4];
		LightRgbPower         m_ambientPower;
		LightRgbPower         m_diffusePower;
		LightRgbPower         m_specularPower;
		SpotDirectionInner    m_spotDirectionInner;
		float				  m_spotDirectionInner_viewSpace[4];
		AttenuationSpotOuter  m_attenuationSpotOuter;
	};

	void screenSpaceQuad(bool _originBottomLeft = true, float _width = 1.0f, float _height = 1.0f)
	{
		if (3 == bgfx::getAvailTransientVertexBuffer(3, PosColorTexCoord0Vertex::ms_layout))
		{
			bgfx::TransientVertexBuffer vb;
			bgfx::allocTransientVertexBuffer(&vb, 3, PosColorTexCoord0Vertex::ms_layout);
			PosColorTexCoord0Vertex* vertex = (PosColorTexCoord0Vertex*)vb.data;

			const float zz = 0.0f;

			const float minx = -_width;
			const float maxx = _width;
			const float miny = 0.0f;
			const float maxy = _height * 2.0f;

			const float minu = -1.0f;
			const float maxu = 1.0f;

			float minv = 0.0f;
			float maxv = 2.0f;

			if (_originBottomLeft)
			{
				std::swap(minv, maxv);
				minv -= 1.0f;
				maxv -= 1.0f;
			}

			vertex[0].m_x = minx;
			vertex[0].m_y = miny;
			vertex[0].m_z = zz;
			vertex[0].m_rgba = 0xffffffff;
			vertex[0].m_u = minu;
			vertex[0].m_v = minv;

			vertex[1].m_x = maxx;
			vertex[1].m_y = miny;
			vertex[1].m_z = zz;
			vertex[1].m_rgba = 0xffffffff;
			vertex[1].m_u = maxu;
			vertex[1].m_v = minv;

			vertex[2].m_x = maxx;
			vertex[2].m_y = maxy;
			vertex[2].m_z = zz;
			vertex[2].m_rgba = 0xffffffff;
			vertex[2].m_u = maxu;
			vertex[2].m_v = maxv;

			bgfx::setVertexBuffer(0, &vb);
		}
	}

	void worldSpaceFrustumCorners(
		float* _corners24f
		, float _near
		, float _far
		, float _projWidth
		, float _projHeight
		, const float* _invViewMtx
	)
	{
		// Define frustum corners in view space.
		const float nw = _near * _projWidth;
		const float nh = _near * _projHeight;
		const float fw = _far * _projWidth;
		const float fh = _far * _projHeight;

		const uint8_t numCorners = 8;
		const bx::Vec3 corners[numCorners] =
		{
			{ -nw,  nh, _near },
			{  nw,  nh, _near },
			{  nw, -nh, _near },
			{ -nw, -nh, _near },
			{ -fw,  fh, _far  },
			{  fw,  fh, _far  },
			{  fw, -fh, _far  },
			{ -fw, -fh, _far  },
		};

		// Convert them to world space.
		float(*out)[3] = (float(*)[3])_corners24f;
		for (uint8_t ii = 0; ii < numCorners; ++ii)
		{
			bx::store(&out[ii], bx::mul(corners[ii], _invViewMtx));
		}
	}

	/**
	 * _splits = { near0, far0, near1, far1... nearN, farN }
	 * N = _numSplits
	 */
	void splitFrustum(float* _splits, uint8_t _numSplits, float _near, float _far, float _splitWeight = 0.75f)
	{
		const float l = _splitWeight;
		const float ratio = _far / _near;
		const int8_t numSlices = _numSplits * 2;
		const float numSlicesf = float(numSlices);

		// First slice.
		_splits[0] = _near;

		for (uint8_t nn = 2, ff = 1; nn < numSlices; nn += 2, ff += 2)
		{
			float si = float(int8_t(ff)) / numSlicesf;

			const float nearp = l * (_near * bx::pow(ratio, si)) + (1 - l) * (_near + (_far - _near) * si);
			_splits[nn] = nearp;          //near
			_splits[ff] = nearp * 1.005f; //far from previous split
		}

		// Last slice.
		_splits[numSlices - 1] = _far;

		_splits[0] = .1f;
		_splits[1] = 15.f;

		_splits[2] = 15.f;
		_splits[3] = 30.f;

		_splits[4] = 30.f;
		_splits[5] = 50.f;

		_splits[6] = 50.f;
		_splits[7] = 100.f;

	}


	RenderState CBgfxShadowCsm::s_renderStates[RenderState::Count] =
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
		}
	};

	void CBgfxShadowCsm::initialize()
	{
		m_viewState = ViewState(uint16_t(0), uint16_t(0));
		m_clearValues = ClearValues(0x00000000, 1.0f, 0);


		s_originBottomLeft = bgfx::getCaps()->originBottomLeft;

		// Uniforms.
		m_uniforms.init();
		s_texColor = bgfx::createUniform("s_texColor", bgfx::UniformType::Sampler);
		m_shadowMap[0] = bgfx::createUniform("m_shadowMap0", bgfx::UniformType::Sampler);
		m_shadowMap[1] = bgfx::createUniform("m_shadowMap1", bgfx::UniformType::Sampler);
		m_shadowMap[2] = bgfx::createUniform("m_shadowMap2", bgfx::UniformType::Sampler);
		m_shadowMap[3] = bgfx::createUniform("m_shadowMap3", bgfx::UniformType::Sampler);

		// Programs.
		m_programs.init();

		// Vertex declarations.
		bgfx::VertexLayout PosNormalTexcoordLayout;
		PosNormalTexcoordLayout.begin()
			.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float)
			.add(bgfx::Attrib::Normal, 4, bgfx::AttribType::Uint8, true, true)
			.add(bgfx::Attrib::TexCoord0, 2, bgfx::AttribType::Float)
			.end();

		m_posLayout.begin();
		m_posLayout.add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float);
		m_posLayout.end();

		PosColorTexCoord0Vertex::init();

		// Setup uniforms.
		m_color[0] = m_color[1] = m_color[2] = m_color[3] = 1.0f;
		m_uniforms.setPtrs(
			 m_color
			, m_lightMtx
			, &m_shadowMapMtx[ShadowMapRenderTargets::First][0]
			, &m_shadowMapMtx[ShadowMapRenderTargets::Second][0]
			, &m_shadowMapMtx[ShadowMapRenderTargets::Third][0]
			, &m_shadowMapMtx[ShadowMapRenderTargets::Fourth][0]
		);
		m_uniforms.submitConstUniforms();

#ifdef DA_REVIEW
		da::debug::CDebugMenuBar::register_debug(HASHSTR("Renderer"), HASHSTR("Shadows"), &m_showDebug, []() {});
#endif

		// Settings.
		ShadowMapSettings smSettings[LightType::Count][DepthImpl::Count][SmImpl::Count] =
		{
			{ //LightType::Directional

				{ //DepthImpl::InvZ

					{ //SmImpl::Hard
						13.0f, 7.0f, 13.0f, 1.0f         // m_sizePwrTwo
						, 1.0f, 1.0f, 20.0f, 1.0f          // m_depthValuePow
						, .10f, 1.0f, 10.0f, 1.0f          // m_near
						, 175.0f, 100.0f, 2000.0f, 50.0f   // m_far
						, 0.0015f, 0.0f, 0.01f, 0.00001f   // m_bias
						, 0.001f, 0.0f, 0.04f, 0.00001f    // m_normalOffset
						, 0.7f, 0.0f, 1.0f, 0.01f          // m_customParam0
						, 200.0f, 1.0f, 400.0f, 1.0f       // m_customParam1
						, 2.0f, 0.0f, 4.0f, 1.0f           // m_xNum
						, 2.0f, 0.0f, 4.0f, 1.0f           // m_yNum
						, 0.3f, 0.0f, 1.0f, 0.01f          // m_xOffset
						, 0.3f, 0.0f, 1.0f, 0.01f          // m_yOffset
						, true                             // m_doBlur
						, m_programs.m_packDepth[DepthImpl::InvZ][PackDepth::RGBA] //m_progPack
						, m_programs.m_packDepthSk[DepthImpl::InvZ][PackDepth::RGBA] //m_progPack
						, m_programs.m_packDepthInstance[DepthImpl::InvZ][PackDepth::RGBA] //m_packDepthInstance
						, m_programs.m_colorLighting[SmType::Cascade][DepthImpl::InvZ][SmImpl::Hard] //m_progDraw
					},
					{ //SmImpl::PCF
						13.0f, 7.0f, 13.0f, 1.0f         // m_sizePwrTwo
						, 1.0f, 1.0f, 20.0f, 1.0f          // m_depthValuePow
						, 0.10f, 1.0f, 99.0f, 1.0f          // m_near
						, 175.0f, 100.0f, 2000.0f, 50.0f   // m_far
						, 0.0015f, 0.0f, 0.01f, 0.00001f   // m_bias
						, 0.001f, 0.0f, 0.04f, 0.00001f    // m_normalOffset
						, 0.7f, 0.0f, 1.0f, 0.01f          // m_customParam0
						, 200.0f, 1.0f, 400.0f, 1.0f       // m_customParam1
						, 2.0f, 0.0f, 8.0f, 1.0f           // m_xNum
						, 2.0f, 0.0f, 8.0f, 1.0f           // m_yNum
						, 1.441f, 0.0f, 3.0f, 0.01f          // m_xOffset
						, 1.322f, 0.0f, 3.0f, 0.01f          // m_yOffset
						, true                             // m_doBlur
						, m_programs.m_packDepth[DepthImpl::InvZ][PackDepth::RGBA] //m_progPack
						, m_programs.m_packDepthSk[DepthImpl::InvZ][PackDepth::RGBA] //m_progPack
						, m_programs.m_packDepthInstance[DepthImpl::InvZ][PackDepth::RGBA] //m_packDepthInstance
						, m_programs.m_colorLighting[SmType::Cascade][DepthImpl::InvZ][SmImpl::PCF] //m_progDraw
					},
					{ //SmImpl::VSM
						13.0f, 7.0f, 13.0f, 1.0f         // m_sizePwrTwo
						, 1.0f, 1.0f, 20.0f, 1.0f          // m_depthValuePow
						, 1.0f, 1.0f, 10.0f, 1.0f          // m_near
						, 550.0f, 100.0f, 2000.0f, 50.0f   // m_far
						, 0.004f, 0.0f, 0.01f, 0.00001f    // m_bias
						, 0.001f, 0.0f, 0.04f, 0.00001f    // m_normalOffset
						, 0.02f, 0.0f, 0.04f, 0.00001f     // m_customParam0
						, 2500.0f, 1.0f, 5000.0f, 1.0f     // m_customParam1
						, 2.0f, 0.0f, 4.0f, 1.0f           // m_xNum
						, 2.0f, 0.0f, 4.0f, 1.0f           // m_yNum
						, 0.2f, 0.0f, 1.0f, 0.01f          // m_xOffset
						, 0.2f, 0.0f, 1.0f, 0.01f          // m_yOffset
						, true                             // m_doBlur
						, m_programs.m_packDepth[DepthImpl::InvZ][PackDepth::VSM] //m_progPack
						, m_programs.m_packDepthSk[DepthImpl::InvZ][PackDepth::VSM] //m_progPackSk
						, m_programs.m_packDepthInstance[DepthImpl::InvZ][PackDepth::VSM] //m_progPackIntance
						, m_programs.m_colorLighting[SmType::Cascade][DepthImpl::InvZ][SmImpl::VSM] //m_progDraw
					},
					{ //SmImpl::ESM
						13.0f, 7.0f, 13.0f, 1.0f         // m_sizePwrTwo
						, 1.0f, 1.0f, 20.0f, 1.0f          // m_depthValuePow
						, 1.0f, 1.0f, 10.0f, 0.01f         // m_near
						, 550.0f, 100.0f, 2000.0f, 50.0f   // m_far
						, 0.004f, 0.0f, 0.01f, 0.00001f    // m_bias
						, 0.001f, 0.0f, 0.04f, 0.00001f    // m_normalOffset
						, 0.7f, 0.0f, 1.0f, 0.01f          // m_customParam0
						, 9500.0f, 1.0f, 15000.0f, 1.0f    // m_customParam1
						, 2.0f, 0.0f, 4.0f, 1.0f           // m_xNum
						, 2.0f, 0.0f, 4.0f, 1.0f           // m_yNum
						, 0.2f, 0.0f, 1.0f, 0.01f          // m_xOffset
						, 0.2f, 0.0f, 1.0f, 0.01f          // m_yOffset
						, true                             // m_doBlur
						, m_programs.m_packDepth[DepthImpl::InvZ][PackDepth::RGBA] //m_progPack
						, m_programs.m_packDepthSk[DepthImpl::InvZ][PackDepth::RGBA] //m_progPack
						, m_programs.m_packDepthInstance[DepthImpl::InvZ][PackDepth::RGBA] //m_packDepthInstance
						, m_programs.m_colorLighting[SmType::Cascade][DepthImpl::InvZ][SmImpl::ESM] //m_progDraw
					}

				},
				{ //DepthImpl::Linear

					{ //SmImpl::Hard
						13.0f, 7.0f, 13.0f, 1.0f         // m_sizePwrTwo
						, 1.0f, 1.0f, 20.0f, 1.0f          // m_depthValuePow
						, 1.0f, 1.0f, 10.0f, 1.0f          // m_near
						, 550.0f, 100.0f, 2000.0f, 50.0f   // m_far
						, 0.003f, 0.0f, 0.01f, 0.00001f   // m_bias
						, 0.001f, 0.0f, 0.04f, 0.00001f    // m_normalOffset
						, 0.7f, 0.0f, 1.0f, 0.01f          // m_customParam0
						, 500.0f, 1.0f, 1000.0f, 1.0f      // m_customParam1
						, 2.0f, 0.0f, 4.0f, 1.0f           // m_xNum
						, 2.0f, 0.0f, 4.0f, 1.0f           // m_yNum
						, 0.2f, 0.0f, 1.0f, 0.01f          // m_xOffset
						, 0.2f, 0.0f, 1.0f, 0.01f          // m_yOffset
						, true                             // m_doBlur
						, m_programs.m_packDepth[DepthImpl::Linear][PackDepth::RGBA] //m_progPack
						, m_programs.m_packDepthSk[DepthImpl::Linear][PackDepth::RGBA] //m_progPack
						, m_programs.m_colorLighting[SmType::Cascade][DepthImpl::Linear][SmImpl::Hard] //m_progDraw
					},
					{ //SmImpl::PCF
						13.0f, 7.0f, 13.0f, 1.0f         // m_sizePwrTwo
						, 1.0f, 1.0f, 20.0f, 1.0f          // m_depthValuePow
						, 1.0f, 1.0f, 99.0f, 1.0f          // m_near
						, 550.0f, 100.0f, 2000.0f, 50.0f   // m_far
						, 0.003f, 0.0f, 0.01f, 0.00001f   // m_bias
						, 0.001f, 0.0f, 0.04f, 0.00001f    // m_normalOffset
						, 0.7f, 0.0f, 1.0f, 0.01f          // m_customParam0
						, 200.0f, 1.0f, 400.0f, 1.0f       // m_customParam1
						, 2.0f, 0.0f, 8.0f, 1.0f           // m_xNum
						, 2.0f, 0.0f, 8.0f, 1.0f           // m_yNum
						, 1.0f, 0.0f, 3.0f, 0.01f          // m_xOffset
						, 1.0f, 0.0f, 3.0f, 0.01f          // m_yOffset
						, true                             // m_doBlur
						, m_programs.m_packDepth[DepthImpl::Linear][PackDepth::RGBA] //m_progPack
						, m_programs.m_packDepthSk[DepthImpl::Linear][PackDepth::RGBA] //m_progPack
						, m_programs.m_colorLighting[SmType::Cascade][DepthImpl::Linear][SmImpl::PCF] //m_progDraw
					},
					{ //SmImpl::VSM
						13.0f, 7.0f, 13.0f, 1.0f         // m_sizePwrTwo
						, 1.0f, 1.0f, 20.0f, 1.0f          // m_depthValuePow
						, 1.0f, 1.0f, 10.0f, 1.0f          // m_near
						, 550.0f, 100.0f, 2000.0f, 50.0f   // m_far
						, 0.004f, 0.0f, 0.01f, 0.00001f    // m_bias
						, 0.0015f, 0.0f, 0.04f, 0.00001f    // m_normalOffset
						, 0.02f, 0.0f, 0.04f, 0.00001f     // m_customParam0
						, 2500.0f, 1.0f, 5000.0f, 1.0f     // m_customParam1
						, 2.0f, 0.0f, 4.0f, 1.0f           // m_xNum
						, 2.0f, 0.0f, 4.0f, 1.0f           // m_yNum
						, 0.2f, 0.0f, 1.0f, 0.01f          // m_xOffset
						, 0.2f, 0.0f, 1.0f, 0.01f          // m_yOffset
						, true                             // m_doBlur
						, m_programs.m_packDepth[DepthImpl::Linear][PackDepth::VSM] //m_progPack
						, m_programs.m_packDepthSk[DepthImpl::Linear][PackDepth::VSM] //m_progPack
						, m_programs.m_colorLighting[SmType::Cascade][DepthImpl::Linear][SmImpl::VSM] //m_progDraw
					},
					{ //SmImpl::ESM
						13.0f, 7.0f, 13.0f, 1.0f         // m_sizePwrTwo
						, 1.0f, 1.0f, 20.0f, 1.0f          // m_depthValuePow
						, 1.0f, 1.0f, 10.0f, 0.01f         // m_near
						, 550.0f, 100.0f, 2000.0f, 50.0f   // m_far
						, 0.004f, 0.0f, 0.01f, 0.00001f    // m_bias
						, 0.001f, 0.0f, 0.04f, 0.00001f    // m_normalOffset
						, 0.7f, 0.0f, 1.0f, 0.01f          // m_customParam0
						, 9500.0f, 1.0f, 15000.0f, 1.0f    // m_customParam1
						, 2.0f, 0.0f, 4.0f, 1.0f           // m_xNum
						, 2.0f, 0.0f, 4.0f, 1.0f           // m_yNum
						, 0.2f, 0.0f, 1.0f, 0.01f          // m_xOffset
						, 0.2f, 0.0f, 1.0f, 0.01f          // m_yOffset
						, true                             // m_doBlur
						, m_programs.m_packDepth[DepthImpl::Linear][PackDepth::RGBA] //m_progPack
						, m_programs.m_packDepthSk[DepthImpl::Linear][PackDepth::RGBA] //m_progPack
						, m_programs.m_colorLighting[SmType::Cascade][DepthImpl::Linear][SmImpl::ESM] //m_progDraw
					}

				}
			}
		};
		bx::memCopy(m_smSettings, smSettings, sizeof(smSettings));

		m_settings.m_lightType = LightType::DirectionalLight;
		m_settings.m_depthImpl = DepthImpl::InvZ;
		m_settings.m_smImpl = SmImpl::PCF;
		m_settings.m_spotOuterAngle = 45.0f;
		m_settings.m_spotInnerAngle = 30.0f;
		m_settings.m_fovXAdjust = 0.0f;
		m_settings.m_fovYAdjust = 0.0f;
		m_settings.m_coverageSpotL = 90.0f;
		m_settings.m_splitDistribution = 0.75f;
		m_settings.m_numSplits = 4;
		m_settings.m_updateLights = true;
		m_settings.m_updateScene = true;
		m_settings.m_drawDepthBuffer = false;
		m_settings.m_showSmCoverage = false;
		m_settings.m_stencilPack = true;
		m_settings.m_stabilize = true;

		ShadowMapSettings* currentSmSettings = &m_smSettings[m_settings.m_lightType][m_settings.m_depthImpl][m_settings.m_smImpl];

		// Render targets.
		uint16_t shadowMapSize = 1 << uint32_t(currentSmSettings->m_sizePwrTwo);
		m_currentShadowMapSize = shadowMapSize;
		float currentShadowMapSizef = float(int16_t(m_currentShadowMapSize));
		m_uniforms.m_shadowMapTexelSize = 1.0f / currentShadowMapSizef;
		for (uint8_t ii = 0; ii < ShadowMapRenderTargets::Count; ++ii)
		{
			bgfx::TextureHandle fbtextures[] =
			{
				bgfx::createTexture2D(m_currentShadowMapSize, m_currentShadowMapSize, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT),
				bgfx::createTexture2D(m_currentShadowMapSize, m_currentShadowMapSize, false, 1, bgfx::TextureFormat::D32F,  BGFX_TEXTURE_RT),
			};
			m_rtShadowMap[ii] = bgfx::createFrameBuffer(BX_COUNTOF(fbtextures), fbtextures, true);
		}
		s_rtBlur = bgfx::createFrameBuffer(m_currentShadowMapSize, m_currentShadowMapSize, bgfx::TextureFormat::BGRA8);
		
		m_timeAccumulatorLight = 0.0f;
		m_timeAccumulatorScene = 0.0f;
	}

	void CBgfxShadowCsm::update(uint8_t viewId, glm::vec3& lightDir, uint16_t width, uint16_t height)
	{
			m_viewState.m_width = width;
			m_viewState.m_height = height;

			const bgfx::Caps* caps = bgfx::getCaps();

			da::core::CCamera* cam = da::core::CCamera::getCamera();

			// Set view and projection matrices.
			const float camFovy = cam->fov;
			const float camAspect = float(int32_t(m_viewState.m_width)) / float(int32_t(m_viewState.m_height));
			const float camNear = cam->zNear;
			const float camFar = cam->zFar;
			const float projHeight = bx::tan(bx::toRad(camFovy) * 0.5f);
			const float projWidth = projHeight * camAspect;
			bx::mtxProj(m_viewState.m_proj, camFovy, camAspect, camNear, camFar, caps->homogeneousDepth, bx::Handedness::Left);

			bx::Vec3 pos(cam->position().x, cam->position().y, cam->position().z);
			glm::vec3 _at = cam->position() + cam->forward();
			bx::Vec3 at(_at.x, _at.y, _at.z);
			bx::Vec3 up(0.f, 0.f, cam->up().z);
			bx::mtxLookAt(m_viewState.m_view, pos, at, up, ::bx::Handedness::Left);

			float currentShadowMapSizef = float(int16_t(m_currentShadowMapSize));
			m_uniforms.m_shadowMapTexelSize = 1.0f / currentShadowMapSizef;

			m_uniforms.submitConstUniforms();

			ShadowMapSettings* currentSmSettings = &m_smSettings[m_settings.m_lightType][m_settings.m_depthImpl][m_settings.m_smImpl];

#ifdef DA_REVIEW
			if (m_showDebug) 
			{

				ImGui::SetNextWindowPos(
					ImVec2(m_viewState.m_width - m_viewState.m_width / 5.0f - 10.0f, 10.0f)
					, ImGuiCond_FirstUseEver
				);
				ImGui::SetNextWindowSize(
					ImVec2(m_viewState.m_width / 5.0f, m_viewState.m_height - 20.0f)
					, ImGuiCond_FirstUseEver
				);
				ImGui::Begin("Settings"
					, &m_showDebug
					, 0
				);

				#define IMGUI_FLOAT_SLIDER(_name, _val) \
				ImGui::SliderFloat(_name            \
						, &_val                     \
						, *( ((float*)&_val)+1)     \
						, *( ((float*)&_val)+2)     \
						)

				#define IMGUI_RADIO_BUTTON(_name, _var, _val)     \
				if (ImGui::RadioButton(_name, _var == _val) ) \
				{                                             \
					_var = _val;                              \
				}

				ImGui::Checkbox("Update lights", &m_settings.m_updateLights);
				ImGui::Checkbox("Update scene", &m_settings.m_updateScene);

				ImGui::Separator();
				ImGui::Text("Shadow map depth:");
				IMGUI_RADIO_BUTTON("InvZ", m_settings.m_depthImpl, DepthImpl::InvZ);
				IMGUI_RADIO_BUTTON("Linear", m_settings.m_depthImpl, DepthImpl::Linear);

				ImGui::Separator();
				ImGui::Checkbox("Draw depth buffer", &m_settings.m_drawDepthBuffer);
				if (m_settings.m_drawDepthBuffer)
				{
					IMGUI_FLOAT_SLIDER("Depth value pow", currentSmSettings->m_depthValuePow);
				}

				ImGui::Separator();
				ImGui::Text("Shadow Map implementation");
				IMGUI_RADIO_BUTTON("Hard", m_settings.m_smImpl, SmImpl::Hard);
				IMGUI_RADIO_BUTTON("PCF", m_settings.m_smImpl, SmImpl::PCF);
				IMGUI_RADIO_BUTTON("VSM", m_settings.m_smImpl, SmImpl::VSM);
				IMGUI_RADIO_BUTTON("ESM", m_settings.m_smImpl, SmImpl::ESM);
				

				ImGui::Separator();
				IMGUI_FLOAT_SLIDER("Bias", currentSmSettings->m_bias);
				IMGUI_FLOAT_SLIDER("Normal offset", currentSmSettings->m_normalOffset);
				ImGui::Separator();
				if (LightType::DirectionalLight != m_settings.m_lightType)
				{
					IMGUI_FLOAT_SLIDER("Near plane", currentSmSettings->m_near);
				}
				IMGUI_FLOAT_SLIDER("Far plane", currentSmSettings->m_far);

				ImGui::Separator();
				switch (m_settings.m_smImpl)
				{
				case SmImpl::Hard:
					//ImGui::Text("Hard");
					break;

				case SmImpl::PCF:
					ImGui::Text("PCF");
					IMGUI_FLOAT_SLIDER("X Offset", currentSmSettings->m_xOffset);
					IMGUI_FLOAT_SLIDER("Y Offset", currentSmSettings->m_yOffset);
					break;

				case SmImpl::VSM:
					ImGui::Text("VSM");
					IMGUI_FLOAT_SLIDER("Min variance", currentSmSettings->m_customParam0);
					IMGUI_FLOAT_SLIDER("Depth multiplier", currentSmSettings->m_customParam1);
					ImGui::Checkbox("Blur shadow map", &currentSmSettings->m_doBlur);
					if (currentSmSettings->m_doBlur)
					{
						IMGUI_FLOAT_SLIDER("Blur X Offset", currentSmSettings->m_xOffset);
						IMGUI_FLOAT_SLIDER("Blur Y Offset", currentSmSettings->m_yOffset);
					}
					break;

				case SmImpl::ESM:
					ImGui::Text("ESM");
					IMGUI_FLOAT_SLIDER("ESM Hardness", currentSmSettings->m_customParam0);
					IMGUI_FLOAT_SLIDER("Depth multiplier", currentSmSettings->m_customParam1);
					ImGui::Checkbox("Blur shadow map", &currentSmSettings->m_doBlur);
					if (currentSmSettings->m_doBlur)
					{
						IMGUI_FLOAT_SLIDER("Blur X Offset", currentSmSettings->m_xOffset);
						IMGUI_FLOAT_SLIDER("Blur Y Offset", currentSmSettings->m_yOffset);
					}
					break;

				default:
					break;
				};

				ImGui::End();
#undef IMGUI_RADIO_BUTTON

				ImGui::SetNextWindowPos(
					ImVec2(10.0f, 260.0f)
					, ImGuiCond_FirstUseEver
				);
				ImGui::SetNextWindowSize(
					ImVec2(m_viewState.m_width / 5.0f, 350.0f)
					, ImGuiCond_FirstUseEver
				);
				ImGui::Begin("Light"
					, &m_showDebug
					, 0
				);
				ImGui::PushItemWidth(185.0f);

				bool bLtChanged = false;
				if (ImGui::RadioButton("Directional light", m_settings.m_lightType == LightType::DirectionalLight))
				{
					m_settings.m_lightType = LightType::DirectionalLight; bLtChanged = true;
				}

				ImGui::Separator();
				ImGui::Checkbox("Show shadow map coverage.", &m_settings.m_showSmCoverage);

				ImGui::Separator();
				ImGui::Text("Shadow map resolution: %ux%u", m_currentShadowMapSize, m_currentShadowMapSize);
				ImGui::SliderFloat("##SizePwrTwo", &currentSmSettings->m_sizePwrTwo,
					currentSmSettings->m_sizePwrTwoMin,
					currentSmSettings->m_sizePwrTwoMax, "%.0f");

				ImGui::Separator();
				if (LightType::DirectionalLight == m_settings.m_lightType)
				{
					ImGui::Text("Directional light");
					ImGui::Checkbox("Stabilize cascades", &m_settings.m_stabilize);
					ImGui::SliderInt("Cascade splits", &m_settings.m_numSplits, 1, 4);
					ImGui::SliderFloat("Cascade distribution", &m_settings.m_splitDistribution, 0.0f, 1.0f);
				}

				ImGui::End();

#undef IMGUI_FLOAT_SLIDER
			}
#endif

			// Update uniforms.
			m_uniforms.m_shadowMapBias = currentSmSettings->m_bias;
			m_uniforms.m_shadowMapOffset = currentSmSettings->m_normalOffset;
			m_uniforms.m_shadowMapParam0 = currentSmSettings->m_customParam0;
			m_uniforms.m_shadowMapParam1 = currentSmSettings->m_customParam1;
			m_uniforms.m_depthValuePow = currentSmSettings->m_depthValuePow;
			m_uniforms.m_XNum = currentSmSettings->m_xNum;
			m_uniforms.m_YNum = currentSmSettings->m_yNum;
			m_uniforms.m_XOffset = currentSmSettings->m_xOffset;
			m_uniforms.m_YOffset = currentSmSettings->m_yOffset;
			m_uniforms.m_showSmCoverage = float(m_settings.m_showSmCoverage);

			m_uniforms.submitPerFrameUniforms();

			// Time.
			int64_t now = bx::getHPCounter();
			static int64_t last = now;
			const int64_t frameTime = now - last;
			last = now;
			const double freq = double(bx::getHPFrequency());
			const float deltaTime = float(frameTime / freq);

			// Update time accumulators.
			if (m_settings.m_updateLights) { m_timeAccumulatorLight += deltaTime; }
			if (m_settings.m_updateScene) { m_timeAccumulatorScene += deltaTime; }

			// Compute transform matrices.
			const uint8_t shadowMapPasses = ShadowMapRenderTargets::Count;
			float lightView[shadowMapPasses][16];
			float lightProj[shadowMapPasses][16];
			float mtxYpr[TetrahedronFaces::Count][16];

			float screenProj[16];
			float screenView[16];
			bx::mtxIdentity(screenView);

			bx::mtxOrtho(
				screenProj
				, 0.0f
				, 1.0f
				, 1.0f
				, 0.0f
				, 0.0f
				, 100.0f
				, 0.0f
				, caps->homogeneousDepth
			);

			// Update render target size.
			uint16_t shadowMapSize = 1 << uint32_t(currentSmSettings->m_sizePwrTwo);
			if (m_currentShadowMapSize != shadowMapSize)
			{
				m_currentShadowMapSize = shadowMapSize;
				m_uniforms.m_shadowMapTexelSize = 1.0f / currentShadowMapSizef;

				{
					bgfx::destroy(m_rtShadowMap[0]);

					bgfx::TextureHandle fbtextures[] =
					{
						bgfx::createTexture2D(m_currentShadowMapSize, m_currentShadowMapSize, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT),
						bgfx::createTexture2D(m_currentShadowMapSize, m_currentShadowMapSize, false, 1, bgfx::TextureFormat::D24S8, BGFX_TEXTURE_RT),
					};
					m_rtShadowMap[0] = bgfx::createFrameBuffer(BX_COUNTOF(fbtextures), fbtextures, true);
				}

				if (LightType::DirectionalLight == m_settings.m_lightType)
				{
					for (uint8_t ii = 1; ii < ShadowMapRenderTargets::Count; ++ii)
					{
						{
							bgfx::destroy(m_rtShadowMap[ii]);

							bgfx::TextureHandle fbtextures[] =
							{
								bgfx::createTexture2D(m_currentShadowMapSize, m_currentShadowMapSize, false, 1, bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT),
								bgfx::createTexture2D(m_currentShadowMapSize, m_currentShadowMapSize, false, 1, bgfx::TextureFormat::D24S8, BGFX_TEXTURE_RT),
							};
							m_rtShadowMap[ii] = bgfx::createFrameBuffer(BX_COUNTOF(fbtextures), fbtextures, true);
						}
					}
				}

				bgfx::destroy(s_rtBlur);
				s_rtBlur = bgfx::createFrameBuffer(m_currentShadowMapSize, m_currentShadowMapSize, bgfx::TextureFormat::BGRA8);
			}

			// LightType::DirectionalLight == settings.m_lightType
			{
				// Ensure light direction is normalized
				glm::vec3 lightDirection = glm::normalize(lightDir);

				// Compute right vector (perpendicular to light direction and up vector)
				glm::vec3 right = glm::normalize(glm::cross(glm::vec3(0.f,0.f,1.f), -lightDirection));

				// Compute up vector (perpendicular to right and light direction)
				glm::vec3 up = glm::cross(-lightDirection, right);

				// Create the view matrix
				glm::mat4 viewMatrix = glm::mat4(1.0f); // Identity matrix
				viewMatrix[0][0] = -right.x;
				viewMatrix[1][0] = -right.y;
				viewMatrix[2][0] = -right.z;
				viewMatrix[0][1] = up.x;
				viewMatrix[1][1] = up.y;
				viewMatrix[2][1] = up.z;
				viewMatrix[0][2] = -lightDirection.x;
				viewMatrix[1][2] = -lightDirection.y;
				viewMatrix[2][2] = -lightDirection.z;

				memcpy(&lightView[0], &viewMatrix[0], sizeof(viewMatrix));

				// Compute camera inverse view mtx.
				float mtxViewInv[16];
				bx::mtxInverse(mtxViewInv, m_viewState.m_view);

				// Compute split distances.
				const uint8_t maxNumSplits = 4;
				BX_ASSERT(maxNumSplits >= m_settings.m_numSplits, "Error! Max num splits.");

				float splitSlices[maxNumSplits * 2];
				splitFrustum(splitSlices
					, uint8_t(m_settings.m_numSplits)
					, currentSmSettings->m_near
					, currentSmSettings->m_far
					, m_settings.m_splitDistribution
				);

				// Update uniforms.
				for (uint8_t ii = 0, ff = 1; ii < m_settings.m_numSplits; ++ii, ff += 2)
				{
					// This lags for 1 frame, but it's not a problem.
					m_uniforms.m_csmFarDistances[ii] = splitSlices[ff];
				}

				const uint8_t numCorners = 8;
				float frustumCorners[maxNumSplits][numCorners][3];
				for (uint8_t ii = 0, nn = 0, ff = 1; ii < m_settings.m_numSplits; ++ii, nn += 2, ff += 2)
				{
					// Compute frustum corners for one split in world space.
					worldSpaceFrustumCorners((float*)frustumCorners[ii], splitSlices[nn], splitSlices[ff], projWidth, projHeight, mtxViewInv);

					bx::Vec3 min = { 9000.0f,  9000.0f,  9000.0f };
					bx::Vec3 max = { -9000.0f, -9000.0f, -9000.0f };

					for (uint8_t jj = 0; jj < numCorners; ++jj)
					{
						// Transform to light space.
						const bx::Vec3 xyz = bx::mul(bx::load<bx::Vec3>(frustumCorners[ii][jj]), lightView[0]);

						// Update bounding box.
						min = bx::min(min, xyz);
						max = bx::max(max, xyz);
					}

					float mtxProj[16];
					bx::mtxOrtho(
						mtxProj
						, 1.0f
						, -1.0f
						, 1.0f
						, -1.0f
						, min.z + -splitSlices[ff] * (ii == 0 ? 1.25f : .5f)
						, max.z
						, 0.0f
						, caps->homogeneousDepth
						, bx::Handedness::Left
					);
					

					const bx::Vec3 minproj = bx::mulH(min, mtxProj);
					const bx::Vec3 maxproj = bx::mulH(max, mtxProj);

					float scalex = 2.0f / (maxproj.x - minproj.x);
					float scaley = 2.0f / (maxproj.y - minproj.y);

					if (m_settings.m_stabilize)
					{
						const float quantizer = 64.0f;
						scalex = quantizer / bx::ceil(quantizer / scalex);
						scaley = quantizer / bx::ceil(quantizer / scaley);
					}

					float offsetx = 0.5f * (maxproj.x + minproj.x) * scalex;
					float offsety = 0.5f * (maxproj.y + minproj.y) * scaley;

					if (m_settings.m_stabilize)
					{
						const float halfSize = currentShadowMapSizef * 0.5f;
						offsetx = bx::ceil(offsetx * halfSize) / halfSize;
						offsety = bx::ceil(offsety * halfSize) / halfSize;
					}

					float mtxCrop[16];
					bx::mtxIdentity(mtxCrop);
					mtxCrop[0] = scalex;
					mtxCrop[5] = scaley;
					mtxCrop[12] = offsetx;
					mtxCrop[13] = offsety;

					bx::mtxMul(lightProj[ii], mtxCrop, mtxProj);
				}
			}

		
			
			// Determine on-screen rectangle size where depth buffer will be drawn.
			uint16_t depthRectHeight = uint16_t(float(m_viewState.m_height) / 2.5f);
			uint16_t depthRectWidth = depthRectHeight;
			uint16_t depthRectX = 0;
			uint16_t depthRectY = m_viewState.m_height - depthRectHeight;

			// LightType::DirectionalLight == settings.m_lightType
			{
				/**
				 * RENDERVIEW_SHADOWMAP_1_ID - Craft shadow map for first  split.
				 * RENDERVIEW_SHADOWMAP_2_ID - Craft shadow map for second split.
				 * RENDERVIEW_SHADOWMAP_3_ID - Craft shadow map for third  split.
				 * RENDERVIEW_SHADOWMAP_4_ID - Craft shadow map for fourth split.
				 * RENDERVIEW_VBLUR_0_ID - Vertical   blur for first  split.
				 * RENDERVIEW_HBLUR_0_ID - Horizontal blur for first  split.
				 * RENDERVIEW_VBLUR_1_ID - Vertical   blur for second split.
				 * RENDERVIEW_HBLUR_1_ID - Horizontal blur for second split.
				 * RENDERVIEW_VBLUR_2_ID - Vertical   blur for third  split.
				 * RENDERVIEW_HBLUR_2_ID - Horizontal blur for third  split.
				 * RENDERVIEW_VBLUR_3_ID - Vertical   blur for fourth split.
				 * RENDERVIEW_HBLUR_3_ID - Horizontal blur for fourth split.
				**/

				depthRectHeight = m_viewState.m_height / 3;
				depthRectWidth = depthRectHeight;
				depthRectX = 0;
				depthRectY = m_viewState.m_height - depthRectHeight;

				bgfx::setViewRect(RENDERVIEW_SHADOWMAP_1_ID, 0, 0, m_currentShadowMapSize, m_currentShadowMapSize);
				bgfx::setViewName(RENDERVIEW_SHADOWMAP_1_ID, "ShadowMap 1");
				bgfx::setViewRect(RENDERVIEW_SHADOWMAP_2_ID, 0, 0, m_currentShadowMapSize, m_currentShadowMapSize);
				bgfx::setViewName(RENDERVIEW_SHADOWMAP_2_ID, "ShadowMap 2");
				bgfx::setViewRect(RENDERVIEW_SHADOWMAP_3_ID, 0, 0, m_currentShadowMapSize, m_currentShadowMapSize);
				bgfx::setViewName(RENDERVIEW_SHADOWMAP_3_ID, "ShadowMap 3");
				bgfx::setViewRect(RENDERVIEW_SHADOWMAP_4_ID, 0, 0, m_currentShadowMapSize, m_currentShadowMapSize);
				bgfx::setViewName(RENDERVIEW_SHADOWMAP_4_ID, "ShadowMap 4");
				bgfx::setViewRect(RENDERVIEW_VBLUR_0_ID, 0, 0, m_currentShadowMapSize, m_currentShadowMapSize);
				bgfx::setViewName(RENDERVIEW_VBLUR_0_ID, "RENDERVIEW_VBLUR 1");
				bgfx::setViewRect(RENDERVIEW_HBLUR_0_ID, 0, 0, m_currentShadowMapSize, m_currentShadowMapSize);
				bgfx::setViewName(RENDERVIEW_HBLUR_0_ID, "RENDERVIEW_HBLUR 1");
				bgfx::setViewRect(RENDERVIEW_VBLUR_1_ID, 0, 0, m_currentShadowMapSize, m_currentShadowMapSize);
				bgfx::setViewName(RENDERVIEW_VBLUR_1_ID, "RENDERVIEW_VBLUR 2");
				bgfx::setViewRect(RENDERVIEW_HBLUR_1_ID, 0, 0, m_currentShadowMapSize, m_currentShadowMapSize);
				bgfx::setViewName(RENDERVIEW_HBLUR_1_ID, "RENDERVIEW_HBLUR 2");
				bgfx::setViewRect(RENDERVIEW_VBLUR_2_ID, 0, 0, m_currentShadowMapSize, m_currentShadowMapSize);
				bgfx::setViewName(RENDERVIEW_VBLUR_2_ID, "RENDERVIEW_VBLUR 3");
				bgfx::setViewRect(RENDERVIEW_HBLUR_2_ID, 0, 0, m_currentShadowMapSize, m_currentShadowMapSize);
				bgfx::setViewName(RENDERVIEW_HBLUR_2_ID, "RENDERVIEW_HBLUR 3");
				bgfx::setViewRect(RENDERVIEW_VBLUR_3_ID, 0, 0, m_currentShadowMapSize, m_currentShadowMapSize);
				bgfx::setViewName(RENDERVIEW_VBLUR_3_ID, "RENDERVIEW_VBLUR 4");
				bgfx::setViewRect(RENDERVIEW_HBLUR_3_ID, 0, 0, m_currentShadowMapSize, m_currentShadowMapSize);
				bgfx::setViewName(RENDERVIEW_HBLUR_3_ID, "RENDERVIEW_HBLUR 4");

				bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_1_ID, lightView[0], lightProj[0]);
				bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_2_ID, lightView[0], lightProj[1]);
				bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_3_ID, lightView[0], lightProj[2]);
				bgfx::setViewTransform(RENDERVIEW_SHADOWMAP_4_ID, lightView[0], lightProj[3]);
				bgfx::setViewTransform(RENDERVIEW_VBLUR_0_ID, screenView, screenProj);
				bgfx::setViewTransform(RENDERVIEW_HBLUR_0_ID, screenView, screenProj);
				bgfx::setViewTransform(RENDERVIEW_VBLUR_1_ID, screenView, screenProj);
				bgfx::setViewTransform(RENDERVIEW_HBLUR_1_ID, screenView, screenProj);
				bgfx::setViewTransform(RENDERVIEW_VBLUR_2_ID, screenView, screenProj);
				bgfx::setViewTransform(RENDERVIEW_HBLUR_2_ID, screenView, screenProj);
				bgfx::setViewTransform(RENDERVIEW_VBLUR_3_ID, screenView, screenProj);
				bgfx::setViewTransform(RENDERVIEW_HBLUR_3_ID, screenView, screenProj);

				bgfx::setViewFrameBuffer(RENDERVIEW_SHADOWMAP_1_ID, m_rtShadowMap[0]);
				bgfx::setViewFrameBuffer(RENDERVIEW_SHADOWMAP_2_ID, m_rtShadowMap[1]);
				bgfx::setViewFrameBuffer(RENDERVIEW_SHADOWMAP_3_ID, m_rtShadowMap[2]);
				bgfx::setViewFrameBuffer(RENDERVIEW_SHADOWMAP_4_ID, m_rtShadowMap[3]);
				bgfx::setViewFrameBuffer(RENDERVIEW_VBLUR_0_ID, s_rtBlur);         //vblur
				bgfx::setViewFrameBuffer(RENDERVIEW_HBLUR_0_ID, m_rtShadowMap[0]); //hblur
				bgfx::setViewFrameBuffer(RENDERVIEW_VBLUR_1_ID, s_rtBlur);         //vblur
				bgfx::setViewFrameBuffer(RENDERVIEW_HBLUR_1_ID, m_rtShadowMap[1]); //hblur
				bgfx::setViewFrameBuffer(RENDERVIEW_VBLUR_2_ID, s_rtBlur);         //vblur
				bgfx::setViewFrameBuffer(RENDERVIEW_HBLUR_2_ID, m_rtShadowMap[2]); //hblur
				bgfx::setViewFrameBuffer(RENDERVIEW_VBLUR_3_ID, s_rtBlur);         //vblur
				bgfx::setViewFrameBuffer(RENDERVIEW_HBLUR_3_ID, m_rtShadowMap[3]); //hblur
			}

			// Clear backbuffer at beginning.
			/*bgfx::setViewClear(0
				, BGFX_CLEAR_COLOR
				| BGFX_CLEAR_DEPTH
				, m_clearValues.m_clearRgba
				, m_clearValues.m_clearDepth
				, m_clearValues.m_clearStencil
			);
			bgfx::touch(0);*/

			// Clear shadowmap rendertarget at beginning.
			const uint8_t flags0 = (LightType::DirectionalLight == m_settings.m_lightType)
				? 0
				: BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH | BGFX_CLEAR_STENCIL
				;

			const uint8_t flags1 = (LightType::DirectionalLight == m_settings.m_lightType)
				? BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
				: 0
				;

			for (uint8_t ii = 0; ii < 4; ++ii)
			{
				bgfx::setViewClear(RENDERVIEW_SHADOWMAP_1_ID + ii
					, flags1
					, 0xfefefefe //blur fails on completely white regions
					, m_clearValues.m_clearDepth
					, m_clearValues.m_clearStencil
				);
				bgfx::touch(RENDERVIEW_SHADOWMAP_1_ID + ii);
			}

			// Render.

			// Craft shadow map.
			{
				// Draw scene into shadowmap.
				uint8_t drawNum;
				{
					drawNum = uint8_t(m_settings.m_numSplits);
				}

				for (uint8_t ii = 0; ii < drawNum; ++ii)
				{
					const uint8_t shadowViewId = RENDERVIEW_SHADOWMAP_1_ID + ii;

					uint8_t renderStateIndex = RenderState::ShadowMap_PackDepth;

					m_renderFunc(shadowViewId, currentSmSettings->m_progPack, currentSmSettings->m_progPackInstance, currentSmSettings->m_progPackSk, s_renderStates[renderStateIndex]);
				}
			}

			PackDepth::Enum depthType = (SmImpl::VSM == m_settings.m_smImpl) ? PackDepth::VSM : PackDepth::RGBA;
			bool bVsmOrEsm = (SmImpl::VSM == m_settings.m_smImpl) || (SmImpl::ESM == m_settings.m_smImpl);

			// Blur shadow map.
			if (bVsmOrEsm
				&& currentSmSettings->m_doBlur)
			{
				bgfx::setTexture(4, m_shadowMap[0], bgfx::getTexture(m_rtShadowMap[0]));
				bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
				screenSpaceQuad(s_originBottomLeft);
				bgfx::submit(RENDERVIEW_VBLUR_0_ID, { m_programs.m_vBlur[depthType]->getHandle() });

				bgfx::setTexture(4, m_shadowMap[0], bgfx::getTexture(s_rtBlur));
				bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
				screenSpaceQuad(s_originBottomLeft);
				bgfx::submit(RENDERVIEW_HBLUR_0_ID, { m_programs.m_hBlur[depthType]->getHandle() });

				if (LightType::DirectionalLight == m_settings.m_lightType)
				{
					for (uint8_t ii = 1, jj = 2; ii < m_settings.m_numSplits; ++ii, jj += 2)
					{
						const uint8_t renderViewId = RENDERVIEW_VBLUR_0_ID + jj;

						bgfx::setTexture(4, m_shadowMap[0], bgfx::getTexture(m_rtShadowMap[ii]));
						bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
						screenSpaceQuad(s_originBottomLeft);
						bgfx::submit(renderViewId, { m_programs.m_vBlur[depthType]->getHandle() });

						bgfx::setTexture(4, m_shadowMap[0], bgfx::getTexture(s_rtBlur));
						bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A);
						screenSpaceQuad(s_originBottomLeft);
						bgfx::submit(renderViewId + 1, { m_programs.m_hBlur[depthType]->getHandle()});
					}
				}
			}

			// Draw scene.
			{
				// Setup shadow mtx.
				float mtxShadow[16];

				const float ymul = (s_originBottomLeft) ? 0.5f : -0.5f;
				float zadd = (DepthImpl::Linear == m_settings.m_depthImpl) ? 0.0f : 0.5f;

				const float mtxBias[16] =
				{
					0.5f, 0.0f, 0.0f, 0.0f,
					0.0f, ymul, 0.0f, 0.0f,
					0.0f, 0.0f, 0.5f, 0.0f,
					0.5f, 0.5f, zadd, 1.0f,
				};

				
				//LightType::DirectionalLight == settings.m_lightType
				{
					for (uint8_t ii = 0; ii < m_settings.m_numSplits; ++ii)
					{
						float mtxTmp[16];

						bx::mtxMul(mtxTmp, lightProj[ii], mtxBias);
						bx::mtxMul(m_shadowMapMtx[ii], lightView[0], mtxTmp); //lViewProjCropBias
					}
				}
			}
	}

	void CBgfxShadowCsm::shutdown()
	{
		for (uint8_t ii = 0; ii < ShadowMapRenderTargets::Count; ++ii)
		{
			bgfx::destroy(m_rtShadowMap[ii]);
		}
		bgfx::destroy(s_rtBlur);

		m_programs.destroy();

		bgfx::destroy(s_texColor);
		bgfx::destroy(m_shadowMap[3]);
		bgfx::destroy(m_shadowMap[2]);
		bgfx::destroy(m_shadowMap[1]);
		bgfx::destroy(m_shadowMap[0]);

		m_uniforms.destroy();

#ifdef DA_REVIEW
		da::debug::CDebugMenuBar::unregister_debug(HASHSTR("Renderer"), HASHSTR("Shadows"));
#endif

	}

	void CBgfxShadowCsm::setRenderFunc(const std::function<void(uint8_t, da::graphics::CMaterial*, da::graphics::CMaterial*, da::graphics::CMaterial*, RenderState)>& func)
	{
		m_renderFunc = func;
	}

	void CBgfxShadowCsm::submitUniforms()
	{
		//m_uniforms.submitConstUniforms();
		m_uniforms.submitPerDrawUniforms();
		//m_uniforms.submitPerFrameUniforms();

		for (uint8_t ii = 0; ii < ShadowMapRenderTargets::Count; ++ii)
		{
			bgfx::setTexture(CBgfxSamplers::SAMPLER_SHADOW_MAP_NEAR + ii, m_shadowMap[ii], bgfx::getTexture(m_rtShadowMap[ii]));
		}
	}

	void CBgfxShadowCsm::FPrograms::init()
	{
		// Misc.
		m_black = da::graphics::CMaterialFactory::create("shaders/csm/vs_shadowMaps_color.sc", "shaders/csm/fs_shadowMaps_color_black.sc");
		m_texture = da::graphics::CMaterialFactory::create("shaders/csm/vs_shadowMaps_texture.sc", "shaders/csm/fs_shadowMaps_texture.sc");
		m_colorTexture = da::graphics::CMaterialFactory::create("shaders/csm/vs_shadowMaps_color_texture.sc", "shaders/csm/fs_shadowMaps_color_texture.sc");

		// Blur.
		m_vBlur[PackDepth::RGBA] = da::graphics::CMaterialFactory::create("shaders/csm/vs_shadowMaps_vblur.sc", "shaders/csm/fs_shadowMaps_vblur.sc");
		m_hBlur[PackDepth::RGBA] = da::graphics::CMaterialFactory::create("shaders/csm/vs_shadowMaps_hblur.sc", "shaders/csm/fs_shadowMaps_hblur.sc");
		m_vBlur[PackDepth::VSM] = da::graphics::CMaterialFactory::create("shaders/csm/vs_shadowMaps_vblur.sc", "shaders/csm/fs_shadowMaps_vblur_vsm.sc");
		m_hBlur[PackDepth::VSM] = da::graphics::CMaterialFactory::create("shaders/csm/vs_shadowMaps_hblur.sc", "shaders/csm/fs_shadowMaps_hblur_vsm.sc");

		// Draw depth.
		m_drawDepth[PackDepth::RGBA] = da::graphics::CMaterialFactory::create("shaders/csm/vs_shadowMaps_unpackdepth.sc", "shaders/csm/fs_shadowMaps_unpackdepth.sc");
		m_drawDepth[PackDepth::VSM] =  da::graphics::CMaterialFactory::create("shaders/csm/vs_shadowMaps_unpackdepth.sc", "shaders/csm/fs_shadowMaps_unpackdepth_vsm.sc");

		// Pack depth.
		m_packDepth[DepthImpl::InvZ][PackDepth::RGBA] = da::graphics::CMaterialFactory::create("shaders/csm/vs_shadowMaps_packdepth.sc", "shaders/csm/fs_shadowMaps_packdepth.sc");
		m_packDepth[DepthImpl::InvZ][PackDepth::VSM] = da::graphics::CMaterialFactory::create("shaders/csm/vs_shadowMaps_packdepth.sc", "shaders/csm/fs_shadowMaps_packdepth_vsm.sc");

		m_packDepthSk[DepthImpl::InvZ][PackDepth::RGBA] = da::graphics::CMaterialFactory::create("shaders/csm/vs_shadowMaps_sk_packdepth.sc", "shaders/csm/fs_shadowMaps_packdepth.sc");
		m_packDepthSk[DepthImpl::InvZ][PackDepth::VSM] = da::graphics::CMaterialFactory::create("shaders/csm/vs_shadowMaps_sk_packdepth.sc", "shaders/csm/fs_shadowMaps_packdepth_vsm.sc");
		
		m_packDepthInstance[DepthImpl::InvZ][PackDepth::RGBA] = da::graphics::CMaterialFactory::create("shaders/csm/vs_shadowMaps_instance_packdepth.sc", "shaders/csm/fs_shadowMaps_packdepth.sc");
		m_packDepthInstance[DepthImpl::InvZ][PackDepth::VSM] = da::graphics::CMaterialFactory::create("shaders/csm/vs_shadowMaps_instance_packdepth.sc", "shaders/csm/fs_shadowMaps_packdepth_vsm.sc");

		m_packDepth[DepthImpl::Linear][PackDepth::RGBA] = da::graphics::CMaterialFactory::create("shaders/csm/vs_shadowMaps_packdepth_linear.sc", "shaders/csm/fs_shadowMaps_packdepth_linear.sc");
		m_packDepth[DepthImpl::Linear][PackDepth::VSM] = da::graphics::CMaterialFactory::create("shaders/csm/vs_shadowMaps_packdepth_linear.sc", "shaders/csm/fs_shadowMaps_packdepth_vsm_linear.sc");

		m_packDepthSk[DepthImpl::Linear][PackDepth::RGBA] = da::graphics::CMaterialFactory::create("shaders/csm/vs_shadowMaps_packdepth_linear.sc", "shaders/csm/fs_shadowMaps_packdepth_linear.sc");
		m_packDepthSk[DepthImpl::Linear][PackDepth::VSM] = da::graphics::CMaterialFactory::create("shaders/csm/vs_shadowMaps_packdepth_linear.sc", "shaders/csm/fs_shadowMaps_packdepth_vsm_linear.sc");

		m_packDepthInstance[DepthImpl::Linear][PackDepth::RGBA] = da::graphics::CMaterialFactory::create("shaders/csm/vs_shadowMaps_packdepth_linear.sc", "shaders/csm/fs_shadowMaps_packdepth_linear.sc");
		m_packDepthInstance[DepthImpl::Linear][PackDepth::VSM] = da::graphics::CMaterialFactory::create("shaders/csm/vs_shadowMaps_packdepth_linear.sc", "shaders/csm/fs_shadowMaps_packdepth_vsm_linear.sc");
	}

	void CBgfxShadowCsm::FPrograms::destroy()
	{
		// Pack depth.
		for (uint8_t ii = 0; ii < DepthImpl::Count; ++ii)
		{
			for (uint8_t jj = 0; jj < PackDepth::Count; ++jj)
			{
				da::graphics::CMaterialFactory::remove(m_packDepth[ii][jj]);
				da::graphics::CMaterialFactory::remove(m_packDepthSk[ii][jj]);
				da::graphics::CMaterialFactory::remove(m_packDepthInstance[ii][jj]);
			}
		}

		// Draw depth.
		for (uint8_t ii = 0; ii < PackDepth::Count; ++ii)
		{
			da::graphics::CMaterialFactory::remove(m_drawDepth[ii]);
		}

		// Hblur.
		for (uint8_t ii = 0; ii < PackDepth::Count; ++ii)
		{
			da::graphics::CMaterialFactory::remove(m_hBlur[ii]);
		}

		// Vblur.
		for (uint8_t ii = 0; ii < PackDepth::Count; ++ii)
		{
			da::graphics::CMaterialFactory::remove(m_vBlur[ii]);
		}

		// Misc.
		da::graphics::CMaterialFactory::remove(m_colorTexture);
		da::graphics::CMaterialFactory::remove(m_texture);
		da::graphics::CMaterialFactory::remove(m_black);
	}

	void CBgfxShadowCsm::FUniforms::init()
	{
		m_ambientPass = 1.0f;
		m_lightingPass = 1.0f;

		m_shadowMapBias = 0.003f;
		m_shadowMapOffset = 0.0f;
		m_shadowMapParam0 = 0.5;
		m_shadowMapParam1 = 1.0;
		m_depthValuePow = 1.0f;
		m_showSmCoverage = 1.0f;
		m_shadowMapTexelSize = 1.0f / 512.0f;

		m_csmFarDistances[0] = 30.0f;
		m_csmFarDistances[1] = 90.0f;
		m_csmFarDistances[2] = 180.0f;
		m_csmFarDistances[3] = 1000.0f;

		m_tetraNormalGreen[0] = 0.0f;
		m_tetraNormalGreen[1] = -0.57735026f;
		m_tetraNormalGreen[2] = 0.81649661f;

		m_tetraNormalYellow[0] = 0.0f;
		m_tetraNormalYellow[1] = -0.57735026f;
		m_tetraNormalYellow[2] = -0.81649661f;

		m_tetraNormalBlue[0] = -0.81649661f;
		m_tetraNormalBlue[1] = 0.57735026f;
		m_tetraNormalBlue[2] = 0.0f;

		m_tetraNormalRed[0] = 0.81649661f;
		m_tetraNormalRed[1] = 0.57735026f;
		m_tetraNormalRed[2] = 0.0f;

		m_XNum = 2.0f;
		m_YNum = 2.0f;
		m_XOffset = 10.0f / 512.0f;
		m_YOffset = 10.0f / 512.0f;

		u_params0 = bgfx::createUniform("u_params0", bgfx::UniformType::Vec4);
		u_params1 = bgfx::createUniform("u_params1", bgfx::UniformType::Vec4);
		u_params2 = bgfx::createUniform("u_params2", bgfx::UniformType::Vec4);
		u_color = bgfx::createUniform("u_color", bgfx::UniformType::Vec4);
		u_smSamplingParams = bgfx::createUniform("u_smSamplingParams", bgfx::UniformType::Vec4);
		u_csmFarDistances = bgfx::createUniform("u_csmFarDistances", bgfx::UniformType::Vec4);
		u_lightMtx = bgfx::createUniform("u_lightMtx", bgfx::UniformType::Mat4);

		u_tetraNormalGreen = bgfx::createUniform("u_tetraNormalGreen", bgfx::UniformType::Vec4);
		u_tetraNormalYellow = bgfx::createUniform("u_tetraNormalYellow", bgfx::UniformType::Vec4);
		u_tetraNormalBlue = bgfx::createUniform("u_tetraNormalBlue", bgfx::UniformType::Vec4);
		u_tetraNormalRed = bgfx::createUniform("u_tetraNormalRed", bgfx::UniformType::Vec4);

		u_shadowMapMtx0 = bgfx::createUniform("u_shadowMapMtx0", bgfx::UniformType::Mat4);
		u_shadowMapMtx1 = bgfx::createUniform("u_shadowMapMtx1", bgfx::UniformType::Mat4);
		u_shadowMapMtx2 = bgfx::createUniform("u_shadowMapMtx2", bgfx::UniformType::Mat4);
		u_shadowMapMtx3 = bgfx::createUniform("u_shadowMapMtx3", bgfx::UniformType::Mat4);
	}

	void CBgfxShadowCsm::FUniforms::setPtrs(float* _colorPtr, float* _lightMtxPtr, float* _shadowMapMtx0, float* _shadowMapMtx1, float* _shadowMapMtx2, float* _shadowMapMtx3)
	{
		m_lightMtxPtr = _lightMtxPtr;
		m_colorPtr = _colorPtr;

		m_shadowMapMtx0 = _shadowMapMtx0;
		m_shadowMapMtx1 = _shadowMapMtx1;
		m_shadowMapMtx2 = _shadowMapMtx2;
		m_shadowMapMtx3 = _shadowMapMtx3;
	}

	void CBgfxShadowCsm::FUniforms::submitConstUniforms()
	{
		bgfx::setUniform(u_tetraNormalGreen, m_tetraNormalGreen);
		bgfx::setUniform(u_tetraNormalYellow, m_tetraNormalYellow);
		bgfx::setUniform(u_tetraNormalBlue, m_tetraNormalBlue);
		bgfx::setUniform(u_tetraNormalRed, m_tetraNormalRed);
	}

	void CBgfxShadowCsm::FUniforms::submitPerFrameUniforms()
	{
		bgfx::setUniform(u_params1, m_params1);
		bgfx::setUniform(u_params2, m_params2);
		bgfx::setUniform(u_smSamplingParams, m_paramsBlur);
		bgfx::setUniform(u_csmFarDistances, m_csmFarDistances);
	}

	void CBgfxShadowCsm::FUniforms::submitPerDrawUniforms()
	{
		bgfx::setUniform(u_shadowMapMtx0, m_shadowMapMtx0);
		bgfx::setUniform(u_shadowMapMtx1, m_shadowMapMtx1);
		bgfx::setUniform(u_shadowMapMtx2, m_shadowMapMtx2);
		bgfx::setUniform(u_shadowMapMtx3, m_shadowMapMtx3);

		bgfx::setUniform(u_params0, m_params0);
		bgfx::setUniform(u_lightMtx, m_lightMtxPtr);
		bgfx::setUniform(u_color, m_colorPtr);
	}

	void CBgfxShadowCsm::FUniforms::destroy()
	{
		bgfx::destroy(u_params0);
		bgfx::destroy(u_params1);
		bgfx::destroy(u_params2);
		bgfx::destroy(u_color);
		bgfx::destroy(u_smSamplingParams);
		bgfx::destroy(u_csmFarDistances);

		bgfx::destroy(u_tetraNormalGreen);
		bgfx::destroy(u_tetraNormalYellow);
		bgfx::destroy(u_tetraNormalBlue);
		bgfx::destroy(u_tetraNormalRed);

		bgfx::destroy(u_shadowMapMtx0);
		bgfx::destroy(u_shadowMapMtx1);
		bgfx::destroy(u_shadowMapMtx2);
		bgfx::destroy(u_shadowMapMtx3);

		bgfx::destroy(u_lightMtx);
	}

}