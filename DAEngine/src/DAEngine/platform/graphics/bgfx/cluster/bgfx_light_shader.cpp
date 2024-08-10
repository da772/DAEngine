
#include "bgfx_light_shader.h"

#include "bgfx_samplers.h"
#include <glm/gtc/type_ptr.hpp>
#include <cassert>

namespace da::platform {

    void CBgfxLightShader::initialize()
    {
        m_lightCountVecUniform = bgfx::createUniform("u_lightCountVec", bgfx::UniformType::Vec4);
        m_ambientLightIrradianceUniform = bgfx::createUniform("u_ambientLightIrradiance", bgfx::UniformType::Vec4);
        m_sunLightDirectionUniform = bgfx::createUniform("u_sunLightDirection", bgfx::UniformType::Vec4);
        m_sunLightRadianceUniform = bgfx::createUniform("u_sunLightRadiance", bgfx::UniformType::Vec4);
    }

    void CBgfxLightShader::shutdown()
    {
        ASSERT(bgfx::isValid(m_lightCountVecUniform));
        bgfx::destroy(m_lightCountVecUniform);
        ASSERT(bgfx::isValid(m_ambientLightIrradianceUniform));
        bgfx::destroy(m_ambientLightIrradianceUniform);
        ASSERT(bgfx::isValid(m_sunLightDirectionUniform));
        bgfx::destroy(m_sunLightDirectionUniform);
        ASSERT(bgfx::isValid(m_sunLightRadianceUniform));
        bgfx::destroy(m_sunLightRadianceUniform);

        m_lightCountVecUniform = m_ambientLightIrradianceUniform = m_sunLightDirectionUniform = m_sunLightRadianceUniform = BGFX_INVALID_HANDLE;
    }

    void CBgfxLightShader::bindLights(const glm::vec3& sunLightDir, const glm::vec3& sunLightRad, const CBgfxAmbientLight& ambientLight, CBgfxPointLightList& pointLights) const
    {
        // a 32-bit IEEE 754 float can represent all integers up to 2^24 (~16.7 million) correctly
        // should be enough for this use case (comparison in for loop)
        float lightCountVec[4] = { (float)da::graphics::CLightManager::getLights().size() };
        bgfx::setUniform(m_lightCountVecUniform, lightCountVec);

        glm::vec4 sunLightDirection(glm::normalize(sunLightDir), 0.0f);
        bgfx::setUniform(m_sunLightDirectionUniform, glm::value_ptr(sunLightDirection));
        glm::vec4 sunLightRadiance(glm::max(glm::vec3(.0001), glm::normalize(sunLightRad)), 1.0f);
        bgfx::setUniform(m_sunLightRadianceUniform, glm::value_ptr(sunLightRadiance));

        glm::vec4 ambientLightIrradiance(ambientLight.irradiance, 1.0f);
        bgfx::setUniform(m_ambientLightIrradianceUniform, glm::value_ptr(ambientLightIrradiance));

        bgfx::setBuffer(CBgfxSamplers::LIGHTS_POINTLIGHTS, pointLights.getBuffer(), bgfx::Access::Read);
    }

}