#include "dapch.h"
#include "bgfx_pbr_shader.h"

#include "bgfx_material.h"
#include "bgfx_samplers.h"
#include <bx/string.h>
#include <bimg/encode.h>
#include <bx/file.h>
#include <glm/gtc/type_ptr.hpp>
#include "core/graphics/graphics_texture2d.h"
#include "platform\graphics\bgfx\bgfx_graphics_material.h"
#include "bgfx_shadow_shader.h"

namespace da::platform {

    void CBgfxPBRShader::initialize()
    {
        m_baseColorFactorUniform = bgfx::createUniform("u_baseColorFactor", bgfx::UniformType::Vec4);
        m_metallicRoughnessNormalOcclusionFactorUniform =
            bgfx::createUniform("u_metallicRoughnessNormalOcclusionFactor", bgfx::UniformType::Vec4);
        m_emissiveFactorUniform = bgfx::createUniform("u_emissiveFactorVec", bgfx::UniformType::Vec4);
        m_hasTexturesUniform = bgfx::createUniform("u_hasTextures", bgfx::UniformType::Vec4);
        m_multipleScatteringUniform = bgfx::createUniform("u_multipleScatteringVec", bgfx::UniformType::Vec4);
        m_albedoLUTSampler = bgfx::createUniform("s_texAlbedoLUT", bgfx::UniformType::Sampler);
        m_baseColorSampler = bgfx::createUniform("s_texBaseColor", bgfx::UniformType::Sampler);
        m_metallicRoughnessSampler = bgfx::createUniform("s_texMetallicRoughness", bgfx::UniformType::Sampler);
        m_normalSampler = bgfx::createUniform("s_texNormal", bgfx::UniformType::Sampler);
        m_occlusionSampler = bgfx::createUniform("s_texOcclusion", bgfx::UniformType::Sampler);
        m_emissiveSampler = bgfx::createUniform("s_texEmissive", bgfx::UniformType::Sampler);
        m_uvScaleUniform = bgfx::createUniform("s_uvScale", bgfx::UniformType::Vec4);
        m_lightMtx = bgfx::createUniform("u_sunLightMtx", bgfx::UniformType::Mat4, SHADOW_MAP_SIZE);

        m_defaultTexture = bgfx::createTexture2D(1, 1, false, 1, bgfx::TextureFormat::RGBA8);
        m_albedoLUTTexture = bgfx::createTexture2D(ALBEDO_LUT_SIZE,
            ALBEDO_LUT_SIZE,
            false,
            1,
            bgfx::TextureFormat::RGBA32F,
            BGFX_SAMPLER_UVW_CLAMP | BGFX_TEXTURE_COMPUTE_WRITE);

        m_pAlbedoLUTProgram = new da::platform::CBgfxGraphicsMaterial("shaders/cluster/cs_multiple_scattering_lut.sc");
        m_pAlbedoLUTProgram->initialize();
    }

    void CBgfxPBRShader::shutdown()
    {
        ASSERT(::bgfx::isValid(m_baseColorFactorUniform));
        bgfx::destroy(m_baseColorFactorUniform);
        ASSERT(::bgfx::isValid(m_metallicRoughnessNormalOcclusionFactorUniform));
        bgfx::destroy(m_metallicRoughnessNormalOcclusionFactorUniform);
        ASSERT(::bgfx::isValid(m_emissiveFactorUniform));
        bgfx::destroy(m_emissiveFactorUniform);
        ASSERT(::bgfx::isValid(m_hasTexturesUniform));
        bgfx::destroy(m_hasTexturesUniform);
        ASSERT(::bgfx::isValid(m_multipleScatteringUniform));
        bgfx::destroy(m_multipleScatteringUniform);
        ASSERT(::bgfx::isValid(m_albedoLUTSampler));
        bgfx::destroy(m_albedoLUTSampler);
        ASSERT(::bgfx::isValid(m_baseColorSampler));
        bgfx::destroy(m_baseColorSampler);
        ASSERT(::bgfx::isValid(m_metallicRoughnessSampler));
        bgfx::destroy(m_metallicRoughnessSampler);
        ASSERT(::bgfx::isValid(m_normalSampler));
        bgfx::destroy(m_normalSampler);
        ASSERT(::bgfx::isValid(m_occlusionSampler));
        bgfx::destroy(m_occlusionSampler);
        ASSERT(::bgfx::isValid(m_emissiveSampler));
        bgfx::destroy(m_emissiveSampler);
        ASSERT(::bgfx::isValid(m_albedoLUTTexture));
        bgfx::destroy(m_albedoLUTTexture);
        ASSERT(::bgfx::isValid(m_defaultTexture));
        bgfx::destroy(m_defaultTexture);
        ASSERT(::bgfx::isValid(m_lightMtx));
		bgfx::destroy(m_lightMtx); 
        ASSERT(::bgfx::isValid(m_uvScaleUniform));
		bgfx::destroy(m_uvScaleUniform);

        m_pAlbedoLUTProgram->shutdown();
        delete m_pAlbedoLUTProgram;
        m_pAlbedoLUTProgram = nullptr;

        m_baseColorFactorUniform = m_metallicRoughnessNormalOcclusionFactorUniform = m_emissiveFactorUniform =
            m_hasTexturesUniform = m_multipleScatteringUniform = m_albedoLUTSampler = m_baseColorSampler =
            m_metallicRoughnessSampler = m_normalSampler = m_occlusionSampler = m_emissiveSampler = m_lightMtx = m_uvScaleUniform = BGFX_INVALID_HANDLE;
        m_albedoLUTTexture = m_defaultTexture = BGFX_INVALID_HANDLE;
    }

