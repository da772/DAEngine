#include "dapch.h"
#include "bgfx_cluster_shader.h"

#ifdef DA_GRAPHICS_BGFX

#include <cmath>
#include "bgfx_samplers.h"
#include "daengine/core/graphics/camera.h"

namespace da::platform::bgfx {

	::bgfx::VertexLayout CBgfxClusterShader::ClusterVertex::layout;

	CBgfxClusterShader::CBgfxClusterShader()
	{

	}

	void CBgfxClusterShader::initialize()
	{
		ClusterVertex::init();

		m_clusterSizesVecUniform = ::bgfx::createUniform("u_clusterSizesVec", ::bgfx::UniformType::Vec4);
		m_zNearFarVecUniform = ::bgfx::createUniform("u_zNearFarVec", ::bgfx::UniformType::Vec4);

		m_clustersBuffer =
			::bgfx::createDynamicVertexBuffer(CLUSTER_COUNT, ClusterVertex::layout, BGFX_BUFFER_COMPUTE_READ_WRITE);
		m_lightIndicesBuffer = ::bgfx::createDynamicIndexBuffer(CLUSTER_COUNT * MAX_LIGHTS_PER_CLUSTER,
			BGFX_BUFFER_COMPUTE_READ_WRITE | BGFX_BUFFER_INDEX32);
		// we have to specify the compute buffer format here since we need uvec4
		// not needed for the rest, the default format for vertex/index buffers is vec4/uint
		m_lightGridBuffer =
			::bgfx::createDynamicIndexBuffer(CLUSTER_COUNT * 4,
				BGFX_BUFFER_COMPUTE_READ_WRITE | BGFX_BUFFER_INDEX32 |
				BGFX_BUFFER_COMPUTE_FORMAT_32X4 | BGFX_BUFFER_COMPUTE_TYPE_UINT);
		m_atomicIndexBuffer = ::bgfx::createDynamicIndexBuffer(1, BGFX_BUFFER_COMPUTE_READ_WRITE | BGFX_BUFFER_INDEX32);
	}

	void CBgfxClusterShader::shutdown()
	{
		::bgfx::destroy(m_clusterSizesVecUniform);
		::bgfx::destroy(m_zNearFarVecUniform);

		::bgfx::destroy(m_clustersBuffer);
		::bgfx::destroy(m_lightIndicesBuffer);
		::bgfx::destroy(m_lightGridBuffer);
		::bgfx::destroy(m_atomicIndexBuffer);

		m_clusterSizesVecUniform = m_zNearFarVecUniform = BGFX_INVALID_HANDLE;
		m_clustersBuffer = BGFX_INVALID_HANDLE;
		m_lightIndicesBuffer = m_lightGridBuffer = m_atomicIndexBuffer = BGFX_INVALID_HANDLE;
	}

	void CBgfxClusterShader::setUniforms(da::core::CCamera* cam, uint16_t screenWidth, uint16_t screenHeight) const
	{
		float clusterSizesVec[4] = { std::ceil((float)screenWidth / CLUSTERS_X),
									 std::ceil((float)screenHeight / CLUSTERS_Y) };

		::bgfx::setUniform(m_clusterSizesVecUniform, clusterSizesVec);
		float zNearFarVec[4] = { cam->zNear, cam->zFar };
		::bgfx::setUniform(m_zNearFarVecUniform, zNearFarVec);
	}

	void CBgfxClusterShader::bindBuffers(bool lightingPass /*= true*/) const
	{
		// binding ReadWrite in the fragment shader doesn't work with D3D11/12
		::bgfx::Access::Enum access = lightingPass ? ::bgfx::Access::Read : ::bgfx::Access::ReadWrite;
		if (!lightingPass)
		{
			::bgfx::setBuffer(Samplers::CLUSTERS_CLUSTERS, m_clustersBuffer, access);
			::bgfx::setBuffer(Samplers::CLUSTERS_ATOMICINDEX, m_atomicIndexBuffer, access);
		}
		::bgfx::setBuffer(Samplers::CLUSTERS_LIGHTINDICES, m_lightIndicesBuffer, access);
		::bgfx::setBuffer(Samplers::CLUSTERS_LIGHTGRID, m_lightGridBuffer, access);
	}

	

}

#endif