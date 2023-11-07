#pragma once

#ifdef DA_GRAPHICS_BGFX 

#include <core/graphics/graphics_api.h>
#include <core/window/window.h>
#include <bgfx/bgfx.h>
#include <glm/glm.hpp>
#include "bgfx_graphics_material.h"

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
		void initalize();
		void update();
		void shutdown();

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

		uint16_t width = 0;
		uint16_t height = 0;

		uint32_t clearColor = 0;
		float time = 0.0f;

		// set by setViewProjection()
		glm::mat4 viewMat = glm::mat4(1.0);
		glm::mat4 projMat = glm::mat4(1.0);

		::bgfx::VertexBufferHandle blitTriangleBuffer = INVALID_HANDLE;

	private:
		CBgfxGraphicsMaterial blitProgram;
		::bgfx::UniformHandle blitSampler = INVALID_HANDLE;
		::bgfx::UniformHandle camPosUniform = INVALID_HANDLE;
		::bgfx::UniformHandle normalMatrixUniform = INVALID_HANDLE;
		::bgfx::UniformHandle exposureVecUniform = INVALID_HANDLE;
		::bgfx::UniformHandle tonemappingModeVecUniform = INVALID_HANDLE;


	};
}

#endif