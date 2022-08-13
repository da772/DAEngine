newoption {
	trigger = "unit-test",
	description = "builds for unit tests"
}

if not _OPTIONS['unit-test'] then
group "ThirdParty"
filter "system:macosx"
	include "thirdparty/GLFW"
	include "thirdparty/DearImGui"
	include "thirdparty/zlib"
	include "thirdparty/assimp"
filter "system:windows"
	include "thirdparty/GLFW"
	include "thirdparty/DearImGui"
	include "thirdparty/zlib"
	include "thirdparty/assimp"
filter "system:linux"
	include "thirdparty/GLFW"
	include "thirdparty/DearImGui"
	include "thirdparty/zlib"
	include "thirdparty/assimp"
group ""

IncludeDir = {}
	IncludeDir["GLFW"] = "%{wks.location}/DAEngine/thirdparty/GLFW/include"
	IncludeDir["Vulkan"] = "%{wks.location}/DAEngine/thirdparty/Vulkan"
	IncludeDir["ImGui"] = "%{wks.location}/DAEngine/thirdparty/DearImGui"
	IncludeDir["glm"] = "%{wks.location}/DAEngine/thirdparty/glm"
	IncludeDir["stb"] = "%{wks.location}/DAEngine/thirdparty/stb"
	IncludeDir["assimp"] = "%{wks.location}/DAEngine/thirdparty/assimp/include"
	IncludeDir["zlib"] = "%{wks.location}/DAEngine/thirdparty/zlib/include"
end

project "DAEngine"
	kind "StaticLib"
	language "C++"
	cppdialect "C++20"
	staticruntime "on"

	pchheader "src/dapch.h"
	pchsource "src/dapch.cpp"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
	objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

	files 
	{
		"src/**.h",
		"src/**.cpp"
	}
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS",
	}

	if not _OPTIONS['unit-test'] then
	includedirs
	{
		"%{prj.location}/src",
		"%{prj.location}/src/DAEngine",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.zlib}",
		"%{IncludeDir.assimp}"
	}
	
	libdirs
	{
		"%{IncludeDir.Vulkan}/lib"
	}
	else
	defines
	{
		"DA_TEST"
	}
	includedirs
	{
		"%{prj.location}/src",
		"%{prj.location}/src/DAEngine"
	}
	end

	filter "system:macosx"
		defines
		{
			"DA_PLATFORM_MACOSX"
		}

		xcodebuildsettings
		{ 
			["ALWAYS_SEARCH_USER_PATHS"] = "YES",
			["SKIP_INSTALL"] = "YES"
		}

		if not _OPTIONS['unit-test'] then
		links 
		{
			"GLFW",
			"libvulkan.1.3.216.dylib",
			"libMoltenVK.dylib",
			"ImGui"
		}
		
		includedirs
		{
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.Vulkan}/include",
			"%{IncludeDir.ImGui}"
		}
		
		libdirs
		{
			"%{IncludeDir.Vulkan}/lib"
		}
		end

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
		
		pchheader "dapch.h"
		pchsource "src/dapch.cpp"
		
		
		defines
		{
			"DA_PLATFORM_WINDOWS"
		}
		
		if not _OPTIONS['unit-test'] then
		links
		{
			"GLFW",
			"vulkan-1",
			"ImGui"
		}

		
		libdirs
		{
			"%{IncludeDir.Vulkan}/lib",
			"%{IncludeDir.ImGui}"
		}
		
		includedirs
		{
			"%{IncludeDir.GLFW}",
			"%{IncludeDir.Vulkan}/include",
			"%{IncludeDir.ImGui}"
		}
		end

		filter "configurations:Debug"
			defines "DA_DEBUG"
			runtime "Debug"
			symbols "On"
		filter "configurations:Release"
			defines "DA_RELEASE"
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
			
			if not _OPTIONS['unit-test'] then
			links
			{
				"GLFW",
				"vulkan-1",
				"ImGui"
			}
			
			includedirs
			{
				"%{IncludeDir.GLFW}",
				"%{IncludeDir.Vulkan}/include",
				"%{IncludeDir.ImGui}"
			}
			end
			
			filter "configurations:Debug"
				defines "DA_DEBUG"
				runtime "Debug"
				symbols "On"
			filter "configurations:Release"
				defines "DA_RELEASE"
				runtime "Release"
				optimize "On"
				symbols "On"
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
