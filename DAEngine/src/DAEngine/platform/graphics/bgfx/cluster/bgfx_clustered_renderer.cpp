
#include "bgfx_clustered_renderer.h"

#include <bx/string.h>
#include <glm/gtc/type_ptr.hpp>
#include <glm/ext/matrix_relational.hpp>
#include <platform/graphics/bgfx/bgfx_graphics_material.h>
#include <core/ecs/scene_manager.h>
#include "platform/app/ecs/smesh_component.h"
#include "platform/graphics/bgfx/bgfx_static_mesh.h"
#include "platform/graphics/bgfx/bgfx_skeletal_mesh.h"
#include "platform/graphics/bgfx/bgfx_graphics_pbr_material.h"
#include "graphics/skeletal_animator.h"
#include <random>
#include "core/logger.h"
#include "bgfx_samplers.h"
#include <bx/bx.h>
#include <bx/math.h>
#include "stl/maths/flags.h"

#ifdef DA_REVIEW
#include "platform/imgui/bgfx/bgfx_imgui.h"
#include "debug/debug_menu_bar.h"
#include "debug/debug_stats_window.h"
#endif
#include <stl/time.h>
#include <platform/app/ecs/skeletal_mesh_component.h>
#include <debug/graphics_debug_render.h>


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

        m_pClusterBuildingComputeProgram = da::factory::CMaterialFactory::create("shaders/cluster/cs_clustered_clusterbuilding.sc");

        m_pResetCounterComputeProgram = da::factory::CMaterialFactory::create("shaders/cluster/cs_clustered_reset_counter.sc");

        m_pLightCullingComputeProgram = da::factory::CMaterialFactory::create("shaders/cluster/cs_clustered_lightculling.sc");

        m_pLightingProgram = da::factory::CMaterialFactory::create("shaders/cluster/vs_clustered.sc", "shaders/cluster/fs_clustered.sc");

        m_pLightingInstanceProgram = da::factory::CMaterialFactory::create("shaders/cluster/vs_clustered_instance.sc", "shaders/cluster/fs_clustered.sc");

        m_pLightingSkeletalProgram = da::factory::CMaterialFactory::create("shaders/cluster/vs_sk_clustered.sc", "shaders/cluster/fs_clustered.sc");

        m_pDebugVisProgram = da::factory::CMaterialFactory::create("shaders/cluster/vs_clustered.sc", "shaders/cluster/fs_clustered_debug_vis.sc");

        m_pointLights.init();
        m_pointLights.update(da::graphics::CLightManager::getLights());
        da::graphics::CLightManager::registerOnLightEvent(BIND_EVENT_FN_2(CBgfxClusteredRenderer, onLightEvent));


        m_ssao.initialize();
#ifdef DA_REVIEW
        m_debugRenderer.initialize();
        da::debug::CDebugRender::setInstance(&m_debugRenderer);
#endif

		m_ambientLight.irradiance = { .25f, .25f, .25f };

        m_bloom.initialize(m_width, m_height);
        m_volumetricLight.initialize();
        m_csm.initialize();
		m_csm.setRenderFunc([this](uint8_t view, da::graphics::CMaterial* mat, da::graphics::CMaterial* instanceMat,da::graphics::CMaterial* skMat, da::platform::RenderState renderState) {
			renderFunc(view, mat, instanceMat, skMat, renderState, ERenderFlags::ShadowPass);
		});

#ifdef DA_REVIEW
        da::debug::CDebugMenuBar::register_debug(HASHSTR("Renderer"), HASHSTR("ClusteredLightView"), &m_clusterDebugVis, [&] {});
        da::debug::CDebugMenuBar::register_debug(HASHSTR("Renderer"), HASHSTR("Light Debug"), &m_lightDebugVis, [&] { renderLightDebug(); });
