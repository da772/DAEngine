#include "dapch.h"

#include "bgfx_sky.h"
#include "DAEngine/platform/graphics/bgfx/bgfx_graphics_material.h"
#include <bx/allocator.h>
#include <platform/graphics/bgfx/bgfx_util.h>
#include <glm/trigonometric.hpp>
#include <glm/ext/quaternion_float.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <bx/math.h>
#ifdef DA_DEBUG
#include "DAEngine/debug/debug_menu_bar.h"
#include <imgui.h>
#endif

namespace da::platform
{
	static constexpr glm::vec3 ABCDE_t[] =
	{
		{ -0.0193f, -0.0167f,  0.1787f },
		{ -0.0665f, -0.0950f, -0.3554f },
		{ -0.0004f, -0.0079f, -0.0227f },
		{ -0.0641f, -0.0441f,  0.1206f },
		{ -0.0033f, -0.0109f, -0.0670f },
	};

	// HDTV rec. 709 matrix.
	static constexpr float M_XYZ2RGB[] =
	{
		3.240479f, -0.969256f,  0.055648f,
		-1.53715f,   1.875991f, -0.204043f,
		-0.49853f,   0.041556f,  1.057311f,
	};

	// Turbidity tables. Taken from:
// A. J. Preetham, P. Shirley, and B. Smits. A Practical Analytic Model for Daylight. SIGGRAPH '99
// Coefficients correspond to xyY colorspace.
	static constexpr glm::vec3 ABCDE[] =
	{
		{ -0.2592f, -0.2608f, -1.4630f },
		{  0.0008f,  0.0092f,  0.4275f },
		{  0.2125f,  0.2102f,  5.3251f },
		{ -0.8989f, -1.6537f, -2.5771f },
		{  0.0452f,  0.0529f,  0.3703f },
	};

	// Precomputed luminance of sunlight in XYZ colorspace.
// Computed using code from Game Engine Gems, Volume One, chapter 15. Implementation based on Dr. Richard Bird model.
// This table is used for piecewise linear interpolation. Transitions from and to 0.0 at sunset and sunrise are highly inaccurate
	static std::map<float, glm::vec3> sunLuminanceXYZTable =
	{
		{  5.0f, {  0.000000f,  0.000000f,  0.000000f } },
		{  7.0f, { 12.703322f, 12.989393f,  9.100411f } },
		{  8.0f, { 13.202644f, 13.597814f, 11.524929f } },
		{  9.0f, { 13.192974f, 13.597458f, 12.264488f } },
		{ 10.0f, { 13.132943f, 13.535914f, 12.560032f } },
		{ 11.0f, { 13.088722f, 13.489535f, 12.692996f } },
		{ 12.0f, { 13.067827f, 13.467483f, 12.745179f } },
		{ 13.0f, { 13.069653f, 13.469413f, 12.740822f } },
		{ 14.0f, { 13.094319f, 13.495428f, 12.678066f } },
		{ 15.0f, { 13.142133f, 13.545483f, 12.526785f } },
		{ 16.0f, { 13.201734f, 13.606017f, 12.188001f } },
		{ 17.0f, { 13.182774f, 13.572725f, 11.311157f } },
		{ 18.0f, { 12.448635f, 12.672520f,  8.267771f } },
		{ 20.0f, {  0.000000f,  0.000000f,  0.000000f } },
	};


	// Precomputed luminance of sky in the zenith point in XYZ colorspace.
	// Computed using code from Game Engine Gems, Volume One, chapter 15. Implementation based on Dr. Richard Bird model.
	// This table is used for piecewise linear interpolation. Day/night transitions are highly inaccurate.
	// The scale of luminance change in Day/night transitions is not preserved.
	// Luminance at night was increased to eliminate need the of HDR render.
	static std::map<float, glm::vec3> skyLuminanceXYZTable =
	{
		{  0.0f, { 0.308f,    0.308f,    0.411f    } },
		{  1.0f, { 0.308f,    0.308f,    0.410f    } },
		{  2.0f, { 0.301f,    0.301f,    0.402f    } },
		{  3.0f, { 0.287f,    0.287f,    0.382f    } },
		{  4.0f, { 0.258f,    0.258f,    0.344f    } },
		{  5.0f, { 0.258f,    0.258f,    0.344f    } },
		{  7.0f, { 0.962851f, 1.000000f, 1.747835f } },
		{  8.0f, { 0.967787f, 1.000000f, 1.776762f } },
		{  9.0f, { 0.970173f, 1.000000f, 1.788413f } },
		{ 10.0f, { 0.971431f, 1.000000f, 1.794102f } },
		{ 11.0f, { 0.972099f, 1.000000f, 1.797096f } },
		{ 12.0f, { 0.972385f, 1.000000f, 1.798389f } },
		{ 13.0f, { 0.972361f, 1.000000f, 1.798278f } },
		{ 14.0f, { 0.972020f, 1.000000f, 1.796740f } },
		{ 15.0f, { 0.971275f, 1.000000f, 1.793407f } },
		{ 16.0f, { 0.969885f, 1.000000f, 1.787078f } },
		{ 17.0f, { 0.967216f, 1.000000f, 1.773758f } },
		{ 18.0f, { 0.961668f, 1.000000f, 1.739891f } },
		{ 20.0f, { 0.264f,    0.264f,    0.352f    } },
		{ 21.0f, { 0.264f,    0.264f,    0.352f    } },
		{ 22.0f, { 0.290f,    0.290f,    0.386f    } },
		{ 23.0f, { 0.303f,    0.303f,    0.404f    } },
	};



