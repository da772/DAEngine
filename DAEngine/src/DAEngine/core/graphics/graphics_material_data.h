#pragma once

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <string>

namespace da::graphics {
    // https://github.com/KhronosGroup/glTF/tree/master/specification/2.0#metallic-roughness-material
    struct FMaterialData
    {
        bool blend = false;
        bool doubleSided = false;

        std::string baseColorTexture = "";
        glm::vec4 baseColorFactor = { 1.0f, 1.0f, 1.0f, 1.0f };

        std::string metallicRoughnessTexture = ""; // blue = metallic, green = roughness
        float metallicFactor = 1.0f;
        float roughnessFactor = 1.0f;

        std::string normalTexture = "";
        float normalScale = 1.0f;

        std::string occlusionTexture = "";
        float occlusionStrength = 1.0f;

        std::string emissiveTexture = "";
        glm::vec3 emissiveFactor = { 0.0f, 0.0f, 0.0f };
    };

}