group "3rdparty"
include "../../thirdparty/assimp"
include "../../thirdparty/zlib"
include "../src/DAEngine/stl"

group "tools"
project "assetbuilder"
	kind "ConsoleApp"
    language "C++"
	cppdialect "C++20"
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-Obj/" .. outputdir .. "/%{prj.name}")
	staticruntime "on"

	includedirs 
    {
		"%{wks.location}/thirdparty/nvtt/include",
		"../../thirdparty/assimp/include",
		"../../thirdparty/glm",
		"%{IncludeDir['da-core']}",
		"%{wks.location}/../thirdparty/glm"
	}

    files {
		"src/**.h",
		"src/**.cpp",
		"%{IncludeDir['da-core']}/graphics/animated_bone.h",
		"%{IncludeDir['da-core']}/graphics/animated_bone.cpp"
	}

	links 
	{
		"nvtt30205",
		"assimp",
		"zlib",
		"da-stl"
	}

	libdirs
	{
		"%{wks.location}/thirdparty/nvtt/lib/x64-v142"
	}

    filter "configurations:Release"
    filter "configurations:Debug"
    
	defines
	{
		"DA_TOOLS"
	}

    filter "system:windows"
        buildoptions {"/Zc:__cplusplus"}
        defines
        {
            "__STDC_LIMIT_MACROS", "DA_TOOLS"
        }
        includedirs
        {
            path.join(GLSL_OPTIMIZER, "include/c99")
        }

		flags
		{
			"MultiProcessorCompile"
		}
		

	filter "system:macosx"
		links
		{
			"CoreFoundation.framework",
		}




	

	configuration {}

group ""
