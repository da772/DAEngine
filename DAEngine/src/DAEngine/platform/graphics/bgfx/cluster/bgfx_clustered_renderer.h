#pragma once

#include "bgfx_type_renderer.h"
#include "bgfx_cluster_shader.h"
#include "bgfx_light_list.h"

namespace da::platform
{
    class CBgfxGraphicsMaterial;

    class CBgfxClusteredRenderer : public CBgfxTypeRenderer
    {
    public:
        CBgfxClusteredRenderer();

        static bool supported();

        virtual void onInitialize() override;
        virtual void onRender(float dt) override;
        virtual void onShutdown() override;

    private:
        void generateLights(uint32_t count);
#ifdef DA_DEBUG
        void renderLightDebug();
#endif
    private:
        glm::mat4 m_oldProjMat = glm::mat4(0.0f);

        da::platform::CBgfxGraphicsMaterial* m_pClusterBuildingComputeProgram;
        da::platform::CBgfxGraphicsMaterial* m_pResetCounterComputeProgram;
        da::platform::CBgfxGraphicsMaterial* m_pLightCullingComputeProgram;
        da::platform::CBgfxGraphicsMaterial* m_pLightingProgram;
        da::platform::CBgfxGraphicsMaterial* m_pDebugVisProgram;

        CBgfxAmbientLight m_ambientLight;
        CBgfxSunLight m_sunLight;
        CBgfxPointLightList m_pointLights;
        CBgfxClusterShader clusters;

#ifdef DA_DEBUG
        bool m_clusterDebugVis = false;
        bool m_lightDebugVis = false;
#endif
    };
}