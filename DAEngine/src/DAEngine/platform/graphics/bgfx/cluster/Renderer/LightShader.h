#pragma once

#include <bgfx/bgfx.h>
#include "../Scene/Light.h"
#include "../Scene/LightList.h"

class LightShader
{
public:
    void initialize();
    void shutdown();

    void bindLights(const SunLight& sunLight, const AmbientLight& ambientLight, PointLightList& pointLights) const;

private:
    bgfx::UniformHandle lightCountVecUniform = BGFX_INVALID_HANDLE;
    bgfx::UniformHandle ambientLightIrradianceUniform = BGFX_INVALID_HANDLE;
    bgfx::UniformHandle sunLightDirectionUniform = BGFX_INVALID_HANDLE;
    bgfx::UniformHandle sunLightRadianceUniform = BGFX_INVALID_HANDLE;
};
