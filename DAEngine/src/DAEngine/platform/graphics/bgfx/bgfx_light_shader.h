#pragma once


#include "bgfx_uniform.h"
#include <glm/glm.hpp>

namespace da
{
	class CBgfxLightShader {

	public:
		void initialize();
		void shutdown();

		void bindLights(uint32_t lightCount, float irradiance);

	private:
		CBgfxUniform<Vector4f> m_lightCountVecUniform;
		CBgfxUniform<Vector4f> m_ambientLightIrradianceUniform;


	};
}