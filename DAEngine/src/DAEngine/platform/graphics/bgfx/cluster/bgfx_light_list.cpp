#include "dapch.h"
#include "bgfx_light_list.h"

#include <algorithm>
#include <glm/gtc/type_ptr.hpp>

namespace da::platform {

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

    void CBgfxPointLightList::update()
    {
        size_t stride = PointLightVertex::layout.getStride();
        const bgfx::Memory* mem = bgfx::alloc(uint32_t(stride * std::max(m_lights.size(), (size_t)1)));

        for (size_t i = 0; i < m_lights.size(); i++)
        {
            PointLightVertex* light = (PointLightVertex*)(mem->data + (i * stride));
            light->position = m_lights[i].position;
            // intensity = flux per unit solid angle (steradian)
            // there are 4*pi steradians in a sphere
            light->intensity = m_lights[i].flux / (4.0f * glm::pi<float>());
            light->radius = m_lights[i].calculateRadius();
        }

        bgfx::update(m_buffer, 0, mem);
    }
}