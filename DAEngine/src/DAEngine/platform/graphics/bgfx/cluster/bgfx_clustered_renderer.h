#pragma once

#include "bgfx_type_renderer.h"
#include "bgfx_cluster_shader.h"
#include "bgfx_light_list.h"
#include "bgfx_shadow_shader.h"
#include "bgfx_ssao_shader.h"

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

    protected:
        virtual void onReset(size_t width, size_t height) override;
    private:
        void generateLights(uint32_t count);

#ifdef DA_DEBUG
        void renderLightDebug();
#endif
    private:
        glm::mat4 m_oldProjMat = glm::mat4(0.0f);

        da::graphics::CMaterial* m_pClusterBuildingComputeProgram;
        da::graphics::CMaterial* m_pResetCounterComputeProgram;
        da::graphics::CMaterial* m_pLightCullingComputeProgram;
        da::graphics::CMaterial* m_pLightingProgram;
        da::graphics::CMaterial* m_pLightingSkeletalProgram;
        da::graphics::CMaterial* m_pDebugVisProgram;

        CBgfxAmbientLight m_ambientLight;
        CBgfxPointLightList m_pointLights;
        CBgfxClusterShader m_clusters;
        CBgfxShadowShader m_shadow;
        CBgfxSSAOShader m_ssao;

#ifdef DA_DEBUG
        bool m_clusterDebugVis = false;
        bool m_lightDebugVis = false;
#endif
    };
}