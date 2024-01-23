#pragma once

#ifdef DA_GRAPHICS_BGFX 

#include <core/graphics/graphics_api.h>
#include <core/window/window.h>
#include <bgfx/bgfx.h>
#include <glm/glm.hpp>
#include "bgfx_graphics_material.h"
#include "cluster/bgfx_pbr_shader.h"

namespace da::platform::bgfx {

	#define INVALID_HANDLE UINT16_MAX 

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

	class CBgfxRenderer {


	public:
		CBgfxRenderer();
		void initialize();
		void update(float dt);
		void lateUpdate(float dt);
		void reset(uint16_t width, uint16_t height);
		void shutdown();

	protected:
		virtual void onInitialize() {};
		virtual void onUpdate(float dt) {};
		virtual void onShutdown() {};

	protected:
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

		static constexpr ::bgfx::ViewId MAX_VIEW = 199; // imgui in bigg uses view 200

		void setViewProjection(::bgfx::ViewId view);
		void setNormalMatrix(const glm::mat4& modelMat);

		void blitToScreen(::bgfx::ViewId view = MAX_VIEW);

		static ::bgfx::TextureFormat::Enum findDepthFormat(uint64_t textureFlags, bool stencil = false);
		static ::bgfx::FrameBufferHandle createFrameBuffer(bool hdr = true, bool depth = true);

		std::unordered_map<std::string, std::string> variables;

		TonemappingMode tonemappingMode = TonemappingMode::NONE;

		uint16_t m_width = 0;
		uint16_t m_height = 0;

		uint32_t m_clearColor = 0x303030FF;
		float time = 0.0f;

		// set by setViewProjection()
		glm::mat4 m_viewMat = glm::mat4(1.0);
		glm::mat4 m_projMat = glm::mat4(1.0);

        ::bgfx::VertexBufferHandle m_blitTriangleBuffer = {INVALID_HANDLE};
		::bgfx::FrameBufferHandle m_frameBuffer = BGFX_INVALID_HANDLE;
		::bgfx::FrameBufferHandle m_skyFrameBuffer = BGFX_INVALID_HANDLE;

		CBgfxPBRShader m_pbr;

	private:
		da::platform::CBgfxGraphicsMaterial m_blitProgram;
        ::bgfx::UniformHandle m_blitSampler = {INVALID_HANDLE};
        ::bgfx::UniformHandle m_skySampler = {INVALID_HANDLE};
        ::bgfx::UniformHandle m_camPosUniform = {INVALID_HANDLE};
        ::bgfx::UniformHandle m_normalMatrixUniform = {INVALID_HANDLE};
        ::bgfx::UniformHandle m_exposureVecUniform = {INVALID_HANDLE};
        ::bgfx::UniformHandle m_tonemappingModeVecUniform = {INVALID_HANDLE};


	};
}

#endif
