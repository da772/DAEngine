#include "dapch.h"
#include "bgfx_clustered_renderer.h"

#include <bx/string.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_relational.hpp>
#include <platform/graphics/bgfx/bgfx_graphics_material.h>
#include <core/ecs/scene_manager.h>
#include <core/ecs/smesh_component.h>
#include "platform/graphics/bgfx/bgfx_static_mesh.h"
#include "platform/graphics/bgfx/bgfx_graphics_pbr_material.h"
#include <random>
#include "logger.h"

#ifdef DA_DEBUG
#include "debug/debug_menu_bar.h"
#endif


namespace da::platform {

    CBgfxClusteredRenderer::CBgfxClusteredRenderer()
    {

    }

    bool CBgfxClusteredRenderer::supported()
    {
        const ::bgfx::Caps* caps = ::bgfx::getCaps();
        return CBgfxTypeRenderer::supported() &&
            // compute shader
            (caps->supported & BGFX_CAPS_COMPUTE) != 0 &&
            // 32-bit index buffers, used for light grid structure
            (caps->supported & BGFX_CAPS_INDEX32) != 0;
    }

    void CBgfxClusteredRenderer::onInitialize()
    {
        // OpenGL backend: uniforms must be created before loading shaders
        m_clusters.initialize();

        m_pClusterBuildingComputeProgram = new da::platform::CBgfxGraphicsMaterial("shaders/cluster/cs_clustered_clusterbuilding.sc");
        m_pClusterBuildingComputeProgram->initialize();

        m_pResetCounterComputeProgram = new da::platform::CBgfxGraphicsMaterial("shaders/cluster/cs_clustered_reset_counter.sc");
        m_pResetCounterComputeProgram->initialize();

        m_pLightCullingComputeProgram = new da::platform::CBgfxGraphicsMaterial("shaders/cluster/cs_clustered_lightculling.sc");
        m_pLightCullingComputeProgram->initialize();

        m_pLightingProgram = new da::platform::CBgfxGraphicsMaterial("shaders/cluster/vs_clustered.sc", "shaders/cluster/fs_clustered.sc");
        m_pLightingProgram->initialize();

        m_pDebugVisProgram = new da::platform::CBgfxGraphicsMaterial("shaders/cluster/vs_clustered.sc", "shaders/cluster/fs_clustered_debug_vis.sc");
        m_pDebugVisProgram->initialize();

        m_pointLights.init();
        generateLights(3);
        m_pointLights.update();

        m_ambientLight.irradiance = { 0.03f, 0.03f, 0.03f };
        m_sunLight.direction = { 0.0f, -1.f, -1.f };
        m_sunLight.radiance = { 1.f,1.f,1.f };

#ifdef DA_DEBUG
        da::debug::CDebugMenuBar::register_debug(HASHSTR("Renderer"), HASHSTR("ClusteredLightView"), &m_clusterDebugVis, [&] {});
        da::debug::CDebugMenuBar::register_debug(HASHSTR("Renderer"), HASHSTR("Light Debug"), &m_lightDebugVis, [&] { renderLightDebug(); });
#endif
    }