	::bgfx::VertexLayout FScreenPosVertex::ms_layout;

	void CBgfxProcSky::initialize(int verticalCount, int horizontalCount, CBgfxSunController& sun) 
	{
		m_sun = &sun;
		m_skyProgram =    da::graphics::CMaterialFactory::create("shaders/cluster/vs_sky.sc", "shaders/cluster/fs_sky.sc");
		m_skyProgramFix = da::graphics::CMaterialFactory::create("shaders/cluster/vs_sky.sc", "shaders/cluster/fs_sky_color_banding_fix.sc");

		FScreenPosVertex::init();
		m_sunLuminanceXYZ.SetMap(sunLuminanceXYZTable);
		m_skyLuminanceXYZ.SetMap(skyLuminanceXYZTable);

		FScreenPosVertex* vertices = (FScreenPosVertex*)malloc(verticalCount * horizontalCount * sizeof(FScreenPosVertex));

		for (int i = 0; i < verticalCount; i++)
		{
			for (int j = 0; j < horizontalCount; j++)
			{
				FScreenPosVertex& v = vertices[i * verticalCount + j];
				v.m_x = float(j) / (horizontalCount - 1) * 2.0f - 1.0f;
				v.m_y = float(i) / (verticalCount - 1) * 2.0f - 1.0f;
			}
		}

		uint16_t* indices = (uint16_t*)malloc((verticalCount - 1) * (horizontalCount - 1) * 6 * sizeof(uint16_t));

		int k = 0;
		for (int i = 0; i < verticalCount - 1; i++)
		{
			for (int j = 0; j < horizontalCount - 1; j++)
			{
				indices[k++] = (uint16_t)(j + 0 + horizontalCount * (i + 0));
				indices[k++] = (uint16_t)(j + 1 + horizontalCount * (i + 0));
				indices[k++] = (uint16_t)(j + 0 + horizontalCount * (i + 1));

				indices[k++] = (uint16_t)(j + 1 + horizontalCount * (i + 0));
				indices[k++] = (uint16_t)(j + 1 + horizontalCount * (i + 1));
				indices[k++] = (uint16_t)(j + 0 + horizontalCount * (i + 1));
			}
		}

		m_vbh = bgfx::createVertexBuffer(bgfx::copy(vertices, sizeof(FScreenPosVertex) * verticalCount * horizontalCount), FScreenPosVertex::ms_layout);
		m_ibh = bgfx::createIndexBuffer(bgfx::copy(indices, sizeof(uint16_t) * k));

		free(vertices);
		free(indices);

		u_skyLuminanceXYZ = bgfx::createUniform("u_skyLuminanceXYZ", bgfx::UniformType::Vec4);
		u_skyLuminance    = bgfx::createUniform("u_skyLuminance", bgfx::UniformType::Vec4);
		u_sunLuminance    = bgfx::createUniform("u_sunLuminance", bgfx::UniformType::Vec4);
		u_sunDirection    = bgfx::createUniform("u_sunDirection", bgfx::UniformType::Vec4);
		u_parameters      = bgfx::createUniform("u_parameters", bgfx::UniformType::Vec4);
		u_perezCoeff      = bgfx::createUniform("u_perezCoeff", bgfx::UniformType::Vec4, 5);

		m_sun->Update();

#ifdef DA_DEBUG
		da::debug::CDebugMenuBar::register_debug(HASHSTR("Renderer"), HASHSTR("Sky"), &m_debug, [this] {renderDebug(); });
#endif
	}

