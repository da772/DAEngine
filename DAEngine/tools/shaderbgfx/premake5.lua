include "../../thirdparty/bgfx/tools/shaderc"

group "tools"

local SHADERC_DIR = "../../thirdparty/bgfx/tools/shaderc"

local BGFX_DIR = "../../thirdparty/bgfx"
local BX_DIR = "../../thirdparty/bx"
local BIMG_DIR = "../../thirdparty/bimg"

local GLSL_OPTIMIZER = path.join(BGFX_DIR, "3rdparty/glsl-optimizer")
local FCPP_DIR       = path.join(BGFX_DIR, "3rdparty/fcpp")
local GLSLANG        = path.join(BGFX_DIR, "3rdparty/glslang")
local SPIRV_CROSS    = path.join(BGFX_DIR, "3rdparty/spirv-cross")
local SPIRV_HEADERS  = path.join(BGFX_DIR, "3rdparty/spirv-headers")
local SPIRV_TOOLS    = path.join(BGFX_DIR, "3rdparty/spirv-tools")

project "shaderbgfx"
	kind "ConsoleApp"
    language "C++"
	cppdialect "C++20"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-Obj/" .. outputdir .. "/%{prj.name}")
	includedirs 
    {
		path.join(BIMG_DIR, "include"),
		path.join(BGFX_DIR, "include"),
        path.join(BX_DIR, "include"),

		path.join(BGFX_DIR, "3rdparty/webgpu/include"),
		path.join(BGFX_DIR, "3rdparty/dxsdk/include"),

		FCPP_DIR,

		path.join(BGFX_DIR, "3rdparty/glslang/glslang/Public"),
		path.join(BGFX_DIR, "3rdparty/glslang/glslang/Include"),
		path.join(BGFX_DIR, "3rdparty/glslang"),

		path.join(GLSL_OPTIMIZER, "include"),
		path.join(GLSL_OPTIMIZER, "src/glsl"),

		SPIRV_CROSS,

		path.join(SPIRV_TOOLS, "include"),
        path.join(BGFX_DIR, "include/compat/msvc"),
		SHADERC_DIR
	}


    files {
		"src/**.h",
		"src/**.cpp",
	}

	links {
		"fcpp",
		"glslang",
		"glsl-optimizer",
		"spirv-opt",
		"spirv-cross",
        "bx",
		"shaderc"
	}

    filter "configurations:Release"
        defines "BX_CONFIG_DEBUG=0"
    filter "configurations:Debug"
        defines "BX_CONFIG_DEBUG=1"

    filter "system:windows"
        buildoptions {"/Zc:__cplusplus"}
        defines
        {
            "__STDC_LIMIT_MACROS"
        }
        includedirs
        {
            path.join(GLSL_OPTIMIZER, "include/c99")
        }

	filter "system:macosx"
		links
		{
			"CoreFoundation.framework",
		}




	

	configuration {}

group ""