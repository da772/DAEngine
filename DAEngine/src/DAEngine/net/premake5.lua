project "da-net"
	kind "StaticLib"
	language "C++"
	cppdialect "C++latest"
	staticruntime "on"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"**.h",
		"**.cpp",
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
		"DA_PROFILE"
	}

	if not _OPTIONS['unit-test'] then
	includedirs
	{
		"%{IncludeDir['da-core']}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.zlib}",
		"%{IncludeDir.enet}",
		"%{IncludeDir.miniupnpc}",
		"%{IncludeDir.optick}",
	}
	
	else
	defines
	{
		"DA_TEST"
	}
	includedirs
	{
		"%{wks.location}/DAEngine/src",
		"%{wks.location}/DAEngine/src/DAEngine",
		"%{IncludeDir.glm}"
	}
	end

	filter "system:macosx"
		defines
		{
			"DA_PLATFORM_MACOSX",
			"GLFW_EXPOSE_NATIVE_COCOA"
		}

		xcodebuildsettings
		{ 
			["ALWAYS_SEARCH_USER_PATHS"] = "YES",
			["SKIP_INSTALL"] = "YES"
		}

		if not _OPTIONS['unit-test'] then
		links 
		{
			"da-core",
			"da-debug",
			"imgui",
			"enet",
			"miniupnpc",
			"zlib"
		}
	
		end

		filter "configurations:Debug"
			defines {"DA_DEBUG", "DA_REVIEW"}
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines {"DA_RELEASE", "DA_REVIEW"}
			runtime "Release"
			optimize "On"
		
		filter "configurations:Final"
			defines "DA_FINAL"
			runtime "Release"
			optimize "On"
		
	filter "system:windows"
		systemversion "latest"

		
		flags
		{
			"MultiProcessorCompile"
		}
		
		defines
		{
			"DA_PLATFORM_WINDOWS",
			"GLFW_EXPOSE_NATIVE_WIN32"
		}
		
		if not _OPTIONS['unit-test'] then
		links
		{
			"da-core",
			"da-debug",
			"imgui",
			"enet",
			"miniupnpc",
			"zlib"
		}

		buildoptions
		{
			"/Zc:__cplusplus"
		}
		end

		filter "configurations:Debug"
			defines {"DA_DEBUG", "DA_REVIEW"}
			runtime "Debug"
			symbols "On"
		filter "configurations:Release"
			defines {"DA_RELEASE", "DA_REVIEW"}
			runtime "Release"
			optimize "On"
			symbols "On"
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
			defines {"DA_DEBUG", "DA_REVIEW"}
			runtime "Debug"
			symbols "On"
		filter "configurations:Release"
			defines {"DA_RELEASE", "DA_REVIEW"}
			runtime "Release"
			optimize "On"
			symbols "On"
		filter "configurations:Final"
			defines "DA_FINAL"
			runtime "Release"
			optimize "On"
				
	filter "system:ios"
		architecture "ARM"

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
			defines {"DA_DEBUG", "DA_REVIEW"}
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines {"DA_RELEASE", "DA_REVIEW"}
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
			defines {"DA_DEBUG", "DA_REVIEW"}
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines {"DA_RELEASE", "DA_REVIEW"}
			runtime "Release"
			optimize "On"
		
		filter "configurations:Final"
			defines "DA_FINAL"
			runtime "Release"
			optimize "On"