	void CBgfxProcSky::render(bgfx::ViewId id, uint64_t state)
	{
		setUniforms();
		bgfx::setState(BGFX_STATE_WRITE_RGB | BGFX_STATE_DEPTH_TEST_EQUAL | state);
		bgfx::setIndexBuffer(m_ibh);
		bgfx::setVertexBuffer(0, m_vbh);
		bgfx::submit(id, { m_skyProgramFix->getHandle() }, 0, ~BGFX_DISCARD_BINDINGS);
	}

	void CBgfxProcSky::shutdown()
	{
		da::graphics::CMaterialFactory::remove(m_skyProgram);
		da::graphics::CMaterialFactory::remove(m_skyProgramFix);

		BGFXTRYDESTROY(m_vbh);
		BGFXTRYDESTROY(m_ibh);
		BGFXTRYDESTROY(u_skyLuminanceXYZ);
		BGFXTRYDESTROY(u_skyLuminance);
		BGFXTRYDESTROY(u_sunDirection);
		BGFXTRYDESTROY(u_parameters);
		BGFXTRYDESTROY(u_perezCoeff);
		BGFXTRYDESTROY(u_sunLuminance);

#ifdef DA_DEBUG
		da::debug::CDebugMenuBar::unregister_debug(HASHSTR("Renderer"), HASHSTR("Sky"));
#endif
	}

	glm::vec3 CBgfxProcSky::getSunLuminance() const
	{
		glm::vec3 sunLuminanceXYZ = m_sunLuminanceXYZ.GetValue(m_sun->getTime());
		glm::vec3 sunLuminanceRGB = xyzToRgb(sunLuminanceXYZ) * m_sunColor;
		return sunLuminanceRGB;
	}

	void CBgfxProcSky::setUniforms()
	{
		m_parameters.w = m_sun->getTime();
		glm::vec3 sunLuminanceRGB = getSunLuminance();

		glm::vec3 skyLuminanceXYZ = m_skyLuminanceXYZ.GetValue(m_parameters.w);
		glm::vec3 skyLuminanceRGB = xyzToRgb(skyLuminanceXYZ);

		bgfx::setUniform(u_sunLuminance, &sunLuminanceRGB.x);
		bgfx::setUniform(u_skyLuminanceXYZ, &skyLuminanceXYZ.x);
		bgfx::setUniform(u_skyLuminance, &skyLuminanceRGB.x);

		bgfx::setUniform(u_sunDirection, &m_sun->m_sunDir.x);

		// x - sun size, y - sun bloom, z - exposition, w - time
		bgfx::setUniform(u_parameters, glm::value_ptr(m_parameters));

		float perezCoeff[4 * 5];
		float turbidity = 2.15f;
		computePerezCoeff(turbidity, perezCoeff);
		bgfx::setUniform(u_perezCoeff, perezCoeff, 5);
	}

	void CBgfxProcSky::computePerezCoeff(float _turbidity, float* _outPerezCoeff)
	{
		const glm::vec3 turbidity = { _turbidity, _turbidity, _turbidity };
		for (uint32_t ii = 0; ii < 5; ++ii)
		{
			const glm::vec3 tmp = (ABCDE_t[ii] * turbidity) + ABCDE[ii];
			float* out = _outPerezCoeff + 4 * ii;
			bx::store(out, tmp);
			out[3] = 0.0f;
		}
	}

	glm::vec3 CBgfxProcSky::xyzToRgb(const glm::vec3& xyz) const
	{
		glm::vec3 rgb;
		rgb.x = M_XYZ2RGB[0] * xyz.x + M_XYZ2RGB[3] * xyz.y + M_XYZ2RGB[6] * xyz.z;
		rgb.y = M_XYZ2RGB[1] * xyz.x + M_XYZ2RGB[4] * xyz.y + M_XYZ2RGB[7] * xyz.z;
		rgb.z = M_XYZ2RGB[2] * xyz.x + M_XYZ2RGB[5] * xyz.y + M_XYZ2RGB[8] * xyz.z;
		return rgb;
	}

#ifdef DA_DEBUG
	void CBgfxProcSky::renderDebug()
	{
		if (ImGui::Begin("Sky debug", &m_debug))
		{
			ImGui::Text("Sun Size: ");
			ImGui::SameLine();
			ImGui::InputFloat("##ss", &m_parameters.x);

			ImGui::Text("Sun Bloom: ");
			ImGui::SameLine();
			ImGui::InputFloat("##sb", &m_parameters.y);

			ImGui::Text("Sun Exposition: ");
			ImGui::SameLine();
			ImGui::InputFloat("##se", &m_parameters.z);

			ImGui::Text("Sun Color: ");
			ImGui::SameLine();
			ImGui::InputFloat3("##scolor", &m_sunColor[0]);

			ImGui::Text("Sky Time: ");
			ImGui::SameLine();
			float skyTime = m_sun->getTime();
			if (ImGui::DragFloat("###skyTime", &skyTime, .1f, 0.f, 24.f)) {
				m_sun->setTime(skyTime);
			}
		}

		ImGui::End();
	}



#endif

