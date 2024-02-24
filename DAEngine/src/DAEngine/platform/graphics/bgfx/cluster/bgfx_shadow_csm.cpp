#include "dapch.h"

#include "bgfx_shadow_csm.h"

#include <bgfx/bgfx.h>
#include <core/graphics/camera.h>
#include <bx/math.h>
#include "DAEngine\platform\graphics\bgfx\bgfx_graphics_material.h"

#include "DAEngine/debug/debug_menu_bar.h"
#include <imgui.h>

#define RNEAR m_near
#define RFAR m_far

namespace da::platform
{

	void CBgfxShadowCSM::initialize()
	{
		m_ambientPass = 1.0f;
		m_lightingPass = 1.0f;

	

		m_csmFarDistances[0] = 30.0f;
		m_csmFarDistances[1] = 90.0f;
		m_csmFarDistances[2] = 180.0f;
		m_csmFarDistances[3] = 1000.0f;

		m_shadowMapBias = 0.0012f;
		m_shadowMapOffset = 0.001f;
		m_shadowMapParam0 = 0.7f;
		m_shadowMapParam1 = 500.f;
		m_depthValuePow = 1.0f;
		m_showSmCoverage = 0.0f;
		m_near = .1f;
		m_far = 100.f;
		m_cascadePerc = .6f;

		m_shadowMapTexelSize = 1 / SHADOW_MAP_RENDER_TARGET_SIZE;

		m_XNum = 2.0f;
		m_YNum = 2.0f;
		m_XOffset = 10.0f / 512.0f;
		m_YOffset = 10.0f / 512.0f;

		u_params0 = ::bgfx::createUniform("u_params0", ::bgfx::UniformType::Vec4);
		u_params1 = ::bgfx::createUniform("u_params1", ::bgfx::UniformType::Vec4);
		u_params2 = ::bgfx::createUniform("u_params2", ::bgfx::UniformType::Vec4);
		u_color = ::bgfx::createUniform("u_color", ::bgfx::UniformType::Vec4);
		u_smSamplingParams = ::bgfx::createUniform("u_smSamplingParams", ::bgfx::UniformType::Vec4);
		u_csmFarDistances = ::bgfx::createUniform("u_csmFarDistances", ::bgfx::UniformType::Vec4);
		u_lightMtx = ::bgfx::createUniform("u_lightMtx", ::bgfx::UniformType::Mat4);


		for (uint8_t ii = 0; ii < SHADOW_MAP_RENDER_TARGETS; ++ii)
		{
			::bgfx::TextureHandle fbtextures[] =
			{
				::bgfx::createTexture2D(SHADOW_MAP_RENDER_TARGET_SIZE, SHADOW_MAP_RENDER_TARGET_SIZE, false, 1, ::bgfx::TextureFormat::BGRA8, BGFX_TEXTURE_RT),
				::bgfx::createTexture2D(SHADOW_MAP_RENDER_TARGET_SIZE, SHADOW_MAP_RENDER_TARGET_SIZE, false, 1, ::bgfx::TextureFormat::D32F,  BGFX_TEXTURE_RT),
			};
			m_rtShadowMap[ii] = ::bgfx::createFrameBuffer(BX_COUNTOF(fbtextures), fbtextures, true);
		}
		m_rtBlur = ::bgfx::createFrameBuffer(SHADOW_MAP_RENDER_TARGET_SIZE, SHADOW_MAP_RENDER_TARGET_SIZE, ::bgfx::TextureFormat::BGRA8);
		
		//m_vBlur = new CBgfxGraphicsMaterial("shaders/cluster/vs_shadowmaps_vblur", "shaders/cluster/fs_shadowmaps_vblur.sc");
		//m_hBlur = new CBgfxGraphicsMaterial("shaders/cluster/vs_shadowmaps_hblur", "shaders/cluster/fs_shadowmaps_hblur.sc");
		m_drawDepth = new CBgfxGraphicsMaterial("shaders/cluster/vs_shadowmaps_unpackdepth.sc", "shaders/cluster/fs_shadowmaps_unpackdepth.sc");
		m_packDepth = new CBgfxGraphicsMaterial("shaders/cluster/vs_shadowmaps_packdepth_linear.sc", "shaders/cluster/fs_shadowmaps_packdepth_linear.sc");
		//m_colorLighting = new CBgfxGraphicsMaterial("shaders/cluster/vs_shadowmaps_color_lighting_linear_csm.sc", "shaders/cluster/fs_shadowmaps_color_lighting_hard_linear_csm.sc");

		//m_vBlur->initialize();
		//m_hBlur->initialize();
		m_drawDepth->initialize();
		m_packDepth->initialize();
		//m_colorLighting->initialize();

#if defined(DA_DEBUG) || defined(DA_RELEASE)
		da::debug::CDebugMenuBar::register_debug(HASHSTR("Renderer"), HASHSTR("Shadows"), &m_showDebug, [this]() { this->renderDebug(); });
#endif
	}