    void CBgfxClusteredRenderer::onRender(float dt)
    {
        enum : ::bgfx::ViewId
        {
            vClusterBuilding = 0,
            vLightCulling,
            vLighting
        };

        ::bgfx::setViewName(vClusterBuilding, "Cluster building pass (compute)");
        // set u_viewRect for screen2Eye to work correctly
        ::bgfx::setViewRect(vClusterBuilding, 0, 0, m_width, m_height);

        ::bgfx::setViewName(vLightCulling, "Clustered light culling pass (compute)");
        ::bgfx::setViewRect(vLightCulling, 0, 0, m_width, m_height);

        ::bgfx::setViewName(vLighting, "Clustered lighting pass");
        ::bgfx::setViewClear(vLighting, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, m_clearColor, 1.0f, 0);
        ::bgfx::setViewRect(vLighting, 0, 0, m_width, m_height);
        ::bgfx::setViewFrameBuffer(vLighting, m_frameBuffer);
        ::bgfx::touch(vLighting);

        m_clusters.setUniforms(m_width, m_height);

        // cluster building needs u_invProj to transform screen coordinates to eye space
        setViewProjection(vClusterBuilding);
        // light culling needs u_view to transform lights to eye space
        setViewProjection(vLightCulling);
        setViewProjection(vLighting);

        // cluster building

        // only run this step if the camera parameters changed (aspect ratio, fov, near/far plane)
        // cluster bounds are saved in camera coordinates so they don't change with camera movement

        // ideally we'd compare the relative error here but a correct implementation would involve
        // a bunch of costly matrix operations: https://floating-point-gui.de/errors/comparison/
        // comparing the absolute error against a rather small epsilon here works as long as the values
        // in the projection matrix aren't getting too large
        bool buildClusters = glm::any(glm::notEqual(m_projMat, m_oldProjMat, 0.00001f));
        if (buildClusters)
        {
            m_oldProjMat = m_projMat;

            m_clusters.bindBuffers(false /*lightingPass*/); // write access, all buffers

            ::bgfx::dispatch(vClusterBuilding,
                { m_pClusterBuildingComputeProgram->getHandle() },
                CBgfxClusterShader::CLUSTERS_X / CBgfxClusterShader::CLUSTERS_X_THREADS,
                CBgfxClusterShader::CLUSTERS_Y / CBgfxClusterShader::CLUSTERS_Y_THREADS,
                CBgfxClusterShader::CLUSTERS_Z / CBgfxClusterShader::CLUSTERS_Z_THREADS);
        }

        // light culling

        m_clusters.bindBuffers(false);

        // reset atomic counter for light grid generation
        // buffers created with BGFX_BUFFER_COMPUTE_WRITE can't be updated from the CPU
        // this used to happen during cluster building when it was still run every frame
        ::bgfx::dispatch(vLightCulling, { m_pResetCounterComputeProgram->getHandle() }, 1, 1, 1);

        m_lights.bindLights(m_sunLight, m_ambientLight, m_pointLights);
        m_clusters.bindBuffers(false);

        ::bgfx::dispatch(vLightCulling,
            { m_pLightCullingComputeProgram->getHandle() },
            CBgfxClusterShader::CLUSTERS_X / CBgfxClusterShader::CLUSTERS_X_THREADS,
            CBgfxClusterShader::CLUSTERS_Y / CBgfxClusterShader::CLUSTERS_Y_THREADS,
            CBgfxClusterShader::CLUSTERS_Z / CBgfxClusterShader::CLUSTERS_Z_THREADS);
        // lighting
#ifdef DA_DEBUG
        ::bgfx::ProgramHandle program = { (m_clusterDebugVis ? m_pDebugVisProgram->getHandle() : m_pLightingProgram->getHandle()) };
#else
        ::bgfx::ProgramHandle program =  { m_pLightingProgram->getHandle() };
#endif

        uint64_t state = BGFX_STATE_DEFAULT & ~BGFX_STATE_CULL_MASK;

        m_pbr.bindAlbedoLUT();
        m_lights.bindLights(m_sunLight, m_ambientLight, m_pointLights);
        m_clusters.bindBuffers(true /*lightingPass*/); // read access, only light grid and indices


        da::core::CScene* scene = da::core::CSceneManager::getScene();

        const da::core::FComponentContainer& container = scene->getComponents<da::core::CSmeshComponent>();

        for (size_t i = 0; i < container.getCount(); i++) {
            da::core::CSmeshComponent* mesh = container.getComponentAtIndex<da::core::CSmeshComponent>(i);
            glm::mat4 model = mesh->getParent().getTransform().matrix();
            ::bgfx::setTransform(glm::value_ptr(model));
            setNormalMatrix(model);
            ::bgfx::setVertexBuffer(0, *((::bgfx::VertexBufferHandle*)mesh->getStaticMesh()->getNativeVB()));
            ::bgfx::setIndexBuffer(*((::bgfx::IndexBufferHandle*)mesh->getStaticMesh()->getNativeIB()));
            da::platform::CBgfxPbrMaterial* material = (da::platform::CBgfxPbrMaterial*)mesh->getMaterial();
            uint64_t materialState = m_pbr.bindMaterial(*material->getMaterial());
            //BGFX_STATE_PT_LINES
            ::bgfx::setState(state | materialState);
            // preserve buffer bindings between submit calls
            ::bgfx::submit(vLighting, program, 0, ~BGFX_DISCARD_BINDINGS);
        }

        /*
        for(const Mesh& mesh : scene->meshes)
        {
            glm::mat4 model = glm::identity<glm::mat4>();
            bgfx::setTransform(glm::value_ptr(model));
            setNormalMatrix(model);
            bgfx::setVertexBuffer(0, mesh.vertexBuffer);
            bgfx::setIndexBuffer(mesh.indexBuffer);
            const Material& mat = scene->materials[mesh.material];
            uint64_t materialState = pbr.bindMaterial(mat);
            bgfx::setState(state | materialState);
            // preserve buffer bindings between submit calls
            bgfx::submit(vLighting, program, 0, ~BGFX_DISCARD_BINDINGS);
        }
        */

        ::bgfx::discard(BGFX_DISCARD_ALL);
    }

