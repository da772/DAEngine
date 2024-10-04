
#ifdef DA_GRAPHICS_BGFX

#include "bgfx_light_shader.h"

#define LIGHTS_POINTLIGHTS 6

namespace da {

	void CBgfxLightShader::initialize()
	{
		m_lightCountVecUniform = CBgfxUniform<Vector4f>(::bgfx::UniformType::Vec4, "u_lightCountVec");
		m_ambientLightIrradianceUniform = CBgfxUniform<Vector4f>(::bgfx::UniformType::Vec4, "u_ambientLightIrradiance");
	}

	void CBgfxLightShader::shutdown()
	{
		m_lightCountVecUniform.destroy();
		m_ambientLightIrradianceUniform.destroy();
	}

	void CBgfxLightShader::bindLights(uint32_t lightCount, float irradiance)
	{
		float lightCountVec[4] = { (float)lightCount };

		m_lightCountVecUniform.setData(lightCountVec);

		Vector4f ambientLightIrradiance(irradiance, 1.f, 1.f, 1.f);
		m_ambientLightIrradianceUniform.setData(ambientLightIrradiance);

		//::bgfx::setBuffer(Samplers::LIGHTS_POINTLIGHTS, scene->pointLights.buffer, bgfx::Access::Read);

	}

}
#endif