    void CBgfxPBRShader::generateAlbedoLUT()
    {
        bindAlbedoLUT(true /* compute */);
        bgfx::dispatch(0, {m_pAlbedoLUTProgram->getHandle()}, ALBEDO_LUT_SIZE / ALBEDO_LUT_THREADS, ALBEDO_LUT_SIZE / ALBEDO_LUT_THREADS, 1);
    }

    uint64_t CBgfxPBRShader::bindMaterial(const da::graphics::FMaterialData& material)
    {
        float factorValues[4] = {
            material.metallicFactor, material.roughnessFactor, material.normalScale, material.occlusionStrength
        };
        bgfx::setUniform(m_baseColorFactorUniform, glm::value_ptr(material.baseColorFactor));
        bgfx::setUniform(m_metallicRoughnessNormalOcclusionFactorUniform, factorValues);
        glm::vec4 emissiveFactor = glm::vec4(material.emissiveFactor, 0.0f);
        bgfx::setUniform(m_emissiveFactorUniform, glm::value_ptr(emissiveFactor));
        glm::vec4 uvScale = { material.uvScale.x, material.uvScale.y, 1.f,1.f };
        bgfx::setUniform(m_uvScaleUniform, glm::value_ptr(uvScale));

        float hasTexturesValues[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

        auto setTextureOrDefault = [&](uint8_t stage, bgfx::UniformHandle uniform, da::graphics::CGraphicsTexture2D* texture2d) -> bool {
            bgfx::TextureHandle texture = m_defaultTexture;

            if (!texture2d) {
                return false;
            }

            if (texture2d && texture2d->getTextureNative()) {
                texture = *(bgfx::TextureHandle*)(texture2d->getTextureNative());;
            }
            bool valid = bgfx::isValid(texture);
            if (!valid)
                texture = m_defaultTexture;
            bgfx::setTexture(stage, uniform, texture);
            return valid;
            };

        const uint32_t hasTexturesMask = 0
			| ((setTextureOrDefault(CBgfxSamplers::PBR_BASECOLOR, m_baseColorSampler, material.getBaseColorTexture()) ? 1 : 0) << 0)
			| ((setTextureOrDefault(CBgfxSamplers::PBR_METALROUGHNESS, m_metallicRoughnessSampler, material.getMetallicRoughnessTexture()) ? 1 : 0) << 1)
			| ((setTextureOrDefault(CBgfxSamplers::PBR_NORMAL, m_normalSampler, material.getNormalTexture()) ? 1 : 0) << 2)
			| ((setTextureOrDefault(CBgfxSamplers::PBR_OCCLUSION, m_occlusionSampler, material.getOcclusionTexture()) ? 1 : 0) << 3)
			| ((setTextureOrDefault(CBgfxSamplers::PBR_EMISSIVE, m_emissiveSampler, material.getEmissiveTexture()) ? 1 : 0) << 4);
        hasTexturesValues[0] = static_cast<float>(hasTexturesMask);

        bgfx::setUniform(m_hasTexturesUniform, hasTexturesValues);

        float multipleScatteringValues[4] = {
            m_multipleScatteringEnabled ? 1.0f : 0.0f, m_whiteFurnaceEnabled ? WHITE_FURNACE_RADIANCE : 0.0f, 0.0f, 0.0f
        };
        bgfx::setUniform(m_multipleScatteringUniform, multipleScatteringValues);

        uint64_t state = 0;
        if (material.blend)
            state |= BGFX_STATE_BLEND_ALPHA;
        if (!material.doubleSided)
            state |= BGFX_STATE_CULL_CCW;
        return state;
    }

    void CBgfxPBRShader::bindAlbedoLUT(bool compute)
    {
        if (compute)
            bgfx::setImage(CBgfxSamplers::PBR_ALBEDO_LUT, m_albedoLUTTexture, 0, bgfx::Access::Write);
        else
            bgfx::setTexture(CBgfxSamplers::PBR_ALBEDO_LUT, m_albedoLUTSampler, m_albedoLUTTexture);
    }

	void CBgfxPBRShader::bindLightPos(const glm::vec3& pos, const glm::mat4* mtx)
	{
        glm::vec4 p(pos, 1.0);
        bgfx::setUniform(m_lightMtx, &(mtx[0]), SHADOW_MAP_SIZE);
	}

}