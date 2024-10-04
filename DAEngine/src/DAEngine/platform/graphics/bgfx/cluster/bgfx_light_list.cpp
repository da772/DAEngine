
#include "bgfx_light_list.h"

#include <algorithm>
#include <glm/gtc/type_ptr.hpp>
#include <graphics/camera.h>
#include "core/logger.h"

namespace da {

    bgfx::VertexLayout CBgfxLightList::PointLightVertex::layout;

    void CBgfxPointLightList::init()
    {
        CBgfxLightList::PointLightVertex::init();
        m_buffer = bgfx::createDynamicVertexBuffer(
            1, PointLightVertex::layout, BGFX_BUFFER_COMPUTE_READ | BGFX_BUFFER_ALLOW_RESIZE);
    }

    void CBgfxPointLightList::shutdown()
    {
        ASSERT(bgfx::isValid(m_buffer))
        bgfx::destroy(m_buffer);
        m_buffer = BGFX_INVALID_HANDLE;
    }

    void CBgfxPointLightList::update(const std::vector<da::FLightData>& lights)
    {
        size_t stride = PointLightVertex::layout.getStride();
        const bgfx::Memory* mem = bgfx::alloc(uint32_t(stride * std::max(lights.size(), (size_t)1)));

        for (size_t i = 0; i < lights.size(); i++)
        {
            PointLightVertex* light = (PointLightVertex*)(mem->data + (i * stride));

            light->position = lights[i].position;
            light->intensity = lights[i].intensity;
            light->radius = lights[i].radius;
            light->direction = lights[i].direction;
            light->angle = lights[i].angle;
        }

        bgfx::update(m_buffer, 0, mem);
    }
}