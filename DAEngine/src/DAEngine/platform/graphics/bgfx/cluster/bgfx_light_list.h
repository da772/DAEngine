#pragma once

#include "bgfx_light.h"
#include <bgfx/bgfx.h>
#include <vector>

namespace da::platform {

    struct CBgfxLightList
    {
        // vertex buffers seem to be aligned to 16 bytes
        struct PointLightVertex
        {
            glm::vec3 position;
            float padding;
            // radiant intensity in W/sr
            // can be calculated from radiant flux
            glm::vec3 intensity;
            float radius;

            static void init()
            {
                layout.begin()
                    .add(bgfx::Attrib::TexCoord0, 4, bgfx::AttribType::Float)
                    .add(bgfx::Attrib::TexCoord1, 4, bgfx::AttribType::Float)
                    .end();
            }
            static bgfx::VertexLayout layout;
        };
    };

    class CBgfxPointLightList : public CBgfxLightList
    {
    public:
        void init();
        void shutdown();

        // upload changes to GPU
        void update();

        std::vector<CBgfxPointLight> lights;

        bgfx::DynamicVertexBufferHandle buffer = BGFX_INVALID_HANDLE;
    };

}