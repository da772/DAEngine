newoption {
	trigger = "unit-test",
	description = "builds for unit tests"
}

if not _OPTIONS['unit-test'] then
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/DAEngine/thirdparty/GLFW/include"
IncludeDir["Vulkan"] = "%{wks.location}/DAEngine/thirdparty/Vulkan"
IncludeDir["ImGui"] = "%{wks.location}/DAEngine/thirdparty/DearImGui"
IncludeDir["glm"] = "%{wks.location}/DAEngine/thirdparty/glm"
IncludeDir["stb"] = "%{wks.location}/DAEngine/thirdparty/stb"
IncludeDir["assimp"] = "%{wks.location}/DAEngine/thirdparty/assimp/include"
IncludeDir["zlib"] = "%{wks.location}/DAEngine/thirdparty/zlib/include"
IncludeDir["ktx"] = "%{wks.location}/DAEngine/thirdparty/ktx/include"
IncludeDir["bx"] = "%{wks.location}/DAEngine/thirdparty/bx/include"
IncludeDir["bimg"] = "%{wks.location}/DAEngine/thirdparty/bimg/include"
IncludeDir["bgfx"] = "%{wks.location}/DAEngine/thirdparty/bgfx/include"
IncludeDir["luajit"] = "%{wks.location}/DAEngine/thirdparty/luajit/include"
IncludeDir["lua51"] = "%{wks.location}/DAEngine/thirdparty/lua51/include"

group "ThirdParty"
if os.host() == "macosx" then
	include "thirdparty/GLFW"
	include "thirdparty/DearImGui"
	include "thirdparty/zlib"
	include "thirdparty/assimp"
	include "thirdparty/ktx"
	include "thirdparty/bx"
	include "thirdparty/bimg"
	include "thirdparty/bgfx"
	include "thirdparty/lua51"
end

if os.host() == "windows" then
	include "thirdparty/GLFW"
	include "thirdparty/DearImGui"
	include "thirdparty/zlib"
	include "thirdparty/assimp"
	include "thirdparty/ktx"
	include "thirdparty/bx"
	include "thirdparty/bimg"
	include "thirdparty/bgfx"
	include "thirdparty/luajit"
end

if os.host() == "linux" then
	include "thirdparty/GLFW"
	include "thirdparty/DearImGui"
	include "thirdparty/zlib"
	include "thirdparty/assimp"
	include "thirdparty/ktx"
	include "thirdparty/bx"
	include "thirdparty/bimg"
	include "thirdparty/bgfx"
	include "thirdparty/lua51"
end

group ""
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

	if not _OPTIONS['unit-test'] then
	files 
	{
		"src/**.h",
		"src/**.cpp"
	}
	else
	files 
	{
		"src/*.h",
		"src/*.cpp",
		"src/DAEngine/*.h",
		"src/DAEngine/*.cpp",
		"src/DAEngine/Core/**.h",
		"src/DAEngine/Core/**.cpp"
	}
	end
	
	defines
	{
		"_CRT_SECURE_NO_WARNINGS"
	}

	if not _OPTIONS['unit-test'] then
	includedirs
	{
		"%{prj.location}/src",
		"%{prj.location}/src/DAEngine",
		"%{IncludeDir.glm}",
		"%{IncludeDir.stb}",
		"%{IncludeDir.zlib}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.ktx}",
		"%{IncludeDir.bx}",
		"%{IncludeDir.bimg}",
		"%{IncludeDir.bgfx}",
		"%{IncludeDir.lua51}"
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
			"GLFW",
			"ImGui",
			"ktx",
			"bx",
			"bimg",
			"bgfx",
			"lua51",
			"QuartzCore.framework",
			"Metal.framework",
			"Cocoa.framework",
			"IOKit.framework",
			"CoreVideo.framework"
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
			"DA_PLATFORM_WINDOWS",
			"GLFW_EXPOSE_NATIVE_WIN32"
		}
		
		if not _OPTIONS['unit-test'] then
		links
		{
			"GLFW",
			"vulkan-1",
			"ImGui",
			"ktx",
			"gdi32",
			"kernel32",
			"psapi",
			"bx",
			"bimg",
			"bgfx",
			"luajit"
		}

		buildoptions
		{
			"/Zc:__cplusplus"
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
			"ImGui",
			"ktx",
			"dl",
			"GL",
			"pthread",
			"X11",
			"lua51"
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
