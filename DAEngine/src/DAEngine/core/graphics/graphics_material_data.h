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

        
        glm::vec4 baseColorFactor = { 1.0f, 1.0f, 1.0f, 1.0f };
        glm::vec2 uvScale = { 1.f,1.f };
        float metallicFactor = .5f;
        float roughnessFactor = .5f;
        float normalScale = 1.0f;
        float occlusionStrength = 1.0f;
        glm::vec3 emissiveFactor = { 0.0f, 0.0f, 0.0f };

		void setBaseColorTexture(CGraphicsTexture2D* texture);
		void setEmissiveTexture(CGraphicsTexture2D* texture);
		void setOcclusionTexture(CGraphicsTexture2D* texture);
		void setMetallicRoughnessTexture(CGraphicsTexture2D* texture);
		void setNormalTexture(CGraphicsTexture2D* texture);


        CGraphicsTexture2D* getBaseColorTexture() const;
        CGraphicsTexture2D* getEmissiveTexture() const;
        CGraphicsTexture2D* getOcclusionTexture() const;
        CGraphicsTexture2D* getMetallicRoughnessTexture() const;
        CGraphicsTexture2D* getNormalTexture() const;

    public:
        CGraphicsTexture2D* m_baseColorTexture = nullptr;
        CGraphicsTexture2D* m_emissiveTexture = nullptr;
        CGraphicsTexture2D* m_occlusionTexture = nullptr;
        CGraphicsTexture2D* m_normalTexture = nullptr;
        CGraphicsTexture2D* m_metallicRoughnessTexture = nullptr;

    };

}