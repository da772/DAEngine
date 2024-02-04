#pragma once

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>

namespace da::graphics {

    class CGraphicsTexture2D;

    // https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#metallic-roughness-material
    struct FMaterialData
    {
        bool blend = false;
        bool doubleSided = false;

        CGraphicsTexture2D* baseColorTexture = nullptr;
        glm::vec4 baseColorFactor = { 1.0f, 1.0f, 1.0f, 1.0f };
        glm::vec2 uvScale = { 1.f,1.f };

        CGraphicsTexture2D* metallicRoughnessTexture = nullptr; // blue = metallic, green = roughness
        float metallicFactor = .5f;
        float roughnessFactor = .5f;

        CGraphicsTexture2D* normalTexture = nullptr;
        float normalScale = 1.0f;

        CGraphicsTexture2D* occlusionTexture = nullptr;
        float occlusionStrength = 1.0f;

        CGraphicsTexture2D* emissiveTexture = nullptr;
        glm::vec3 emissiveFactor = { 0.0f, 0.0f, 0.0f };
    };

}