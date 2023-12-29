#pragma once

#include <bgfx/bgfx.h>

namespace da::platform
{
    class CBgfxMaterial;
    class CBgfxGraphicsMaterial;

    class CBgfxPBRShader
    {
    public:
        void initialize();
        void shutdown();

        void generateAlbedoLUT();

        uint64_t bindMaterial(const CBgfxMaterial& material);
        void bindAlbedoLUT(bool compute = false);

        static constexpr float WHITE_FURNACE_RADIANCE = 1.0f;

        bool m_multipleScatteringEnabled = true;
        bool m_whiteFurnaceEnabled = false;

    private:
        static constexpr uint16_t ALBEDO_LUT_SIZE = 32;
        static constexpr uint16_t ALBEDO_LUT_THREADS = 32;

        ::bgfx::UniformHandle m_baseColorFactorUniform = BGFX_INVALID_HANDLE;
        ::bgfx::UniformHandle m_metallicRoughnessNormalOcclusionFactorUniform = BGFX_INVALID_HANDLE;
        ::bgfx::UniformHandle m_emissiveFactorUniform = BGFX_INVALID_HANDLE;
        ::bgfx::UniformHandle m_hasTexturesUniform = BGFX_INVALID_HANDLE;
        ::bgfx::UniformHandle m_multipleScatteringUniform = BGFX_INVALID_HANDLE;
        ::bgfx::UniformHandle m_albedoLUTSampler = BGFX_INVALID_HANDLE;
        ::bgfx::UniformHandle m_baseColorSampler = BGFX_INVALID_HANDLE;
        ::bgfx::UniformHandle m_metallicRoughnessSampler = BGFX_INVALID_HANDLE;
        ::bgfx::UniformHandle m_normalSampler = BGFX_INVALID_HANDLE;
        ::bgfx::UniformHandle m_occlusionSampler = BGFX_INVALID_HANDLE;
        ::bgfx::UniformHandle m_emissiveSampler = BGFX_INVALID_HANDLE;

        ::bgfx::TextureHandle m_albedoLUTTexture = BGFX_INVALID_HANDLE;
        ::bgfx::TextureHandle m_defaultTexture = BGFX_INVALID_HANDLE;

        da::platform::CBgfxGraphicsMaterial* m_pAlbedoLUTProgram;
    };

}