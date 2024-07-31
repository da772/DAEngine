project "bx"
	kind "StaticLib"
	language "C++"
	cppdialect "C++14"
	staticruntime "on"
	defines "__STDC_FORMAT_MACROS"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-Obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/bx/*.h",
		"include/bx/inline/*.inl",
		"src/*.cpp"
	}
	excludes
	{
		"src/amalgamated.cpp",
		"src/crtnone.cpp"
	}
	includedirs
	{
		"3rdparty",
		"include"
	}

	flags
	{
		"MultiProcessorCompile"
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