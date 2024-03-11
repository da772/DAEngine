#pragma once

#include <bgfx/bgfx.h>
#include "bgfx_pbr_shader.h"
#include "bgfx_light_shader.h"
#include <glm/matrix.hpp>
#include <unordered_map>
#include <string>
#include "core/enum.h"
#include "bgfx_sky.h"
#include <map>
#include "bgfx_bloom_shader.h"
#include "bgfx_volumetric_light_shader.h"
#include <platform/graphics/bgfx/bgfx_debug_renderer.h>

namespace da::core {
    class CCamera;
}

namespace da::graphics
{
    class CMaterial;
}

namespace da::platform
{
    class CBgfxGraphicsMaterial;

	struct PosVertex
	{
		float x;
		float y;
		float z;

		static void init()
		{
			layout.begin().add(::bgfx::Attrib::Position, 3, ::bgfx::AttribType::Float).end();
		}

		static ::bgfx::VertexLayout layout;
	};

    class CBgfxTypeRenderer
    {
    public:
        CBgfxTypeRenderer();
        virtual ~CBgfxTypeRenderer() { }

        void initialize();
        void reset(uint16_t width, uint16_t height);
        void render(float dt);
        void shutdown();

        #define ETONEMAPPINGMODE_ENUM(e) \
        e(NONE) \
        e(EXPONENTIAL) \
        e(REINHARD) \
        e(REINHARD_LUM) \
        e(HABLE) \
        e(DUIKER) \
        e(ACES) \
        e(ACES_LUM) \
        e(MAX)
        
        ENUM_CREATE(ETonemappingMode, ETONEMAPPINGMODE_ENUM);

        void setTonemappingMode(ETonemappingMode mode);
        void setMultipleScattering(bool enabled);
        void setWhiteFurnace(bool enabled);

        static bool supported();
        static const char* shaderDir();

        // subclasses should override these

        // the first reset happens before initialize
        virtual void onInitialize() { }
        // window resize/flags changed (MSAA, V-Sync, ...)
        virtual void onRender(float dt) = 0;
        virtual void onShutdown() { }

        // buffers for debug output (display in the UI)

        struct TextureBuffer
        {
            ::bgfx::TextureHandle Handle;
            const char* Name;
        };

        TextureBuffer* m_buffers = nullptr;

        // final output
        // used for tonemapping
        ::bgfx::FrameBufferHandle m_frameBuffer = BGFX_INVALID_HANDLE;
        ::bgfx::FrameBufferHandle m_depthBuffer = BGFX_INVALID_HANDLE;

    protected:
        static constexpr ::bgfx::ViewId MAX_VIEW = 199; // imgui in bigg uses view 200

        void setViewProjection(::bgfx::ViewId view, bool rightHanded = true);
        void setViewProjection(::bgfx::ViewId view, const glm::mat4& mat,float fov, float zNear, float zFar, float aspectRatio = 1.0);
        void setNormalMatrix(const glm::mat4& modelMat);

        inline virtual void onReset(size_t width, size_t height) { };

        void blitToScreen(::bgfx::ViewId view = MAX_VIEW);

        static ::bgfx::TextureFormat::Enum findDepthFormat(uint64_t textureFlags, bool stencil = false);
        static ::bgfx::FrameBufferHandle createFrameBuffer(bool hdr = true, bool depth = true);
        ::bgfx::FrameBufferHandle createDepthBuffer();

        ETonemappingMode m_tonemappingMode = ETonemappingMode::ACES_LUM;

        uint16_t m_width = 0;
        uint16_t m_height = 0;

        CBgfxPBRShader m_pbr;
        CBgfxLightShader m_lights;
        CBgfxProcSky m_sky;
        CBgfxSunController m_sun;
        CBgfxBloomShader m_bloom;
        CBgfxVolumetricLightShader m_volumetricLight;
#if defined(DA_DEBUG) || defined(DA_RELEASE)
        CBgfxDebugRenderer m_debugRenderer;
#endif

        uint32_t m_clearColor = 0;
        float m_time = 0.f;

        // set by setViewProjection()
        glm::mat4 m_viewMat = glm::mat4(1.0);
        glm::mat4 m_projMat = glm::mat4(1.0);

        ::bgfx::VertexBufferHandle m_blitTriangleBuffer = BGFX_INVALID_HANDLE;
        da::graphics::CMaterial* m_pDepthprogram;

        ::bgfx::UniformHandle m_bonesUniform = BGFX_INVALID_HANDLE;
    private:
        ::bgfx::UniformHandle m_blitSampler = BGFX_INVALID_HANDLE;
        ::bgfx::UniformHandle m_bloomSampler = BGFX_INVALID_HANDLE;
        ::bgfx::UniformHandle m_volLightSampler = BGFX_INVALID_HANDLE;
        ::bgfx::UniformHandle m_camPosUniform = BGFX_INVALID_HANDLE;
        ::bgfx::UniformHandle m_normalMatrixUniform = BGFX_INVALID_HANDLE;
#if defined(DA_DEBUG) || defined(DA_RELEASE)
        ::bgfx::UniformHandle m_debugSamplerUniform = BGFX_INVALID_HANDLE;
#endif
        
        ::bgfx::UniformHandle m_exposureVecUniform = BGFX_INVALID_HANDLE;
        ::bgfx::UniformHandle m_tonemappingModeVecUniform = BGFX_INVALID_HANDLE;
        da::graphics::CMaterial* m_pBlipProgram;
        
    };
}
