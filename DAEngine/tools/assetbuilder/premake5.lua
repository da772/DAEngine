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
		"%{wks.location}/../thirdparty/glm/include"
	}


    files {
		"src/**.h",
		"src/**.cpp",
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