	void CBgfxShadowCSM::submitUniforms()
	{
		::bgfx::setUniform(u_params0, m_params0);
		::bgfx::setUniform(u_params1, m_params1);
		::bgfx::setUniform(u_params2, m_params2);
		::bgfx::setUniform(u_smSamplingParams, m_paramsBlur);
		::bgfx::setUniform(u_csmFarDistances, m_csmFarDistances);
	}

	void CBgfxShadowCSM::render(uint8_t view, const glm::vec3& lightDir, float width, float height)
	{
		const ::bgfx::Caps* caps = ::bgfx::getCaps();
		float lightView[SHADOW_MAP_RENDER_TARGETS][16];
		float lightProj[SHADOW_MAP_RENDER_TARGETS][16];

		glm::vec3 dir = glm::radians(lightDir);

		bx::mtxLookAt(lightView[0], { 3, 3, 10 }, { 0, 0, 0}, { 0.f,0.f,1.f }, bx::Handedness::Right);

		float invCamView[16];
		float fov = 0.f;
		{
			da::core::CCamera* cam = da::core::CCamera::getCamera();
			fov = cam->fov;
			bx::Vec3 pos(cam->position().x, cam->position().y, cam->position().z);
			glm::vec3 _at = cam->position() + cam->forward();
			bx::Vec3 at(_at.x, _at.y, _at.z);
			bx::Vec3 up(cam->up().x, cam->up().y, cam->up().z);
			float lightView[16];
			bx::mtxLookAt(lightView, pos, at, up, bx::Handedness::Right);
			bx::mtxInverse(invCamView, glm::value_ptr(cam->matrix()));
		}

		// Compute split distances.
		const uint8_t maxNumSplits = 4;

		float splitSlices[maxNumSplits * 2];
		splitFrustum(splitSlices
			, uint8_t(4)
			, RNEAR
			, RFAR
			, m_cascadePerc
		);

		// Update uniforms.
		for (uint8_t ii = 0, ff = 1; ii < 4; ++ii, ff += 2)
		{
			// This lags for 1 frame, but it's not a problem.
			m_csmFarDistances[ii] = splitSlices[ff];
		}

		float mtxProj[16];
		bx::mtxOrtho(
			mtxProj
			, 1.0f
			, -1.0f
			, 1.0f
			, -1.0f
			, -RFAR
			, RFAR
			, 0.0f
			, caps->homogeneousDepth
			, bx::Handedness::Right
		);

		const float projHeight = .577f;
		const float projWidth = 1.0f;

		const uint8_t numCorners = 8;
		float frustumCorners[maxNumSplits][numCorners][3];
		for (uint8_t ii = 0, nn = 0, ff = 1; ii < 4; ++ii, nn += 2, ff += 2)
		{
			// Compute frustum corners for one split in world space.
			worldSpaceFrustumCorners((float*)frustumCorners[ii], splitSlices[nn], splitSlices[ff], projWidth, projHeight, invCamView);

			bx::Vec3 min = { 9000.0f,  9000.0f,  9000.0f };
			bx::Vec3 max = { -9000.0f, -9000.0f, -9000.0f };

			glm::vec3 center = glm::vec3(0.f);

			for (uint8_t jj = 0; jj < numCorners; ++jj)
			{
				bx::Vec3 xyz = bx::load<bx::Vec3>(frustumCorners[ii][jj]);
				center = center + glm::vec3(xyz.x,xyz.y, xyz.z);
			}

			center /= (float)numCorners;
			//bx::mtxLookAt(lightView[0], { lightDir.x, lightDir.y, lightDir.z }, { center.x, center.y, center.z }, { 0.f,0.f,1.f }, bx::Handedness::Right);

			for (uint8_t jj = 0; jj < numCorners; ++jj)
			{
				// Transform to light space.
				const bx::Vec3 xyz = bx::mul(bx::load<bx::Vec3>(frustumCorners[ii][jj]), lightView[0]);

				// Update bounding box.
				min = bx::min(min, xyz);
				max = bx::max(max, xyz);
			}

			const bx::Vec3 minproj = bx::mulH(min, mtxProj);
			const bx::Vec3 maxproj = bx::mulH(max, mtxProj);

			float scalex = 2.0f / (maxproj.x - minproj.x);
			float scaley = 2.0f / (maxproj.y - minproj.y);

			if (true)
			{
				const float quantizer = 64.0f;
				scalex = quantizer / bx::ceil(quantizer / scalex);
				scaley = quantizer / bx::ceil(quantizer / scaley);
			}

			float offsetx = 0.5f * (maxproj.x + minproj.x) * scalex;
			float offsety = 0.5f * (maxproj.y + minproj.y) * scaley;

			if (true)
			{
				const float halfSize = SHADOW_MAP_RENDER_TARGET_SIZE * 0.5f;
				offsetx = bx::ceil(offsetx * halfSize) / halfSize;
				offsety = bx::ceil(offsety * halfSize) / halfSize;
			}

			float mtxCrop[16];
			bx::mtxIdentity(mtxCrop);
			mtxCrop[0] = scalex;
			mtxCrop[5] = scaley;

			// Swap Y and Z axes
			float temp = mtxCrop[1];
			mtxCrop[1] = mtxCrop[2];
			mtxCrop[2] = -temp;

			// Negate the new Z component
			mtxCrop[14] = -mtxCrop[14] - offsety * mtxCrop[13];
			mtxCrop[13] = offsetx * mtxCrop[13];

			mtxCrop[12] = offsetx * mtxCrop[0] + offsety * mtxCrop[4] + mtxCrop[12];
			mtxCrop[13] = offsetx * mtxCrop[1] + offsety * mtxCrop[5] + mtxCrop[13];
			mtxCrop[14] = offsetx * mtxCrop[2] + offsety * mtxCrop[6] + mtxCrop[14];
			mtxCrop[15] = offsetx * mtxCrop[3] + offsety * mtxCrop[7] + mtxCrop[15];
			bx::mtxMul(lightProj[ii], mtxCrop, mtxProj);
		}


		const float mtxBias[16] =
		{
			0.5f, 0.0f, 0.0f, 0.0f,
			0.0f, -.5f, 0.0f, 0.0f,
			0.0f, 0.0f, 0.5f, 0.0f,
			0.5f, 0.5f, 0.f, 1.0f,
		};
		//LightType::DirectionalLight == settings.m_lightType
		{
			for (uint8_t ii = 0; ii < 4; ++ii)
			{
				float mtxTmp[16];

				bx::mtxMul(mtxTmp, lightProj[ii], mtxBias);
				bx::mtxMul(m_shadowMapMtx[ii], lightView[0], mtxTmp); //lViewProjCropBias
			}
		}

		/*
		for (uint8_t ii = 0; ii < SHADOW_MAP_RENDER_TARGETS; ++ii)
		{			
			const ::bgfx::Caps* caps = ::bgfx::getCaps();
			const float sy = caps->originBottomLeft ? 0.5f : -0.5f;
			const float sz = caps->homogeneousDepth ? 0.5f : 1.0f;
			const float tz = caps->homogeneousDepth ? 0.5f : 0.0f;
			const float mtxCrop[16] =
			{
				0.5f, 0.0f, 0.0f, 0.0f,
				0.0f,   sy, 0.0f, 0.0f,
				0.0f, 0.0f, sz,   0.0f,
				0.5f, 0.5f, tz,   1.0f,
			};

			
			float mtxTmp[16];
			
			bx::mtxMul(mtxTmp, lightProj[ii], mtxCrop);
			bx::mtxMul(m_shadowMapMtx[ii], lightView[0], mtxTmp);
		}
		*/
		

		// Reset render targets.
		const ::bgfx::FrameBufferHandle invalidRt = BGFX_INVALID_HANDLE;
		for (uint8_t ii = 0; ii < SHADOW_MAP_RENDER_TARGETS; ++ii)
		{
			::bgfx::setViewFrameBuffer(view+ii, invalidRt);
			::bgfx::setViewRect(view+ii, 0, 0, width, height);
		}

		// Determine on-screen rectangle size where depth buffer will be drawn.
		uint16_t depthRectHeight = uint16_t(float(height) / 2.5f);
		uint16_t depthRectWidth = depthRectHeight;
		uint16_t depthRectX = 0;
		uint16_t depthRectY = height - depthRectHeight;

		::bgfx::setViewName(view + 0, (std::string("Shadow: ") + std::to_string(0)).c_str());
		::bgfx::setViewName(view + 1, (std::string("Shadow: ") + std::to_string(1)).c_str());
		::bgfx::setViewName(view + 2, (std::string("Shadow: ") + std::to_string(2)).c_str());
		::bgfx::setViewName(view + 3, (std::string("Shadow: ") + std::to_string(3)).c_str());

		::bgfx::setViewRect(view + 0, 0, 0, SHADOW_MAP_RENDER_TARGET_SIZE, SHADOW_MAP_RENDER_TARGET_SIZE);
		::bgfx::setViewRect(view + 1, 0, 0, SHADOW_MAP_RENDER_TARGET_SIZE, SHADOW_MAP_RENDER_TARGET_SIZE);
		::bgfx::setViewRect(view + 2, 0, 0, SHADOW_MAP_RENDER_TARGET_SIZE, SHADOW_MAP_RENDER_TARGET_SIZE);
		::bgfx::setViewRect(view + 3, 0, 0, SHADOW_MAP_RENDER_TARGET_SIZE, SHADOW_MAP_RENDER_TARGET_SIZE);

		::bgfx::setViewTransform(view + 0, lightView[0], lightProj[0]);
		::bgfx::setViewTransform(view + 1, lightView[0], lightProj[1]);
		::bgfx::setViewTransform(view + 2, lightView[0], lightProj[2]);
		::bgfx::setViewTransform(view + 3, lightView[0], lightProj[3]);

		::bgfx::setViewFrameBuffer(view + 0, m_rtShadowMap[0]);
		::bgfx::setViewFrameBuffer(view + 1, m_rtShadowMap[1]);
		::bgfx::setViewFrameBuffer(view + 2, m_rtShadowMap[2]);
		::bgfx::setViewFrameBuffer(view + 3, m_rtShadowMap[3]);

		for (uint8_t ii = 0; ii < 4; ++ii)
		{
			::bgfx::setViewClear(view + ii
				, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
				, 0xfefefefe //blur fails on completely white regions
				, 1.0
				, 0.0
			);
			::bgfx::touch(view + ii);
		}

		for (uint8_t ii = 0; ii < SHADOW_MAP_RENDER_TARGETS; ++ii)
		{
			const uint8_t viewId = view + ii;
			uint8_t renderStateIndex = RenderState::ShadowMap_PackDepth;
			m_renderFunc(viewId, m_packDepth, m_renderStates[renderStateIndex].m_state);
		}
	}



