#pragma once
#include "core.h"
#include "bgfx_light.h"
#include <bgfx/bgfx.h>
#include <vector>
#include <graphics/graphics_light_manager.h>

namespace da {

    struct CBgfxLightList
    {
        // vertex buffers seem to be aligned to 16 bytes
        struct PointLightVertex
        {
			glm::vec3 position;
			uint32_t padding;
			// radiant intensity in W/sr
			// can be calculated from radiant flux
			glm::vec3 intensity;
			float radius;
            glm::vec3 direction;
			float angle;

            static void init()
            {
                layout.begin()
                    .add(::bgfx::Attrib::TexCoord0, 4, ::bgfx::AttribType::Float)
                    .add(::bgfx::Attrib::TexCoord1, 4, ::bgfx::AttribType::Float)
                    .add(::bgfx::Attrib::TexCoord2, 4, ::bgfx::AttribType::Float)
                    .end();
            }
            static ::bgfx::VertexLayout layout;
        };
    };

    class CBgfxPointLightList : public CBgfxLightList
    {
    public:
        void init();
        void shutdown();

        // upload changes to GPU
        void update(const std::vector<da::FLightData>& lights);
        inline const ::bgfx::DynamicVertexBufferHandle getBuffer() const { return m_buffer; };


    private:
        ::bgfx::DynamicVertexBufferHandle m_buffer = BGFX_INVALID_HANDLE;
    };

}