project "bimg"
	kind "StaticLib"
	language "C++"
	cppdialect "C++14"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-Obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/bimg/*.h",
		"src/image.cpp",
		"src/image_decode.cpp",
		"src/image_gnf.cpp",
		"src/*.h",
		"3rdparty/astc-encoder/source/*.cpp",
		"3rdparty/**.cpp",
		"3rdparty/**.c",
		"3rdparty/**.h"
	}
	excludes
	{
		"src/amalgamated.cpp",
		"src/crtnone.cpp"
	}
	includedirs
	{
		"%{IncludeDir.bx}",
		"include",
		"src",
		"3rdparty/astc-encoder/include",
		"3rdparty/",
		"3rdparty/iqa/include",
		"3rdparty/nvtt",
		"3rdparty/tinyexr/deps/miniz",
	}


	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		defines "BX_CONFIG_DEBUG=1"
	filter "configurations:Release"
		runtime "Release"
		optimize "On"
		symbols "On"
	filter "configurations:Final"
		runtime "Release"
		optimize "On"
		defines "BX_CONFIG_DEBUG=0"


	filter "configurations:Release"
		defines "BX_CONFIG_DEBUG=0"
	filter "configurations:Debug"
		defines "BX_CONFIG_DEBUG=1"
	filter "action:vs*"
		defines "_CRT_SECURE_NO_WARNINGS"
		buildoptions {"/Zc:__cplusplus"}

	filter "action:vs*"
		includedirs { "include/compat/msvc"}
	filter { "system:windows", "action:gmake" }
		includedirs { "include/compat/mingw" }
	filter { "system:macosx" }
		includedirs { "include/compat/osx" }
		buildoptions { "-x objective-c++" }
		xcodebuildsettings
		{ 
			["ALWAYS_SEARCH_USER_PATHS"] = "YES"
		}