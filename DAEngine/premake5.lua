project "DAEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	pchheader "dapch.h"
	pchsource "src/dapch.cpp"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-Obj/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"src/**.h",
		"src/**.cpp"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"GE_ENGINE"
	}

	includedirs
	{
		"%{prj.location}",
		"src/",
		"src/DAEngine"
	}

	filter "system:macosx"
		systemversion "latest"
		defines
		{
			"DA_PLATFORM_MACOSX"
		}

		xcodebuildsettings
		{ 
			["ALWAYS_SEARCH_USER_PATHS"] = "YES",
			["SKIP_INSTALL"] = "YES"
		}

		links 
		{

		}

		filter "configurations:Debug"
			defines "DA_DEBUG"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines "DA_RELEASE"
			runtime "Release"
			optimize "On"
		
		filter "configurations:Final"
			defines "DA_FINAL"
			runtime "Release"
			optimize "On"
		
	filter "system:ios"
		architecture "ARM"

		pchheader "src/dapch.h"
		pchsource "%{prj.location}src/dapch.cpp"

		defines
		{
			"DA_PLATFORM_IOS"
		}

		xcodebuildsettings
		{ 
			["ALWAYS_SEARCH_USER_PATHS"] = "YES",
			["ARCHS"] = "$(ARCHS_STANDARD)",
			["SKIP_INSTALL"] = "YES"
		}

		filter "configurations:Debug"
			defines "DA_DEBUG"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines "DA_RELEASE"
			runtime "Release"
			optimize "On"
		
		filter "configurations:Final"
			defines "DA_FINAL"
			runtime "Release"
			optimize "On"

	filter "system:android"
		toolset "clang"
		rtti ("On")
		cppdialect "gnu++17"
		linkoptions { "-lm" }
		exceptionhandling ("On")
		androidapilevel(android_version)

		defines
		{
			"DA_PLATFORM_ANDROID"
		}

		filter "platforms:x86"
			architecture "x86"
			system "android"

		filter "platforms:x64"
			architecture "x64"
			system "android"

		filter "platforms:ARM"
			architecture "ARM"
			system "android"

		filter "platforms:ARM64"
			architecture "ARM64"
			system "android"
			
		filter "configurations:Debug"
			defines "DA_DEBUG"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines "DA_RELEASE"
			runtime "Release"
			optimize "On"
		
		filter "configurations:Final"
			defines "DA_FINAL"
			runtime "Release"
			optimize "On"

	filter "system:windows"
		systemversion "latest"
		
		defines
		{
			"DA_PLATFORM_WINDOWS"
		}

		filter "configurations:Debug"
			defines "DA_DEBUG"
			runtime "Debug"
			symbols "On"
		filter "configurations:Release"
			defines "DA_RELEASE"
			runtime "Release"
			optimize "On"
		filter "configurations:Final"
			defines "DA_FINAL"
			runtime "Release"
			optimize "On"

		filter "system:linux"
			linkgroups 'on'
			systemversion "latest"
			cppdialect "gnu++20"
			
			defines
			{
				"DA_PLATFORM_LINUX"
			}
			
			filter "configurations:Debug"
				defines "DA_DEBUG"
				runtime "Debug"
				symbols "On"
			filter "configurations:Release"
				defines "DA_RELEASE"
				runtime "Release"
				optimize "On"
			filter "configurations:Final"
				defines "DA_FINAL"
				runtime "Release"
				optimize "On"