#endif
    }

    void CBgfxClusteredRenderer::onRender(float dt)
    {
        PROFILE()
        enum : ::bgfx::ViewId
        {
            vDepth = 0,
#ifdef DA_REVIEW
            vDebug,
#endif
            vSSAO,
            vSSAOBlur,
            vShadow,
            vClusterBuilding = vShadow + 13,
            vLightCulling,
            vLighting,
            vVolumetricLight,
            vBloom,
            vBloomBlur,
            COUNT
        };

#ifdef DA_REVIEW
        {
            if (da::core::CTime::getFrameCount() % 100 == 0) {
                da::debug::CDebugStatsWindow::s_viewTimes = {};
                for (size_t i = 0; i < ::bgfx::getStats()->numViews; i++) {
                    const ::bgfx::ViewStats& stats = ::bgfx::getStats()->viewStats[i];

                    double time = (stats.cpuTimeEnd - stats.cpuTimeBegin) / 1000.0;

                    switch (stats.view) {
                    case vDepth: da::debug::CDebugStatsWindow::s_viewTimes["vDepth"] = time; break;
                    case vDebug: da::debug::CDebugStatsWindow::s_viewTimes["vDebug"] = time; break;
                    case vSSAO: da::debug::CDebugStatsWindow::s_viewTimes["vSSAO"] = time; break;
                    case vSSAOBlur: da::debug::CDebugStatsWindow::s_viewTimes["vSSAOBlur"] = time; break;
                    case vShadow: da::debug::CDebugStatsWindow::s_viewTimes["vShadow"] += time; break;
                    case vLightCulling: da::debug::CDebugStatsWindow::s_viewTimes["vLightCulling"] = time; break;
                    case vLighting: da::debug::CDebugStatsWindow::s_viewTimes["vLighting"] = time; break;
                    case vVolumetricLight: da::debug::CDebugStatsWindow::s_viewTimes["vVolumetricLight"] = time; break;
                    case vBloom: da::debug::CDebugStatsWindow::s_viewTimes["vBloom"] = time; break;
                    case vBloomBlur: da::debug::CDebugStatsWindow::s_viewTimes["vBloomBlur"] += time; break;
                    }

                    if (stats.view > vBloomBlur) {
                        da::debug::CDebugStatsWindow::s_viewTimes["vBloomBlur"] += time;
                    }

                    if (stats.view > vShadow && stats.view < vLightCulling) {
                        da::debug::CDebugStatsWindow::s_viewTimes["vShadow"] += time;
                    }
                }
            }
        }
        
#endif

		da::core::CScene* scene = da::core::CSceneManager::getScene();

		::bgfx::setViewName(vDepth, "Depth pass");
		::bgfx::setViewClear(vDepth, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH, 0xFFFFFF00, 1.0f, 0);
		::bgfx::setViewRect(vDepth, 0, 0, m_width, m_height);
		::bgfx::setViewFrameBuffer(vDepth, m_depthBuffer);
		::bgfx::touch(vDepth);

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

		::bgfx::setViewName(vBloom, "Bloom first pass");
		::bgfx::setViewClear(vBloom, BGFX_CLEAR_COLOR, 0x00000000, 1.0f, 0);
		::bgfx::touch(vBloom);

#if defined(DA_DEBUG) || defined(DA_RELEASE)
		::bgfx::setViewName(vDebug, "Debug Renderer");
		::bgfx::setViewClear(vDebug, BGFX_CLEAR_COLOR, 0x00000000, 1.0f, 0);
        ::bgfx::setViewRect(vDebug, 0, 0, m_width, m_height);
        ::bgfx::setViewFrameBuffer(vDebug, m_debugRenderer.getFrameBuffer());
		::bgfx::touch(vDebug);
		setViewProjection(vDebug);
#endif

        setViewProjection(vDepth);

		const da::core::FComponentContainer* staticMeshcontainer = scene ? &scene->getComponents<da::core::CSmeshComponent>() : nullptr;
		const da::core::FComponentContainer* skeletalMeshcontainer = scene ? &scene->getComponents<da::core::CSkeletalMeshComponent>() : nullptr;

        // Depth pass
        {
            PROFILE_NAME("depthPass")
            renderFunc(vDepth, m_pDepthprogram, m_pDepthprogramInst, m_pDepthprogramSk, { BGFX_STATE_WRITE_Z | BGFX_STATE_DEPTH_TEST_LESS | BGFX_STATE_CULL_CCW }, ERenderFlags::None);
        }

        {
            PROFILE_NAME("ssao")
            m_ssao.renderSSAO(m_width, m_height, vSSAO, m_depthBuffer);
            m_ssao.renderBlur(m_width, m_height, vSSAOBlur);
        }

        {
            PROFILE_NAME("csm")
            m_csm.update(vShadow, m_sun.m_sunDir, m_width, m_height);
        }
        m_clusters.setUniforms(m_width, m_height);

        // cluster building needs u_invProj to transform screen coordinates to eye space
        setViewProjection(vClusterBuilding, false);
        // light culling needs u_view to transform lights to eye space
        setViewProjection(vLightCulling, false);
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
            PROFILE_NAME("buildClusters")
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

        {
            PROFILE_NAME("bingLights")
            m_lights.bindLights(m_sun.m_sunDir, { 0.f,0.f,0.f }, m_ambientLight, m_pointLights);
        }
        {
            PROFILE_NAME("bindClusters")
            m_clusters.bindBuffers(false);
        }

        ::bgfx::dispatch(vLightCulling,
            { m_pLightCullingComputeProgram->getHandle() },
            CBgfxClusterShader::CLUSTERS_X / CBgfxClusterShader::CLUSTERS_X_THREADS,
            CBgfxClusterShader::CLUSTERS_Y / CBgfxClusterShader::CLUSTERS_Y_THREADS,
            CBgfxClusterShader::CLUSTERS_Z / CBgfxClusterShader::CLUSTERS_Z_THREADS);
        // lighting
#ifdef DA_REVIEW
        da::graphics::CMaterial* program = m_clusterDebugVis ? m_pDebugVisProgram : m_pLightingProgram;
#else
        da::graphics::CMaterial* program =  m_pLightingProgram;
#endif

		uint64_t state = BGFX_STATE_DEFAULT & ~BGFX_STATE_CULL_MASK;

		m_pbr.bindAlbedoLUT();
        {
            PROFILE_NAME("bingLights")
            m_lights.bindLights(m_sun.m_sunDir, m_sky.getSunLuminance(), m_ambientLight, m_pointLights);
        }
        {
            PROFILE_NAME("bindClusters2")
            m_clusters.bindBuffers(true /*lightingPass*/); // read access, only light grid and indices
        }

        {
            PROFILE_NAME("sky")
            m_sky.render(vLighting, state);
        }
		
        {
            PROFILE_NAME("lighting")
            renderFunc(vLighting, program, m_pLightingInstanceProgram, m_pLightingSkeletalProgram, { state }, ERenderFlags::PBR);
        }

        ::bgfx::TextureHandle tex = ::bgfx::getTexture(m_frameBuffer, 0);
        {
            PROFILE_NAME("bloom")
            m_bloom.render(vBloom, tex, m_width, m_height);
            m_bloom.renderBlur(vBloomBlur, m_width, m_height);
        }

        {
            PROFILE_NAME("volumetriclight")
            m_volumetricLight.render(vVolumetricLight, m_width, m_height, ::bgfx::getTexture(m_frameBuffer, 1), m_sun.getScreenSpacePos(m_viewMat, m_projMat));
        }

#if defined(DA_DEBUG) || defined(DA_RELEASE)
        {
            PROFILE_NAME("debug")
            m_debugRenderer.renderXRay(vDebug);
            m_debugRenderer.render(vLighting);
        }
#endif

        {
            PROFILE_NAME("discard")
            ::bgfx::discard(BGFX_DISCARD_ALL);
        }
    }


    void CBgfxClusteredRenderer::onShutdown()
    {

        m_clusters.shutdown();
        m_pointLights.shutdown();
        m_ssao.shutdown();
        m_bloom.shutdown();
        m_volumetricLight.shutdown();
        m_csm.shutdown();
#if defined(DA_DEBUG) || defined(DA_RELEASE)
        m_debugRenderer.shutdown();
#endif

        da::graphics::CLightManager::unregisterOnLightEvent(BIND_EVENT_FN_2(CBgfxClusteredRenderer, onLightEvent));

        da::factory::CMaterialFactory::remove(m_pClusterBuildingComputeProgram);
		da::factory::CMaterialFactory::remove(m_pResetCounterComputeProgram);
		da::factory::CMaterialFactory::remove(m_pLightCullingComputeProgram);
		da::factory::CMaterialFactory::remove(m_pLightingProgram);
		da::factory::CMaterialFactory::remove(m_pDebugVisProgram);
        da::factory::CMaterialFactory::remove(m_pLightingSkeletalProgram);
        da::factory::CMaterialFactory::remove(m_pLightingInstanceProgram);

        m_pClusterBuildingComputeProgram = m_pResetCounterComputeProgram = m_pLightCullingComputeProgram = m_pLightingProgram =
            m_pDebugVisProgram = nullptr;

#ifdef DA_REVIEW
        da::debug::CDebugMenuBar::unregister_debug(HASHSTR("Renderer"), HASHSTR("ClusteredLightView"));
        da::debug::CDebugMenuBar::unregister_debug(HASHSTR("Renderer"), HASHSTR("Light Debug"));
#endif
    }

	void CBgfxClusteredRenderer::onReset(size_t width, size_t height)
	{
		m_ssao.reset(width, height);
        m_bloom.onReset(width, height);
        m_volumetricLight.onReset(width, height);
#ifdef DA_REVIEW
        m_debugRenderer.onReset(width, height);
#endif
	}


	void CBgfxClusteredRenderer::onLightEvent(const da::graphics::FLightData& data, bool added)
	{
		m_pointLights.update(da::graphics::CLightManager::getLights());
	}

	void CBgfxClusteredRenderer::renderFunc(uint8_t view, da::graphics::CMaterial* mat, da::graphics::CMaterial* instanceMat, da::graphics::CMaterial* skMat, da::platform::RenderState renderState, ERenderFlags flags)
	{
        if (da::core::CScene* scene = da::core::CSceneManager::getScene()) {

            if (da::maths::CFlag::hasFlag(flags, ERenderFlags::PBR))
            {
                PROFILE_NAME("bind")
                    m_ssao.bindSSAO();
                m_csm.bindTextures();
            }

            const da::core::FComponentContainer& staticMeshcontainer = scene->getComponents<da::core::CSmeshComponent>();
            const da::core::FComponentContainer& skeletalMeshcontainer = scene->getComponents<da::core::CSkeletalMeshComponent>();
            {
                    PROFILE_NAME("staticMeshPass")
                    for (size_t x = 0; x < staticMeshcontainer.getCount(); x++) {
                        da::core::CSmeshComponent* meshComponent = staticMeshcontainer.getComponentAtIndex<da::core::CSmeshComponent>(x);
                        glm::mat4 model = meshComponent->getParent().getTransform().matrix();
                        da::graphics::CStaticMesh* mesh = meshComponent->getStaticMesh();
                        ASSERT(mesh);
                        const std::vector<da::graphics::FMesh>& meshes = mesh->getMeshes();
                        PROFILE_TAG("meshes", meshes.size())
                        for (size_t z = 0; z < meshes.size(); z++) {
                            if (da::maths::CFlag::hasFlag(flags, ERenderFlags::ShadowPass) && !mesh->getCastShadows()) continue;
                            if (mesh->getHidden()) continue;

                            ::bgfx::setTransform(glm::value_ptr(model));

                            uint64_t materialState = 0;

                            if (da::maths::CFlag::hasFlag(flags, ERenderFlags::PBR))
                            {
                                PROFILE("pbr")
                                m_csm.submitUniforms();
                                setNormalMatrix(model);
                                materialState = m_pbr.bindMaterial(mesh->getMaterial(meshes[z].MaterialIndex));
                            }


                            const std::vector<da::core::FInstance>& instances = meshComponent->getInstances();

                            if (instances.empty() || !instanceMat) {
                                ::bgfx::setVertexBuffer(0, *((::bgfx::VertexBufferHandle*)mesh->getNativeVBIndex(z)));
                                ::bgfx::setIndexBuffer(*((::bgfx::IndexBufferHandle*)mesh->getNativeIBIndex(z)));
                                ::bgfx::setState(renderState.m_state | materialState);
                                ::bgfx::submit(view, { mat->getHandle() }, 0, ~BGFX_DISCARD_BINDINGS);
                            }
                            else {
                                const uint16_t instanceStride = sizeof(glm::mat4);
                                const uint32_t total = instances.size();
                                ASSERT(total);
                                uint32_t drawn = ::bgfx::getAvailInstanceDataBuffer(total, instanceStride);

                                if (!drawn) return;

                                ::bgfx::InstanceDataBuffer idb;
                                ::bgfx::allocInstanceDataBuffer(&idb, drawn, instanceStride);

                                uint8_t* data = idb.data;

                                for (int ii = 0; ii < drawn; ii++) {
                                    memcpy(data, &instances[ii].Transform, sizeof(glm::mat4));
                                    data += instanceStride;
                                }
                                ::bgfx::setInstanceDataBuffer(&idb);

                                ::bgfx::setVertexBuffer(0, *((::bgfx::VertexBufferHandle*)mesh->getNativeVBIndex(z)));
                                ::bgfx::setIndexBuffer(*((::bgfx::IndexBufferHandle*)mesh->getNativeIBIndex(z)));
                                ::bgfx::setState(renderState.m_state | materialState);
                                ::bgfx::submit(view, { instanceMat->getHandle() }, 0, ~BGFX_DISCARD_BINDINGS);
                            }

                        }
                    }
            }

            {
                PROFILE_NAME("skeletalMeshPass")
                for (size_t x = 0; x < skeletalMeshcontainer.getCount(); x++) {

                    da::core::CSkeletalMeshComponent* meshComponent = skeletalMeshcontainer.getComponentAtIndex<da::core::CSkeletalMeshComponent>(x);
                    const glm::mat4& model = meshComponent->getTransform();
                    da::graphics::CSkeletalMesh* mesh = meshComponent->getSkeletalMesh();
                    ASSERT(mesh);
                    const std::vector<da::graphics::FSkeletalMesh>& meshes = mesh->getMeshes();
                    PROFILE_TAG("meshes", meshes.size())
                    for (size_t z = 0; z < meshes.size(); z++) {
                        ::bgfx::setUniform(m_bonesUniform, meshComponent->getSkeletalAnimator()->getFinalBoneMatrices().data(), 128);
                        ::bgfx::setTransform(glm::value_ptr(model));
                        ::bgfx::setVertexBuffer(0, *((::bgfx::VertexBufferHandle*)mesh->getNativeVBIndex(z)));
                        ::bgfx::setIndexBuffer(*((::bgfx::IndexBufferHandle*)mesh->getNativeIBIndex(z)));

                        uint64_t materialState = 0;

                        if (da::maths::CFlag::hasFlag(flags, ERenderFlags::PBR))
                        {
                            PROFILE("pbr")
                            m_csm.submitUniforms();
                            setNormalMatrix(model);
                            materialState = m_pbr.bindMaterial(mesh->getMaterial(meshes[z].MaterialIndex));
                        }

                        ::bgfx::setState(renderState.m_state | materialState);
                        ::bgfx::submit(view, { skMat->getHandle() }, 0, ~BGFX_DISCARD_BINDINGS);
                    }
                }
            }
        }
	}


#ifdef DA_REVIEW
    void CBgfxClusteredRenderer::renderLightDebug()
    {
        if (ImGui::Begin("Light Debug", &m_lightDebugVis)) {

            ImGui::Text("ToneMapping: ");
            ImGui::SameLine();
            const char* const* toneMaps = getETonemappingModeNames();
            ImGui::Combo("##toneMapping", (int*)&m_tonemappingMode, toneMaps, (int)ETonemappingMode::MAX);

            ImGui::Text("Ambient Light: ");
            ImGui::SameLine();
            ImGui::InputFloat3("##ambientLightControl", glm::value_ptr(m_ambientLight.irradiance));

        }
        ImGui::End();
    }


#endif
}
