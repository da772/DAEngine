project "bgfx"
	kind "StaticLib"
	language "C++"
	cppdialect "C++14"
	defines "__STDC_FORMAT_MACROS"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-Obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/bgfx/**.h",
		"src/*.cpp",
		"src/*.h"
	}
	excludes
	{
		"src/amalgamated.cpp"
	}
	includedirs
	{
		"%{IncludeDir.bx}",
		"%{IncludeDir.bimg}",
		"include",
		"3rdparty",
		"3rdparty/dxsdk/include",
		"3rdparty/khronos",
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		defines "BX_CONFIG_DEBUG=1"
	filter "configurations:Release"
		runtime "Release"
		optimize "On"
		symbols "On"
		defines "BX_CONFIG_DEBUG=1"
	filter "configurations:Final"
		runtime "Release"
		optimize "On"
		defines "BX_CONFIG_DEBUG=0"
		
		
	filter "configurations:Debug"
		defines "BX_CONFIG_DEBUG=1"
	filter "action:vs*"
		defines "_CRT_SECURE_NO_WARNINGS"
		buildoptions {"/Zc:__cplusplus"}

	filter {"system:windows"}
		includedirs {
			"3rdparty/directx-headers/include/directx"
		}
		
	filter "action:vs*"
		includedirs { "include/compat/msvc"}
		defines "_CRT_SECURE_NO_WARNINGS"
		excludes
		{
			"src/glcontext_glx.cpp",
			"src/glcontext_egl.cpp"
		}
	filter { "system:windows", "action:gmake" }
		includedirs { "include/compat/mingw" }
	filter { "system:macosx" }
		includedirs { "include/compat/osx" }
		buildoptions { "-x objective-c++" }
		xcodebuildsettings
		{ 
			["ALWAYS_SEARCH_USER_PATHS"] = "YES"
		}
		defines
		{
			"BGFX_CONFIG_MULTITHREADED=0"
		}
		files
		{
			"src/*.mm",
		}