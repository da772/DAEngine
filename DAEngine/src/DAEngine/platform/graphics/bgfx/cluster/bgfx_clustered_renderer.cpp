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
#include "bgfx_samplers.h"
#include <bx/bx.h>
#include <bx/math.h>

#ifdef DA_DEBUG
#include "DAEngine/platform/imgui/bgfx/bgfx_imgui.h"
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
        //generateLights(100);
        m_pointLights.update();

        m_shadow.initialize();
        m_shadow.getCamera().setPosition({ 0.f, 15, 15.f });
        m_shadow.getCamera().setRotation({ -50.f, 0.f, 180.f });


        m_ssao.initialize();

		m_ambientLight.irradiance = { .25f, .25f, .25f };
		m_sunLight.direction = m_shadow.getLightDir();
		m_sunLight.radiance = { 1.f, 1.f,1.f };

#ifdef DA_DEBUG
        da::debug::CDebugMenuBar::register_debug(HASHSTR("Renderer"), HASHSTR("ClusteredLightView"), &m_clusterDebugVis, [&] {});
        da::debug::CDebugMenuBar::register_debug(HASHSTR("Renderer"), HASHSTR("Light Debug"), &m_lightDebugVis, [&] { renderLightDebug(); });
#endif
    }

    void CBgfxClusteredRenderer::onRender(float dt)
    {
        enum : ::bgfx::ViewId
        {
            vDepth = 0,
            vSSAO,
            vSSAOBlur,
            vShadow,
            vClusterBuilding = vShadow + SHADOW_MAP_SIZE,
            vLightCulling,
            vLighting,
        };

		da::core::CScene* scene = da::core::CSceneManager::getScene();
		const da::core::FComponentContainer& container = scene->getComponents<da::core::CSmeshComponent>();

        m_shadow.getLightDir() = m_sun.m_sunDir;
        m_sunLight.direction = m_shadow.getLightDir();

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

        setViewProjection(vDepth);
        // Depth pass
		for (size_t x = 0; x < container.getCount(); x++) {
			da::core::CSmeshComponent* meshComponent = container.getComponentAtIndex<da::core::CSmeshComponent>(x);
			glm::mat4 model = meshComponent->getParent().getTransform().matrix();

            for (size_t z = 0; z < meshComponent->getStaticMesh()->getMeshes().size(); z++) {
                const da::graphics::CStaticMesh* mesh = meshComponent->getStaticMesh();
                ASSERT(mesh);

                ::bgfx::setTransform(glm::value_ptr(model));
                ::bgfx::setVertexBuffer(0, *((::bgfx::VertexBufferHandle*)mesh->getNativeVBIndex(z)));
                ::bgfx::setIndexBuffer(*((::bgfx::IndexBufferHandle*)mesh->getNativeIBIndex(z)));
                ::bgfx::setState( BGFX_STATE_WRITE_Z
					| BGFX_STATE_DEPTH_TEST_LESS
					| BGFX_STATE_CULL_CCW);
				::bgfx::submit(vDepth, { m_pDepthprogram->getHandle() }, 0, ~BGFX_DISCARD_BINDINGS);
			}
		}

        m_ssao.renderSSAO(m_width, m_height, vSSAO, m_depthBuffer);
        m_ssao.renderBlur(m_width, m_height, vSSAOBlur);

        glm::mat4 lightMtx[SHADOW_MAP_SIZE];

        for (size_t i = 0; i < m_shadow.getShadowMapsCount(); i++) {

            if (!::bgfx::isValid(m_shadow.getShadowMaps().ShadowMaps[i].FrameBuffer))
                m_shadow.createFrameBuffers();

            std::string name = std::string("Shadow Pass: ") + std::to_string(i);
            ::bgfx::setViewName(vShadow + i, name.c_str());
            ::bgfx::setViewRect(vShadow + i, 0, 0, m_shadow.getShadowMapSize(), m_shadow.getShadowMapSize());
            ::bgfx::setViewFrameBuffer(vShadow + i, m_shadow.getShadowMaps().ShadowMaps[i].FrameBuffer);
            ::bgfx::setViewClear(vShadow + i
                , BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
                , 0x303030ff, 1.0f, 0
            );

            glm::mat4 lightProj, lightView;
            float nearPlane = 1000.f* (50.f * i);
            float farPlane = (1000.f * (50.f * i) + 1000.f);
            if (i == 0) {
                nearPlane = 1.f;
                farPlane  = 25.f;
            }

            if (i == 1) {
                nearPlane = 25.f;
                farPlane = 75.f;
            }

			if (i == 2) {
				nearPlane = 75.f;
				farPlane = 125.f;
			}

			if (i == 3) {
				nearPlane = 150.f;
				farPlane = 300.f;
			}

            //bx::mtxProj(glm::value_ptr(lightProj), 75.f, (float)m_width / (float)m_height, nearPlane, farPlane, ::bgfx::getCaps()->homogeneousDepth);
            bx::mtxOrtho(glm::value_ptr(lightProj), -farPlane, farPlane, -farPlane, farPlane, nearPlane, farPlane, 0, ::bgfx::getCaps()->homogeneousDepth);
            lightView = m_shadow.getCamera().matrix();
            std::pair<glm::mat4, glm::mat4> p = m_shadow.getLightSpaceProjMatrix(nearPlane, farPlane, i, lightView);;
            lightProj = p.first;
            lightView = p.second;
            lightMtx[i] = lightProj * lightView;


            const ::bgfx::Caps* caps = ::bgfx::getCaps();
            const float sy = caps->originBottomLeft ? 0.5f : -0.5f;
            const float sz = caps->homogeneousDepth ? 0.5f : 1.0f;
            const float tz = caps->homogeneousDepth ? 0.5f : 0.0f;
            const float mtxCrop[16] =
            {
                0.5f, 0.0f, 0.0f, 0.0f,
                0.0f,   sy, 0.0f, 0.0f,
                0.0f, 0.0f, sz,   0.0f,
                0.5f, 0.5f, tz,   1.0f,
            };

            float mtxTmp[16];
            bx::mtxMul(mtxTmp, glm::value_ptr(lightProj), mtxCrop);
            bx::mtxMul(glm::value_ptr(lightMtx[i]), glm::value_ptr(lightView), mtxTmp);

            ::bgfx::setViewTransform(vShadow + i, glm::value_ptr(lightView), glm::value_ptr(lightProj));

			// Shadow map pass
			for (size_t x = 0; x < container.getCount(); x++) {
				da::core::CSmeshComponent* meshComponent = container.getComponentAtIndex<da::core::CSmeshComponent>(x);
                glm::mat4 model = meshComponent->getParent().getTransform().matrix();

                for (size_t z = 0; z < meshComponent->getStaticMesh()->getMeshes().size(); z++) {
                    const da::graphics::CStaticMesh* mesh = meshComponent->getStaticMesh();
                    ASSERT(mesh);

                    if (!mesh->getCastShadows()) continue;
                    if (mesh->getHidden()) continue;

                    ::bgfx::setTransform(glm::value_ptr(model));
                    ::bgfx::setVertexBuffer(0, *((::bgfx::VertexBufferHandle*)mesh->getNativeVBIndex(z)));
                    ::bgfx::setIndexBuffer(*((::bgfx::IndexBufferHandle*)mesh->getNativeIBIndex(z)));
                    ::bgfx::setState((m_shadow.useShadowSampler() ? 0 : BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A)
                        | BGFX_STATE_WRITE_Z
                        | BGFX_STATE_DEPTH_TEST_LESS
                        | BGFX_STATE_CULL_CCW
                        | BGFX_STATE_MSAA);
                    ::bgfx::submit(vShadow + i, { m_shadow.getMaterial()->getHandle() }, 0, ~BGFX_DISCARD_BINDINGS);
                }
			}
        }

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
        glm::vec3 rotRadians = glm::vec3(glm::radians(m_shadow.getCamera().rotation().x), glm::radians(m_shadow.getCamera().rotation().y), glm::radians(m_shadow.getCamera().rotation().z));
        m_lights.bindLights({ m_shadow.getLightDir(),m_sunLight.radiance }, m_ambientLight, m_pointLights);
		m_clusters.bindBuffers(true /*lightingPass*/); // read access, only light grid and indices

        m_sun.Update(m_skyTime);
		m_sky.setUniforms(m_sun, m_skyTime);
		m_sky.render(vLighting);

        // Render pass
        for (size_t i = 0; i < container.getCount(); i++) {
            da::core::CSmeshComponent* meshComponent = container.getComponentAtIndex<da::core::CSmeshComponent>(i);
            const glm::mat4& model = meshComponent->getParent().getTransform().matrix();
           
            for (size_t z = 0; z < meshComponent->getStaticMesh()->getMeshes().size(); z++) {
                da::graphics::CStaticMesh* mesh = meshComponent->getStaticMesh();

                if (mesh->getHidden()) continue;

                ::bgfx::setTransform(glm::value_ptr(model));
                setNormalMatrix(model);
                ::bgfx::setVertexBuffer(0, *((::bgfx::VertexBufferHandle*)mesh->getNativeVBIndex(z)));
                ::bgfx::setIndexBuffer(*((::bgfx::IndexBufferHandle*)mesh->getNativeIBIndex(z)));
                m_pbr.bindLightPos(m_shadow.getCamera().position(), lightMtx);
                uint64_t materialState = m_pbr.bindMaterial(mesh->getMaterial(mesh->getMeshes()[z].MaterialIndex));
                for (size_t s = 0; s < m_shadow.getShadowMapsCount(); s++) {
                    ::bgfx::setTexture(CBgfxSamplers::SAMPLER_SHADOW_MAP_NEAR + s, m_shadow.getShadowMaps().ShadowMaps[s].Uniform, m_shadow.getShadowMaps().ShadowMaps[s].Texture);
                }
                m_ssao.bindSSAO();

                //BGFX_STATE_PT_LINES
                ::bgfx::setState(state | materialState);
                // preserve buffer bindings between submit calls
                ::bgfx::submit(vLighting, program, 0, ~BGFX_DISCARD_BINDINGS);
            }
        }

        ::bgfx::discard(BGFX_DISCARD_ALL);
    }

    void CBgfxClusteredRenderer::onShutdown()
    {

        m_clusters.shutdown();
        m_shadow.shutdown();
        m_pointLights.shutdown();
        m_ssao.shutdown();

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

        auto& lights = m_pointLights.getLights();

        size_t keep = lights.size();
        if (count < keep)
            keep = count;

        lights.resize(count*count);

        glm::vec3 scale = glm::vec3(100.f, 100.f, 1.f) * 0.75f;

        constexpr float POWER_MIN = 20.0f;
        constexpr float POWER_MAX = 100.0f;

        int countHalf = count / 2;
        float distScale = 3.f;

        int index = 0;
        for (int y = -countHalf; y <= countHalf; y+=2) {
            for (int x = -countHalf; x <= countHalf; x+=2) {
                std::string str1 = std::string(std::to_string(x) + ":" + std::to_string(y).c_str());
                CHashString hsh1(str1.c_str());
                glm::vec3 col = { ((hsh1.hash() & 0x00ff0000u) >> 16), ((hsh1.hash() & 0x0000ff00u) >> 8) * .75f, (hsh1.hash() & 0x000000ffu) * .5f};
                lights[index++] = { {x* distScale ,y* distScale ,1.f}, glm::vec3(5.f) * col };
            }
        }

        lights.resize(index+1);

        return;

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

	void CBgfxClusteredRenderer::onReset(size_t width, size_t height)
	{
		m_ssao.reset(width, height);
	}


#ifdef DA_DEBUG
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

            ImGui::Text("Sun light Direction: ");
            ImGui::SameLine();
            ImGui::SliderFloat3("##sunLightDirControl", glm::value_ptr(m_shadow.getLightDir()), -1.f, 1.f);

            ImGui::Text("Sunlight Radiance: ");
            ImGui::SameLine();
            ImGui::InputFloat3("##sunLightRadControl", glm::value_ptr(m_sunLight.radiance));

			ImGui::Text("Sunlight Position: ");
			ImGui::SameLine();
            glm::vec3 p = m_shadow.getCamera().position();
            if (ImGui::InputFloat3("##sunLightPosControl", glm::value_ptr(p))) {
                m_shadow.getCamera().setPosition(p);
            }

			ImGui::Text("Sunlight Rotation: ");
			ImGui::SameLine();
			glm::vec3 r = m_shadow.getCamera().rotation();
			if (ImGui::InputFloat3("##sunLightRotControl", glm::value_ptr(r))) {
				m_shadow.getCamera().setRotation(r);
			}

            ImGui::Text("Sky Time: ");
            ImGui::SameLine();
            ImGui::DragFloat("###skyTime", &m_skyTime, .1f, 0.f, 24.f);

            ::bgfx::TextureHandle handle = ::bgfx::getTexture(m_ssao.getBuffer());

            if (handle.idx == UINT16_MAX) {
                return;
            }
            ImGui::Image((ImTextureID)handle.idx, { 480, 240 });
            //ImGui::Image(handle, ImVec2(480, 240));
            
        }
        ImGui::End();
    }
#endif
}