	void CBgfxShadowCSM::worldSpaceFrustumCorners(float* _corners24f, float _near, float _far, float _projWidth, float _projHeight, const float* _invViewMtx)
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


	void CBgfxShadowCSM::splitFrustum(float* _splits, uint8_t _numSplits, float _near, float _far, float _splitWeight /*= 0.75f*/)
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
	}



	void CBgfxShadowCSM::shutdown()
	{
		m_vBlur->shutdown();
		m_hBlur->shutdown();
		m_drawDepth->shutdown();
		m_packDepth->shutdown();
		m_colorLighting->shutdown();

		for (uint8_t ii = 0; ii < SHADOW_MAP_RENDER_TARGETS; ++ii)
		{
			::bgfx::destroy(m_rtShadowMap[ii]);
		}
		::bgfx::destroy(m_rtBlur);

		delete m_vBlur;
		delete m_hBlur;
		delete m_drawDepth;
		delete m_packDepth;
		delete m_colorLighting;

#if defined(DA_DEBUG) || defined(DA_RELEASE)
		da::debug::CDebugMenuBar::unregister_debug(HASHSTR("Renderer"), HASHSTR("Shadows"));
#endif
	}

	void CBgfxShadowCSM::setRenderFunc(const std::function<void(uint8_t id, CBgfxGraphicsMaterial* shader, uint64_t state)>& f)
	{
		m_renderFunc = f;
	}

	const float* CBgfxShadowCSM::getShadowMat(uint8_t index) const
	{
		return m_shadowMapMtx[index];
	}

	::bgfx::TextureHandle CBgfxShadowCSM::getShadowMap(uint8_t index)
	{
		return ::bgfx::getTexture(m_rtShadowMap[index]);
	}

	void CBgfxShadowCSM::convertLeftToRightHandedZUp(float* matrix)
	{
		float tmp[16];
		// Swap Y and Z axes
		tmp[0] = matrix[0];
		tmp[1] = matrix[2];
		tmp[2] = -matrix[1];
		tmp[3] = matrix[3];

		tmp[4] = matrix[8];
		tmp[5] = matrix[10];
		tmp[6] = -matrix[9];
		tmp[7] = matrix[11];

		tmp[8] = -matrix[4];
		tmp[9] = -matrix[6];
		tmp[10] = matrix[5];
		tmp[11] = matrix[7];

		tmp[12] = matrix[12];
		tmp[13] = matrix[13];
		tmp[14] = -matrix[14];
		tmp[15] = matrix[15];

		memcpy(matrix, tmp, sizeof(tmp));
	}
#if defined(DA_DEBUG) || defined(DA_RELEASE)
	void CBgfxShadowCSM::renderDebug()
	{
		if (ImGui::Begin("Shadow Debug", &m_showDebug)) {

			ImGui::InputFloat("m_shadowMapBias", &m_shadowMapBias);
			ImGui::InputFloat("m_shadowMapOffset", &m_shadowMapOffset);
			ImGui::InputFloat("m_shadowMapParam0", &m_shadowMapParam0);
			ImGui::InputFloat("m_shadowMapParam1", &m_shadowMapParam1);
			ImGui::InputFloat("m_depthValuePow", &m_depthValuePow);
			ImGui::InputFloat("m_showSmCoverage", &m_showSmCoverage);
			ImGui::InputFloat("m_far", &m_far);
			ImGui::InputFloat("m_near", &m_near);
			ImGui::InputFloat("m_cascadePerc", &m_cascadePerc);

		}
		ImGui::End();
	}
#endif

}