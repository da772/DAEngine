#pragma once

#include <bgfx/bgfx.h>
#include "../Scene/LightList.h"

class LightShader
{
public:
    void initialize();
    void shutdown();

    void bindLights(PointLightList& pointLights) const;

private:
    bgfx::UniformHandle lightCountVecUniform = BGFX_INVALID_HANDLE;
    bgfx::UniformHandle ambientLightIrradianceUniform = BGFX_INVALID_HANDLE;
};
