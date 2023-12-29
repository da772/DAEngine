#pragma once

#include "Renderer.h"
#include "ClusterShader.h"
#include "../Scene/LightList.h"

namespace da::platform
{
    class CBgfxGraphicsMaterial;
}

class ClusteredRenderer : public Renderer
{
public:
    ClusteredRenderer();

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
    glm::mat4 oldProjMat = glm::mat4(0.0f);

    bgfx::ProgramHandle clusterBuildingComputeProgram = BGFX_INVALID_HANDLE;
    bgfx::ProgramHandle resetCounterComputeProgram = BGFX_INVALID_HANDLE;
    bgfx::ProgramHandle lightCullingComputeProgram = BGFX_INVALID_HANDLE;
    bgfx::ProgramHandle lightingProgram = BGFX_INVALID_HANDLE;
    bgfx::ProgramHandle debugVisProgram = BGFX_INVALID_HANDLE;
    da::platform::CBgfxGraphicsMaterial* m_pClusterBuildingComputeProgram;
    da::platform::CBgfxGraphicsMaterial* m_pResetCounterComputeProgram;
    da::platform::CBgfxGraphicsMaterial* m_pLightCullingComputeProgram;
    da::platform::CBgfxGraphicsMaterial* m_pLightingProgram;
    da::platform::CBgfxGraphicsMaterial* m_pDebugVisProgram;

    AmbientLight m_ambientLight;
    SunLight m_sunLight;
    PointLightList m_pointLights;
    ClusterShader clusters;

#ifdef DA_DEBUG
    bool m_clusterDebugVis = false;
    bool m_lightDebugVis = false;
#endif
};
