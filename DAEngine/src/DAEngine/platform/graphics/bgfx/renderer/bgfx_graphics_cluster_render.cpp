#include "dapch.h"

#include "bgfx_graphics_cluster_render.h"

#ifdef DA_GRAPHICS_BGFX

#include "daengine/platform/graphics/bgfx/bgfx_graphics_material.h"
#include "daengine/core/graphics/camera.h"
#include "daengine/core/ecs/scene.h"
#include "daengine/core/ecs/scene_manager.h"
#include "daengine/core/ecs/smesh_component.h"
#include "daengine/core/graphics/graphics_smesh_cube.h"


#include <bx/bx.h>
#include <bx/macros.h>
#include <bx/string.h>
#include <bx/math.h>
#include <glm/common.hpp>
#include <glm/vector_relational.hpp>
#include <glm/gtx/component_wise.hpp>
#include <glm/gtc/color_space.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/matrix_operation.hpp>
#include <glm/ext/matrix_relational.hpp>

namespace da::platform::bgfx
{

	void CBgfxGraphicsClusterRender::onInitialize()
	{
		m_clusters.initialize();

		m_clusterBuildingComputeProgram = new CBgfxGraphicsMaterial("shaders/cluster/cs_clustered_clusterbuilding.sc");
		m_clusterBuildingComputeProgram->initialize();

		m_resetCounterComputeProgram = new CBgfxGraphicsMaterial("shaders/cluster/cs_clustered_reset_counter.sc");
		m_resetCounterComputeProgram->initialize();

		m_lightCullingComputeProgram = new CBgfxGraphicsMaterial("shaders/cluster/cs_clustered_lightculling.sc");
		m_lightCullingComputeProgram->initialize();

		m_lightingProgram = new CBgfxGraphicsMaterial("shaders/cluster/vs_clustered.sc", "shaders/cluster/fs_clustered.sc");
		m_lightingProgram->initialize();

		m_debugVisProgram = new CBgfxGraphicsMaterial("shaders/cluster/fs_clustered_debug_vis.sc");
		m_debugVisProgram->initialize();
	}

	void CBgfxGraphicsClusterRender::onUpdate(float dt)
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

		m_clusters.setUniforms(da::core::CCamera::getCamera(), m_width, m_height);

		
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
		bool buildClusters = glm::any(glm::equal(m_projMat, m_oldProjMat, 0.00001f));
		if (buildClusters)
		{
			m_oldProjMat = m_projMat;

			m_clusters.bindBuffers(false /*lightingPass*/); // write access, all buffers

			::bgfx::dispatch(vClusterBuilding,
				{ m_clusterBuildingComputeProgram->getHandle() },
				CBgfxClusterShader::CLUSTERS_X / CBgfxClusterShader::CLUSTERS_X_THREADS,
				CBgfxClusterShader::CLUSTERS_Y / CBgfxClusterShader::CLUSTERS_Y_THREADS,
				CBgfxClusterShader::CLUSTERS_Z / CBgfxClusterShader::CLUSTERS_Z_THREADS);
		}

		// light culling
		m_clusters.bindBuffers(false);

		// reset atomic counter for light grid generation
		// buffers created with BGFX_BUFFER_COMPUTE_WRITE can't be updated from the CPU
		// this used to happen during cluster building when it was still run every frame
		::bgfx::dispatch(vLightCulling, { m_resetCounterComputeProgram->getHandle()}, 1, 1, 1);

		// m_lights.bindLights(scene); TODO
		m_clusters.bindBuffers(false);

		::bgfx::dispatch(vLightCulling,
			{ m_lightCullingComputeProgram->getHandle() },
			CBgfxClusterShader::CLUSTERS_X / CBgfxClusterShader::CLUSTERS_X_THREADS,
			CBgfxClusterShader::CLUSTERS_Y / CBgfxClusterShader::CLUSTERS_Y_THREADS,
			CBgfxClusterShader::CLUSTERS_Z / CBgfxClusterShader::CLUSTERS_Z_THREADS);
		// lighting

		bool debugVis = false;
		::bgfx::ProgramHandle program = { debugVis ? m_debugVisProgram->getHandle() : m_lightingProgram->getHandle() };

		uint64_t state = BGFX_STATE_DEFAULT & ~BGFX_STATE_CULL_MASK;

		m_pbr.bindAlbedoLUT();
		/* TO DO
		
		lights.bindLights(scene);
		*/
		
		m_clusters.bindBuffers(true); // read access, only light grid and indices

		da::core::CScene* scene = da::core::CSceneManager::getScene();

		const da::core::FComponentContainer& container = scene->getComponents<da::core::CSmeshComponent>();

		for (size_t i = 0; i < container.getCount(); i++) {
			da::core::CSmeshComponent* mesh = container.getComponentAtIndex<da::core::CSmeshComponent>(i);
			glm::mat4 model = mesh->getParent().getTransform().getMat();
			::bgfx::setTransform(glm::value_ptr(model));
			setNormalMatrix(model);
			::bgfx::setVertexBuffer(0, *((::bgfx::VertexBufferHandle*)mesh->getStaticMesh()->getNativeVB()));
			::bgfx::setIndexBuffer(*((::bgfx::IndexBufferHandle*)mesh->getStaticMesh()->getNativeIB()));
			//const Material& mat = scene->materials[mesh.material];
			//uint64_t materialState = m_pbr.bindMaterial(mat);
			::bgfx::setState(state | 0);//materialState);
			// preserve buffer bindings between submit calls
			::bgfx::submit(vLighting, program, 0, ~BGFX_DISCARD_BINDINGS);
		}
		
		::bgfx::discard(BGFX_DISCARD_ALL);
	}

	void CBgfxGraphicsClusterRender::onShutdown()
	{
		m_clusterBuildingComputeProgram->shutdown();
		delete m_clusterBuildingComputeProgram;

		m_resetCounterComputeProgram->shutdown();
		delete m_resetCounterComputeProgram;

		m_lightCullingComputeProgram->shutdown();
		delete m_lightCullingComputeProgram;

		m_lightingProgram->shutdown();
		delete m_lightingProgram;

		m_debugVisProgram->shutdown();
		delete m_debugVisProgram;

		m_clusters.shutdown();
	}

}
#endif