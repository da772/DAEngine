project "bullet3"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-Obj/" .. outputdir .. "/%{prj.name}")

	files
	{
		"src/**.cpp",
		"src/**.h",
		"src/**.hpp"
	}

	includedirs
	{
		"src"
	}

	flags
	{
		"MultiProcessorCompile"
	}

	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
	filter "configurations:Release"
		runtime "Release"
		optimize "On"
		symbols "On"
	filter "configurations:Final"
		runtime "Release"
		optimize "On"