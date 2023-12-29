#pragma once

#include <bgfx/bgfx.h>
#include "bgfx_pbr_shader.h"
#include "bgfx_light_shader.h"
#include <glm/matrix.hpp>
#include <unordered_map>
#include <string>

class Scene;

namespace da::platform
{
    class CBgfxGraphicsMaterial;

    class CBgfxTypeRenderer
    {
    public:
        CBgfxTypeRenderer();
        virtual ~CBgfxTypeRenderer() { }

        void initialize();
        void reset(uint16_t width, uint16_t height);
        void render(float dt);
        void shutdown();

        enum class TonemappingMode : int
        {
            NONE = 0,
            EXPONENTIAL,
            REINHARD,
            REINHARD_LUM,
            HABLE,
            DUIKER,
            ACES,
            ACES_LUM
        };

        void setTonemappingMode(TonemappingMode mode);
        void setMultipleScattering(bool enabled);
        void setWhiteFurnace(bool enabled);

        static bool supported();
        static const char* shaderDir();

        // subclasses should override these

        // the first reset happens before initialize
        virtual void onInitialize() { }
        // window resize/flags changed (MSAA, V-Sync, ...)
        virtual void onReset() { }
        virtual void onRender(float dt) = 0;
        virtual void onShutdown() { }

        // buffers for debug output (display in the UI)

        struct TextureBuffer
        {
            bgfx::TextureHandle Handle;
            const char* Name;
        };

        TextureBuffer* m_buffers = nullptr;

        // final output
        // used for tonemapping
        bgfx::FrameBufferHandle m_frameBuffer = BGFX_INVALID_HANDLE;

    protected:
        struct PosVertex
        {
            float x;
            float y;
            float z;

            static void init()
            {
                layout.begin().add(bgfx::Attrib::Position, 3, bgfx::AttribType::Float).end();
            }

            static ::bgfx::VertexLayout layout;
        };

        static constexpr ::bgfx::ViewId MAX_VIEW = 199; // imgui in bigg uses view 200

        void setViewProjection(::bgfx::ViewId view);
        void setNormalMatrix(const glm::mat4& modelMat);

        void blitToScreen(::bgfx::ViewId view = MAX_VIEW);

        static ::bgfx::TextureFormat::Enum findDepthFormat(uint64_t textureFlags, bool stencil = false);
        static ::bgfx::FrameBufferHandle createFrameBuffer(bool hdr = true, bool depth = true);

        TonemappingMode m_tonemappingMode = TonemappingMode::ACES;

        uint16_t m_width = 0;
        uint16_t m_height = 0;

        CBgfxPBRShader m_pbr;
        CBgfxLightShader m_lights;

        uint32_t m_clearColor = 0;
        float m_time = 0.0f;

        // set by setViewProjection()
        glm::mat4 m_viewMat = glm::mat4(1.0);
        glm::mat4 m_projMat = glm::mat4(1.0);

        ::bgfx::VertexBufferHandle m_blitTriangleBuffer = BGFX_INVALID_HANDLE;

    private:
        ::bgfx::UniformHandle m_blitSampler = BGFX_INVALID_HANDLE;
        ::bgfx::UniformHandle m_camPosUniform = BGFX_INVALID_HANDLE;
        ::bgfx::UniformHandle m_normalMatrixUniform = BGFX_INVALID_HANDLE;
        ::bgfx::UniformHandle m_exposureVecUniform = BGFX_INVALID_HANDLE;
        ::bgfx::UniformHandle m_tonemappingModeVecUniform = BGFX_INVALID_HANDLE;
        da::platform::CBgfxGraphicsMaterial* m_pBlipProgram;
    };
}