    void CBgfxClusteredRenderer::onShutdown()
    {
        m_clusters.shutdown();
		m_pClusterBuildingComputeProgram->shutdown();
		m_pResetCounterComputeProgram->shutdown();
		m_pLightCullingComputeProgram->shutdown();
		m_pLightingProgram->shutdown();
		m_pDebugVisProgram->shutdown();

        delete m_pClusterBuildingComputeProgram;
        delete m_pResetCounterComputeProgram;
        delete m_pLightCullingComputeProgram;
        delete m_pLightingProgram;
        delete m_pDebugVisProgram;

        m_pointLights.shutdown();

        m_pClusterBuildingComputeProgram = m_pResetCounterComputeProgram = m_pLightCullingComputeProgram = m_pLightingProgram =
            m_pDebugVisProgram = nullptr;

#ifdef DA_DEBUG
        da::debug::CDebugMenuBar::unregister_debug(HASHSTR("Renderer"), HASHSTR("ClusteredLightView"));
        da::debug::CDebugMenuBar::unregister_debug(HASHSTR("Renderer"), HASHSTR("Light Debug"));
#endif
    }

    void CBgfxClusteredRenderer::generateLights(uint32_t count)
    {
        // TODO? normalize power

        auto& lights = m_pointLights.lights;

        size_t keep = lights.size();
        if (count < keep)
            keep = count;

        lights.resize(count);

        if (count == 3)
        {
            lights[0] = { {0.f ,0.f,100.f}, {20,20,20} };
            lights[1] = { {0.f ,-5.f,-10.f}, {255,0,0} };
            lights[2] = { {-0.71f ,0.f,3.265f}, {255,187,115} };
            return;
        }

        glm::vec3 scale = glm::vec3(5.f, 5.f, 5.f) * 0.75f;

        constexpr float POWER_MIN = 20.0f;
        constexpr float POWER_MAX = 100.0f;

        std::random_device rd;
        std::seed_seq seed = { rd() };
        std::mt19937 mt(seed);
        std::uniform_real_distribution<float> dist(0.0f, 2.0f);

        for (size_t i = keep; i < count; i++)
        {
            glm::vec3 position = { 0,0,0 };
            position += glm::vec3(dist(mt), dist(mt), dist(mt)) * scale - (scale * 0.5f);
            position.y = 1.f;


            glm::vec3 color = glm::vec3(1.f, 1.f, 1.f);
            glm::vec3 power = color * (dist(mt) * (POWER_MAX - POWER_MIN) + POWER_MIN);
            lights[i] = { position, power };
            LOG_DEBUG(da::ELogChannel::Graphics, "Light created: %f %f %f, Power: %f, %f, %f", position.x, position.y, position.z, power.x, power.y, power.z);
        }
    }
#ifdef DA_DEBUG
    void CBgfxClusteredRenderer::renderLightDebug()
    {
        if (ImGui::Begin("Light Debug", &m_lightDebugVis)) {

            ImGui::Text("Ambient Light: ");
            ImGui::SameLine();
            ImGui::InputFloat3("##ambientLightControl", glm::value_ptr(m_ambientLight.irradiance));

            ImGui::Text("Sun light Direction: ");
            ImGui::SameLine();
            ImGui::SliderFloat3("##sunLightDirControl", glm::value_ptr(m_sunLight.direction), -1.f, 1.f);

            ImGui::Text("Sunlight Radiance: ");
            ImGui::SameLine();
            ImGui::InputFloat3("##sunLightRadControl", glm::value_ptr(m_sunLight.radiance));
        }

        ImGui::End();
    }
#endif
}
