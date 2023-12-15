#pragma once

#include "DAEngine/core/core.h"

#ifdef DA_GRAPHICS_BGFX

#include <glm/glm.hpp>
#include <bgfx/bgfx.h>
#include "bgfx_cluster_shader.h"
#include "daengine/platform/graphics/bgfx/bgfx_renderer.h"


namespace da::platform {
	class CBgfxGraphicsMaterial;
}


namespace da::platform::bgfx {
	class CBgfxGraphicsClusterRender : public CBgfxRenderer
	{
	protected:
		virtual void onInitialize() override;
		virtual void onUpdate(float dt) override;
		virtual void onShutdown() override;

	private:

		::bgfx::FrameBufferHandle m_frameBuffer = BGFX_INVALID_HANDLE;

		glm::mat4 m_oldProjMat = glm::mat4(0.0f);

		da::platform::CBgfxGraphicsMaterial* m_clusterBuildingComputeProgram;
		da::platform::CBgfxGraphicsMaterial* m_resetCounterComputeProgram;
		da::platform::CBgfxGraphicsMaterial* m_lightCullingComputeProgram;;
		da::platform::CBgfxGraphicsMaterial* m_lightingProgram;
		da::platform::CBgfxGraphicsMaterial* m_debugVisProgram;

		CBgfxClusterShader m_clusters;
	};
}

#endif