	CBgfxSunController::CBgfxSunController() : m_northDir(1.0f, 0.0f, 0.0f)
		, m_sunDir(0.0f, 0.0f, -1.0f)
		, m_upDir(0.0f, 0.0f, 1.0f)
		, m_latitude(50.0f)
		, m_month(June)
		, m_eclipticObliquity(glm::radians(23.4f))
		, m_delta(0.0f)
	{

	}

	void CBgfxSunController::Update()
	{
		CalculateSunOrbit();
		UpdateSunPosition(m_time - 12.0f);
	}

	void CBgfxSunController::CalculateSunOrbit()
	{
		const float day = 30.0f * float(m_month) + 15.0f;
		float lambda = 280.46f + 0.9856474f * day;
		lambda = glm::radians(lambda);
		m_delta = glm::asin(glm::sin(m_eclipticObliquity) * glm::sin(lambda));
	}

	void CBgfxSunController::UpdateSunPosition(float _hour)
	{
		const float latitude = glm::radians(m_latitude);
		const float hh = _hour * bx::kPi / 12.0f;
		const float azimuth = atan2(
			glm::sin(hh)
			, glm::cos(hh) * glm::sin(latitude) - glm::tan(m_delta) * glm::cos(latitude)
		);

		const float altitude = glm::asin(
			glm::sin(latitude) * glm::sin(m_delta) + glm::cos(latitude) * glm::cos(m_delta) * glm::cos(hh)
		);

		const glm::quat rot0 = glm::angleAxis(-azimuth, m_upDir);
		const glm::vec3 dir = m_northDir * rot0;
		const glm::vec3 uxd = glm::cross(m_upDir, dir);

		const glm::quat rot1 = glm::angleAxis(altitude, uxd);
		m_sunDir = dir * rot1;
	}

	glm::vec2 CBgfxSunController::getScreenSpacePos(glm::mat4 viewMat, glm::mat4 projMatrix) const
	{
		glm::vec4 lightDirectionViewSpace = viewMat * glm::vec4(m_sunDir, 0.0);
		glm::vec4 lightDirectionClipSpace = projMatrix * lightDirectionViewSpace;
		glm::vec3 lightDirectionNDC = glm::vec3(lightDirectionClipSpace) / lightDirectionClipSpace.w;
		glm::vec2 lightDirectionScreenSpace = glm::vec2(0.5) * (glm::vec2(lightDirectionNDC) + glm::vec2(1.0, 1.0));
		return lightDirectionScreenSpace;
	}

	float CBgfxSunController::getTime() const
	{
		return m_time;
	}

	void CBgfxSunController::setTime(float time)
	{
		m_time = time;
		Update();
	}

	glm::vec3 lerp(glm::vec3 x, glm::vec3 y, float t) {
		return x * (1.f - t) + y * t;
	}

	CDynamicValueController::ValueType CDynamicValueController::interpolate(float lowerTime, const ValueType& lowerVal, float upperTime, const ValueType& upperVal, float time) const
	{
		float tt = (time - lowerTime) / (upperTime - lowerTime);
		const ValueType result = lerp(lowerVal, upperVal, tt);
		return result;
	}

	CDynamicValueController::ValueType CDynamicValueController::GetValue(float time) const
	{
		typename KeyMap::const_iterator itUpper = m_keyMap.upper_bound(time + 1e-6f);
		typename KeyMap::const_iterator itLower = itUpper;
		--itLower;

		if (itLower == m_keyMap.end())
		{
			return itUpper->second;
		}

		if (itUpper == m_keyMap.end())
		{
			return itLower->second;
		}

		float lowerTime = itLower->first;
		const ValueType& lowerVal = itLower->second;
		float upperTime = itUpper->first;
		const ValueType& upperVal = itUpper->second;

		if (lowerTime == upperTime)
		{
			return lowerVal;
		}

		return interpolate(lowerTime, lowerVal, upperTime, upperVal, time);
	}

	void CDynamicValueController::Clear()
	{
		m_keyMap.clear();
	}

}