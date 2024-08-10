
#include "bgfx_cluster_shader.h"

#include "bgfx_samplers.h"
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <graphics/camera.h>
#include "../bgfx_util.h"

namespace da::platform {

    bgfx::VertexLayout CBgfxClusterShader::ClusterVertex::layout;

    CBgfxClusterShader::CBgfxClusterShader()
    {
        static_assert(CLUSTERS_Z % CLUSTERS_Z_THREADS == 0,
            "number of cluster depth slices must be divisible by thread count z-dimension");
    }

    void CBgfxClusterShader::initialize()
    {
        ClusterVertex::init();

        m_clusterSizesVecUniform = bgfx::createUniform("u_clusterSizesVec", bgfx::UniformType::Vec4);
        m_zNearFarVecUniform = bgfx::createUniform("u_zNearFarVec", bgfx::UniformType::Vec4);

        m_clustersBuffer =
            bgfx::createDynamicVertexBuffer(CLUSTER_COUNT, ClusterVertex::layout, BGFX_BUFFER_COMPUTE_READ_WRITE);
        m_lightIndicesBuffer = bgfx::createDynamicIndexBuffer(CLUSTER_COUNT * MAX_LIGHTS_PER_CLUSTER,
            BGFX_BUFFER_COMPUTE_READ_WRITE | BGFX_BUFFER_INDEX32);
        // we have to specify the compute buffer format here since we need uvec4
        // not needed for the rest, the default format for vertex/index buffers is vec4/uint
        m_lightGridBuffer =
            bgfx::createDynamicIndexBuffer(CLUSTER_COUNT * 4,
                BGFX_BUFFER_COMPUTE_READ_WRITE | BGFX_BUFFER_INDEX32 |
                BGFX_BUFFER_COMPUTE_FORMAT_32X4 | BGFX_BUFFER_COMPUTE_TYPE_UINT);
        m_atomicIndexBuffer = bgfx::createDynamicIndexBuffer(1, BGFX_BUFFER_COMPUTE_READ_WRITE | BGFX_BUFFER_INDEX32);
    }

    void CBgfxClusterShader::shutdown()
    {

        BGFXDESTROY(m_clusterSizesVecUniform);
        BGFXDESTROY(m_zNearFarVecUniform);

        
        BGFXDESTROY(m_clustersBuffer);
        
        BGFXDESTROY(m_lightIndicesBuffer);
        BGFXDESTROY(m_lightGridBuffer);
        BGFXDESTROY(m_atomicIndexBuffer);
    }

    void CBgfxClusterShader::setUniforms(uint16_t screenWidth, uint16_t screenHeight) const
    {
        float clusterSizesVec[4] = { std::ceil((float)screenWidth / CLUSTERS_X),
                                     std::ceil((float)screenHeight / CLUSTERS_Y) };

        bgfx::setUniform(m_clusterSizesVecUniform, clusterSizesVec);
        float zNearFarVec[4] = { da::core::CCamera::getCamera()->zNear, da::core::CCamera::getCamera()->zFar };
        bgfx::setUniform(m_zNearFarVecUniform, zNearFarVec);
    }

    void CBgfxClusterShader::bindBuffers(bool lightingPass) const
    {
        // binding ReadWrite in the fragment shader doesn't work with D3D11/12
        bgfx::Access::Enum access = lightingPass ? bgfx::Access::Read : bgfx::Access::ReadWrite;
        if (!lightingPass)
        {
            bgfx::setBuffer(CBgfxSamplers::CLUSTERS_CLUSTERS, m_clustersBuffer, access);
            bgfx::setBuffer(CBgfxSamplers::CLUSTERS_ATOMICINDEX, m_atomicIndexBuffer, access);
        }
        bgfx::setBuffer(CBgfxSamplers::CLUSTERS_LIGHTINDICES, m_lightIndicesBuffer, access);
        bgfx::setBuffer(CBgfxSamplers::CLUSTERS_LIGHTGRID, m_lightGridBuffer, access);
    }

}