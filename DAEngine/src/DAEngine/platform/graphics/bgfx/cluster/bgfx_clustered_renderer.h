#pragma once

#include "bgfx_type_renderer.h"
#include "bgfx_cluster_shader.h"
#include "bgfx_light_list.h"
#include "bgfx_shadow_shader.h"
#include "bgfx_ssao_shader.h"
#include <core/graphics/graphics_light_manager.h>

namespace da::platform
{
    class CBgfxGraphicsMaterial;

    enum ERenderFlags : uint8_t {
        None = 0x00,
        PBR = 0x01,
        ShadowPass = 0x02
    };

    class CBgfxClusteredRenderer : public CBgfxTypeRenderer
    {
    public:
        CBgfxClusteredRenderer();

        static bool supported();

        virtual void onInitialize() override;
        virtual void onRender(float dt) override;
        virtual void onShutdown() override;

    protected:
        virtual void onReset(size_t width, size_t height) override;
    private:
        void onLightEvent(const da::graphics::FLightData& light, bool added);
        void renderFunc(uint8_t view, da::graphics::CMaterial* mat, da::graphics::CMaterial* instanceMat, da::graphics::CMaterial* skMat, da::platform::RenderState renderState, ERenderFlags useMaterials);

#ifdef DA_REVIEW
        void renderLightDebug();
#endif
    private:
        glm::mat4 m_oldProjMat = glm::mat4(0.0f);

        da::graphics::CMaterial* m_pClusterBuildingComputeProgram;
        da::graphics::CMaterial* m_pResetCounterComputeProgram;
        da::graphics::CMaterial* m_pLightCullingComputeProgram;
        da::graphics::CMaterial* m_pLightingProgram;
        da::graphics::CMaterial* m_pLightingInstanceProgram;
        da::graphics::CMaterial* m_pLightingSkeletalProgram;
        da::graphics::CMaterial* m_pDebugVisProgram;

        CBgfxAmbientLight m_ambientLight;
        CBgfxPointLightList m_pointLights;
        CBgfxClusterShader m_clusters;
        CBgfxSSAOShader m_ssao;

#ifdef DA_REVIEW
        bool m_clusterDebugVis = false;
        bool m_lightDebugVis = false;
#endif
    };
}