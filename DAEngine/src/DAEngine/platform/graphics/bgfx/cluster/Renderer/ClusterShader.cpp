#include "dapch.h"
#include "ClusterShader.h"

#include "../Scene/Scene.h"
#include "Samplers.h"
#include <glm/common.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <core/graphics/camera.h>

bgfx::VertexLayout ClusterShader::ClusterVertex::layout;

ClusterShader::ClusterShader()
{
    static_assert(CLUSTERS_Z % CLUSTERS_Z_THREADS == 0,
                  "number of cluster depth slices must be divisible by thread count z-dimension");
}

void ClusterShader::initialize()
{
    ClusterVertex::init();

    clusterSizesVecUniform = bgfx::createUniform("u_clusterSizesVec", bgfx::UniformType::Vec4);
    zNearFarVecUniform = bgfx::createUniform("u_zNearFarVec", bgfx::UniformType::Vec4);

    clustersBuffer =
        bgfx::createDynamicVertexBuffer(CLUSTER_COUNT, ClusterVertex::layout, BGFX_BUFFER_COMPUTE_READ_WRITE);
    lightIndicesBuffer = bgfx::createDynamicIndexBuffer(CLUSTER_COUNT * MAX_LIGHTS_PER_CLUSTER,
                                                        BGFX_BUFFER_COMPUTE_READ_WRITE | BGFX_BUFFER_INDEX32);
    // we have to specify the compute buffer format here since we need uvec4
    // not needed for the rest, the default format for vertex/index buffers is vec4/uint
    lightGridBuffer =
        bgfx::createDynamicIndexBuffer(CLUSTER_COUNT * 4,
                                       BGFX_BUFFER_COMPUTE_READ_WRITE | BGFX_BUFFER_INDEX32 |
                                           BGFX_BUFFER_COMPUTE_FORMAT_32X4 | BGFX_BUFFER_COMPUTE_TYPE_UINT);
    atomicIndexBuffer = bgfx::createDynamicIndexBuffer(1, BGFX_BUFFER_COMPUTE_READ_WRITE | BGFX_BUFFER_INDEX32);
}

void ClusterShader::shutdown()
{
    bgfx::destroy(clusterSizesVecUniform);
    bgfx::destroy(zNearFarVecUniform);

    bgfx::destroy(clustersBuffer);
    bgfx::destroy(lightIndicesBuffer);
    bgfx::destroy(lightGridBuffer);
    bgfx::destroy(atomicIndexBuffer);

    clusterSizesVecUniform = zNearFarVecUniform = BGFX_INVALID_HANDLE;
    clustersBuffer = BGFX_INVALID_HANDLE;
    lightIndicesBuffer = lightGridBuffer = atomicIndexBuffer = BGFX_INVALID_HANDLE;
}

void ClusterShader::setUniforms(uint16_t screenWidth, uint16_t screenHeight) const
{
    float clusterSizesVec[4] = { std::ceil((float)screenWidth / CLUSTERS_X),
                                 std::ceil((float)screenHeight / CLUSTERS_Y) };

    bgfx::setUniform(clusterSizesVecUniform, clusterSizesVec);
    float zNearFarVec[4] = { da::core::CCamera::getCamera()->zNear, da::core::CCamera::getCamera()->zFar };
    bgfx::setUniform(zNearFarVecUniform, zNearFarVec);
}

void ClusterShader::bindBuffers(bool lightingPass) const
{
    // binding ReadWrite in the fragment shader doesn't work with D3D11/12
    bgfx::Access::Enum access = lightingPass ? bgfx::Access::Read : bgfx::Access::ReadWrite;
    if(!lightingPass)
    {
        bgfx::setBuffer(Samplers::CLUSTERS_CLUSTERS, clustersBuffer, access);
        bgfx::setBuffer(Samplers::CLUSTERS_ATOMICINDEX, atomicIndexBuffer, access);
    }
    bgfx::setBuffer(Samplers::CLUSTERS_LIGHTINDICES, lightIndicesBuffer, access);
    bgfx::setBuffer(Samplers::CLUSTERS_LIGHTGRID, lightGridBuffer, access);
}
