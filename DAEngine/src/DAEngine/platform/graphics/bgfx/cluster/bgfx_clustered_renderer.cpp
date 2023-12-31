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
        //generateLights(40);
        m_pointLights.update();

        m_ambientLight.irradiance = { 0.03f, 0.03f, 0.03f };
        m_sunLight.direction = { 1.f, -0.667, .204 };
        m_sunLight.radiance = { 1.f,1.f,1.f };

        m_shadow.initialize();
        m_shadow.getCamera().setPosition({ 5.f, 0, 10.f });
        m_shadow.getCamera().lookAt({ 0.f, 0.f, 0.f });

#ifdef DA_DEBUG
        da::debug::CDebugMenuBar::register_debug(HASHSTR("Renderer"), HASHSTR("ClusteredLightView"), &m_clusterDebugVis, [&] {});
        da::debug::CDebugMenuBar::register_debug(HASHSTR("Renderer"), HASHSTR("Light Debug"), &m_lightDebugVis, [&] { renderLightDebug(); });
#endif
    }

    void CBgfxClusteredRenderer::onRender(float dt)
    {
        enum : ::bgfx::ViewId
        {
            vShadow = 0,
            vClusterBuilding,
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

		if (!::bgfx::isValid(m_shadow.getFrameBuffer()))
			m_shadow.createFrameBuffers();

		::bgfx::setViewName(vShadow, "Shadow pass");
		::bgfx::setViewRect(vShadow, 0, 0, m_shadow.getShadowMapSize(), m_shadow.getShadowMapSize());
		::bgfx::setViewFrameBuffer(vShadow, m_shadow.getFrameBuffer());
		::bgfx::setViewClear(vShadow
			, BGFX_CLEAR_COLOR | BGFX_CLEAR_DEPTH
			, 0x303030ff, 1.0f, 0
		);

        m_clusters.setUniforms(m_width, m_height);

        // cluster building needs u_invProj to transform screen coordinates to eye space
        setViewProjection(vClusterBuilding);
        // light culling needs u_view to transform lights to eye space
        setViewProjection(vLightCulling);
        setViewProjection(vLighting);

        glm::mat4 lightProj, lightView;
        bx::mtxProj(glm::value_ptr(lightProj), 75.f, (float)m_width / (float)m_height, 1.f, 10.f, ::bgfx::getCaps()->homogeneousDepth);
        bx::mtxLookAt(glm::value_ptr(lightView), { m_shadow.getCamera().position() .x, m_shadow.getCamera().position().y, m_shadow.getCamera().position() .z}, { 0.f, 0.f, 0.f }, { 0.f,0.f, 1.f });
        ::bgfx::setViewTransform(vShadow, glm::value_ptr(lightView), glm::value_ptr(lightProj));

        glm::mat4 lightMtx = lightProj * lightView;

		float mtxShadow[16];
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
		bx::mtxMul(mtxShadow, glm::value_ptr(lightView), mtxTmp);

		float mtxFloor[16];
		bx::mtxSRT(mtxFloor
			, -1.0f, -1.0f, -1.0f
			, 0.0f, 0.0f, 0.0f
			, 0.0f, 0.0f, 0.0f
		);

		// Floor.
		bx::mtxMul(glm::value_ptr(lightMtx), mtxFloor, mtxShadow);

		// Light matrix used in combine pass and inverse used in light pass
        //bx::mtxMul(glm::value_ptr(lightMtx), glm::value_ptr(lightView), glm::value_ptr(lightProj));

        da::core::CScene* scene = da::core::CSceneManager::getScene();

        const da::core::FComponentContainer& container = scene->getComponents<da::core::CSmeshComponent>();

        // Shadow map pass
        for (size_t i = 0; i < container.getCount(); i++) {
            da::core::CSmeshComponent* mesh = container.getComponentAtIndex<da::core::CSmeshComponent>(i);

            glm::mat4 model = mesh->getParent().getTransform().matrix();
            ::bgfx::setTransform(glm::value_ptr(model));
            ::bgfx::setVertexBuffer(0, *((::bgfx::VertexBufferHandle*)mesh->getStaticMesh()->getNativeVB()));
            ::bgfx::setIndexBuffer(*((::bgfx::IndexBufferHandle*)mesh->getStaticMesh()->getNativeIB()));
            ::bgfx::setState((m_shadow.useShadowSampler() ? 0 : BGFX_STATE_WRITE_RGB | BGFX_STATE_WRITE_A)
                | BGFX_STATE_WRITE_Z
                | BGFX_STATE_DEPTH_TEST_LESS
                | BGFX_STATE_CULL_CCW
                | BGFX_STATE_MSAA);
            ::bgfx::submit(vShadow, { m_shadow.getMaterial()->getHandle() }, 0, ~BGFX_DISCARD_BINDINGS);
        }


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

        // Render pass
        for (size_t i = 0; i < container.getCount(); i++) {
            da::core::CSmeshComponent* mesh = container.getComponentAtIndex<da::core::CSmeshComponent>(i);
            m_pbr.bindLightPos(m_shadow.getCamera().position(), lightMtx);
            glm::mat4 model = mesh->getParent().getTransform().matrix();
            ::bgfx::setTransform(glm::value_ptr(model));
            setNormalMatrix(model);
            ::bgfx::setVertexBuffer(0, *((::bgfx::VertexBufferHandle*)mesh->getStaticMesh()->getNativeVB()));
            ::bgfx::setIndexBuffer(*((::bgfx::IndexBufferHandle*)mesh->getStaticMesh()->getNativeIB()));
            da::platform::CBgfxPbrMaterial* material = (da::platform::CBgfxPbrMaterial*)mesh->getMaterial();
            uint64_t materialState = m_pbr.bindMaterial(*material->getMaterial());
            ::bgfx::setTexture(CBgfxSamplers::SHADOW_MAP, m_pbr.getShadowMapUniform(), m_shadow.getShadowMap());
            //BGFX_STATE_PT_LINES
			::bgfx::setState(state | materialState);
            // preserve buffer bindings between submit calls
            ::bgfx::submit(vLighting, program, 0, ~BGFX_DISCARD_BINDINGS);
        }

        ::bgfx::discard(BGFX_DISCARD_ALL);
    }

    void CBgfxClusteredRenderer::onShutdown()
    {

        m_clusters.shutdown();
        m_shadow.shutdown();
        m_pointLights.shutdown();

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

        int index = 0;
        for (int y = -countHalf; y <= countHalf; y+=2) {
            for (int x = -countHalf; x <= countHalf; x+=2) {
                std::string str1 = std::string(std::to_string(x) + ":" + std::to_string(y).c_str());
                CHashString hsh1(str1.c_str());
                glm::vec3 col = { ((hsh1.hash() & 0x00ff0000u) >> 16), ((hsh1.hash() & 0x0000ff00u) >> 8) * .75f, (hsh1.hash() & 0x000000ffu) * .5f};
                lights[index++] = { {x,y,-.99f}, glm::vec3(100.f) * col };
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

            ::bgfx::TextureHandle handle = ::bgfx::getTexture(m_shadow.getFrameBuffer());

            if (handle.idx == UINT16_MAX) {
                return;
            }

            //ImGui::Image(handle, ImVec2(m_shadow.getShadowMapSize(), m_shadow.getShadowMapSize()));
        }

        ImGui::End();
    }
#endif
}
