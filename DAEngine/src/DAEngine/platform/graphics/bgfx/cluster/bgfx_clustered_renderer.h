#pragma once

#include "bgfx_type_renderer.h"
#include "bgfx_cluster_shader.h"
#include "bgfx_light_list.h"
#include "bgfx_shadow_shader.h"
#include "bgfx_ssao_shader.h"
#include <graphics/graphics_light_manager.h>

namespace da
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
        void onLightEvent(const da::FLightData& light, bool added);
        void renderFunc(uint8_t view, da::CMaterial* mat, da::CMaterial* instanceMat, da::CMaterial* skMat, da::RenderState renderState, ERenderFlags useMaterials);

#ifdef DA_REVIEW
        void renderLightDebug();
#endif
    private:
        glm::mat4 m_oldProjMat = glm::mat4(0.0f);

        da::CMaterial* m_pClusterBuildingComputeProgram;
        da::CMaterial* m_pResetCounterComputeProgram;
        da::CMaterial* m_pLightCullingComputeProgram;
        da::CMaterial* m_pLightingProgram;
        da::CMaterial* m_pLightingInstanceProgram;
        da::CMaterial* m_pLightingSkeletalProgram;
        da::CMaterial* m_pDebugVisProgram;

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