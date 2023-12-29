#pragma once

#include <bgfx/bgfx.h>
#include "bgfx_light.h"
#include "bgfx_light_list.h"

namespace da::platform {

    class CBgfxLightShader
    {
    public:
        void initialize();
        void shutdown();

        void bindLights(const CBgfxSunLight& sunLight, const CBgfxAmbientLight& ambientLight, CBgfxPointLightList& pointLights) const;

    private:
        ::bgfx::UniformHandle m_lightCountVecUniform = BGFX_INVALID_HANDLE;
        ::bgfx::UniformHandle m_ambientLightIrradianceUniform = BGFX_INVALID_HANDLE;
        ::bgfx::UniformHandle m_sunLightDirectionUniform = BGFX_INVALID_HANDLE;
        ::bgfx::UniformHandle m_sunLightRadianceUniform = BGFX_INVALID_HANDLE;
    };

}