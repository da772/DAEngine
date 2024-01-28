#pragma once

#include <bgfx/bgfx.h>

namespace da::platform
{
	class CBgfxGraphicsMaterial;

	class CDynamicValueController
	{
		typedef glm::vec3 ValueType;
		typedef std::map<float, ValueType> KeyMap;

	public:
		CDynamicValueController()
		{
		}

		~CDynamicValueController()
		{
		}

		void SetMap(const KeyMap& keymap)
		{
			m_keyMap = keymap;
		}

		ValueType GetValue(float time) const;

		void Clear();

	private:
		ValueType interpolate(float lowerTime, const ValueType& lowerVal, float upperTime, const ValueType& upperVal, float time) const;

		KeyMap m_keyMap;
	};

	struct FScreenPosVertex
	{
		float m_x;
		float m_y;

		static void init()
		{
			ms_layout
				.begin()
				.add(::bgfx::Attrib::Position, 2, ::bgfx::AttribType::Float)
				.end();
		}

		static ::bgfx::VertexLayout ms_layout;
	};

	class CBgfxSunController
	{
	public:
		enum Month : int
		{
			January = 0,
			February,
			March,
			April,
			May,
			June,
			July,
			August,
			September,
			October,
			November,
			December
		};

		CBgfxSunController();
		void Update(float _time);

		glm::vec3 m_northDir;
		glm::vec3 m_sunDir;
		glm::vec3 m_upDir;
		float m_latitude;
		Month m_month;

	private:
		void CalculateSunOrbit();
		void UpdateSunPosition(float _hour);

		float m_eclipticObliquity;
		float m_delta;
		float m_time;
	};

	class CBgfxProcSky
	{
	public:
		void initialize(int vertCount, int horzCount);
		void render(::bgfx::ViewId id, uint64_t state);
		void setUniforms(const CBgfxSunController& sun, float time);
		void shutdown();

	private:
		void computePerezCoeff(float _turbidity, float* _outPerezCoeff);
		glm::vec3 xyzToRgb(const glm::vec3& xyz);

	private:

		CDynamicValueController m_sunLuminanceXYZ;
		CDynamicValueController m_skyLuminanceXYZ;
		CBgfxGraphicsMaterial* m_skyProgram = nullptr;
		CBgfxGraphicsMaterial* m_skyProgramFix = nullptr;
		::bgfx::VertexBufferHandle m_vbh = BGFX_INVALID_HANDLE;
		::bgfx::IndexBufferHandle m_ibh = BGFX_INVALID_HANDLE;
		::bgfx::UniformHandle u_skyLuminanceXYZ = BGFX_INVALID_HANDLE, u_skyLuminance = BGFX_INVALID_HANDLE, 
			u_sunDirection = BGFX_INVALID_HANDLE, u_parameters = BGFX_INVALID_HANDLE, u_perezCoeff = BGFX_INVALID_HANDLE , u_sunLuminance = BGFX_INVALID_HANDLE;
	};

	
}