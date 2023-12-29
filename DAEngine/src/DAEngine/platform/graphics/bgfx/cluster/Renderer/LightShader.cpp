#include "dapch.h"
#include "LightShader.h"

#include "Samplers.h"
#include <glm/gtc/type_ptr.hpp>
#include <cassert>

void LightShader::initialize()
{
    lightCountVecUniform = bgfx::createUniform("u_lightCountVec", bgfx::UniformType::Vec4);
    ambientLightIrradianceUniform = bgfx::createUniform("u_ambientLightIrradiance", bgfx::UniformType::Vec4);
    sunLightDirectionUniform = bgfx::createUniform("u_sunLightDirection", bgfx::UniformType::Vec4);
    sunLightRadianceUniform = bgfx::createUniform("u_sunLightRadiance", bgfx::UniformType::Vec4);
}

void LightShader::shutdown()
{
    bgfx::destroy(lightCountVecUniform);
    bgfx::destroy(ambientLightIrradianceUniform);
    bgfx::destroy(sunLightDirectionUniform);
    bgfx::destroy(sunLightRadianceUniform);

    lightCountVecUniform = ambientLightIrradianceUniform = BGFX_INVALID_HANDLE;
}

void LightShader::bindLights(const SunLight& sunLight, const AmbientLight& ambientLight, PointLightList& pointLights) const
{
    // a 32-bit IEEE 754 float can represent all integers up to 2^24 (~16.7 million) correctly
    // should be enough for this use case (comparison in for loop)
    float lightCountVec[4] = { (float)pointLights.lights.size() };
    bgfx::setUniform(lightCountVecUniform, lightCountVec);

	glm::vec4 sunLightDirection(glm::normalize(sunLight.direction), 0.0f);
	bgfx::setUniform(sunLightDirectionUniform, glm::value_ptr(sunLightDirection));
	glm::vec4 sunLightRadiance(sunLight.radiance, 1.0f);
	bgfx::setUniform(sunLightRadianceUniform, glm::value_ptr(sunLightRadiance));
    
    glm::vec4 ambientLightIrradiance(ambientLight.irradiance, 1.0f);
    bgfx::setUniform(ambientLightIrradianceUniform, glm::value_ptr(ambientLightIrradiance));

    bgfx::setBuffer(Samplers::LIGHTS_POINTLIGHTS, pointLights.buffer, bgfx::Access::Read);
}
