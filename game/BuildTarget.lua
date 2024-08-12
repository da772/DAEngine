CC="clang"
CXX="clang++"

workspace "game"
	architecture "x64"

	configurations
	{
		"Debug",
		"Release",
		"Final"
	}

	filter "system:android"
		android_version = 21
		platforms 
		{
			"ARM",
			"x86",
			"ARM64",
			"x64"
		}
	
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

include "DAEngine"

group "game"

project "game"
	location "game"
	kind "ConsoleApp"
	language "C++"
			cppdialect "C++latest"
	staticruntime "on"

	targetdir ("%{prj.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{prj.location}/bin-obj/" .. outputdir .. "/%{prj.name}")

	defines 
	{
		"DA_PROJECT_game"
	}
	
	files 
	{
		"DAEngine/src/DAEngine/headers/**.h",
		"%{prj.name}/src/**.h",
		"%{prj.name}/src/**.c",
		"%{prj.name}/src/**.cpp"
	}

	includedirs 
	{
		"%{IncludeDir['da-core']}",
		"DAEngine/src/DAEngine/headers",
		"%{prj.name}/src",
		"%{IncludeDir.stb}",
		"%{IncludeDir.assimp}",
		"%{IncludeDir.ImGui}",
		"%{IncludeDir.glm}",
		"%{IncludeDir.sol2}",
		"%{IncludeDir.lua51}/lua",
		"%{IncludeDir.recast}",
		"%{IncludeDir['recast-debugutils']}",
		"%{IncludeDir['recast-detour']}",
		"%{IncludeDir['recast-detour-crowd']}",
		"%{IncludeDir['recast-detour-tilecache']}",
		"%{IncludeDir.optick}",
	}

	links
	{
		"da-ai",
		"da-core",
		"da-debug",
		"da-graphics",
		"da-net",
		"da-physics",
		"da-platform",
		"da-script",
		"da-stl",
		"assimp",
		"ImGui",
		"bgfx",
		"bimg",
		"bx",
		"GLFW",
		"zlib",
		"optick"
	}
	
	filter "system:windows"
		systemversion "latest"
		kind "WindowedApp"
		
		flags
		{
			"MultiProcessorCompile"
		}

		defines
		{
			"DA_PLATFORM_WINDOWS"
		}

		links
		{
			"gdi32",
			"kernel32",
			"psapi",
		}

		filter "configurations:Debug"
			defines {"DA_DEBUG", "DA_REVIEW"}
			runtime "Debug"
			symbols "On"
			kind "ConsoleApp"
			
		filter "configurations:Release"
			defines {"DA_RELEASE", "DA_REVIEW"}
			runtime "Release"
			optimize "On"
			symbols "On"
			kind "ConsoleApp"
			
		filter "configurations:Final"
			defines "DA_Final"
			runtime "Release"
			optimize "On"

	filter "system:ios"
		architecture "ARM"
		kind "WindowedApp"
		
		defines
		{
			"DA_PLATFORM_IOS"
		}

		xcodebuildsettings
		{ 
			["ALWAYS_SEARCH_USER_PATHS"] = "YES",
			["ARCHS"] = "$(ARCHS_STANDARD)",
			["TARGETED_DEVICE_FAMILY"] = "1,2",
			["ASSETCATALOG_COMPILER_APPICON_NAME"] = "AppIcon",
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
			defines "DA_Final"
			runtime "Release"
			optimize "On"
	
	filter "system:android"
		toolset "clang"
		cppdialect "gnu++20"
		kind "SharedLib"
		linkoptions { "-lm"}
		rtti ("On")
		exceptionhandling ("On")

		androidapilevel(android_version)
		defines
		{
			"DA_PLATFORM_ANDROID"
		}

		androidLibDir = ""

		filter "configurations:Debug"
			defines {"DA_DEBUG", "DA_REVIEW"}
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines {"DA_RELEASE", "DA_REVIEW"}
			runtime "Release"
			optimize "On"
			
		filter "configurations:Final"
			defines "DA_Final"
			runtime "Release"
			optimize "On"

		filter "platforms:x86"
			architecture "x86"
			androidLibDir = "x86"
			postbuildcommands
			{
			"copy /y \"%{wks.location}%{cfg.architecture}\\%{cfg.buildcfg}\\lib%{prj.name}.so\"  \"%{prj.location}AndroidStudio\\app\\src\\main\\jniLibs\\" .. androidLibDir .. "\"",
			"XCOPY /I /E /S /Y \"%{wks.location}%{cfg.architecture}\" \"%{prj.location}/bin/" .. outputdir .. "/%{prj.name}\"",
			"RMDIR /Q/S \"%{wks.location}%{cfg.architecture}\"",
			"XCOPY /I /E /S /Y \"$(ProjectDir)%{prj.name}/Data\" \"%{prj.location}AndroidStudio\\app\\src\\main\\assets\\Data\\\""
			}
		filter "platforms:x64"
			architecture "x64"
			libdirs
			{
				"ImpulseEngine/%{IncludeDir.firebase}/lib/android/libx64"
			}
			androidLibDir = "x86_64"
			postbuildcommands
			{
			"copy /y \"%{wks.location}%{cfg.architecture}\\%{cfg.buildcfg}\\lib%{prj.name}.so\"  \"%{prj.location}AndroidStudio\\app\\src\\main\\jniLibs\\" .. androidLibDir .. "\"",
			"XCOPY /I /E /S /Y \"%{wks.location}%{cfg.architecture}\" \"%{prj.location}/bin/" .. outputdir .. "/%{prj.name}\"",
			"RMDIR /Q/S \"%{wks.location}%{cfg.architecture}\"",
			"XCOPY /I /E /S /Y \"$(ProjectDir)%{prj.name}/Data\" \"%{prj.location}AndroidStudio\\app\\src\\main\\assets\\Data\\\""
			}
			
		filter "platforms:ARM"
			architecture "ARM"
			libdirs
			{
				"ImpulseEngine/%{IncludeDir.firebase}/lib/android/libarm"
			}
			androidLibDir = "armeabi-v7a"
			postbuildcommands
			{
			"copy /y \"%{wks.location}%{cfg.architecture}\\%{cfg.buildcfg}\\lib%{prj.name}.so\"  \"%{prj.location}AndroidStudio\\app\\src\\main\\jniLibs\\" .. androidLibDir .. "\"",
			"XCOPY /I /E /S /Y \"%{wks.location}%{cfg.architecture}\" \"%{prj.location}/bin/" .. outputdir .. "/%{prj.name}\"",
			"RMDIR /Q/S \"%{wks.location}%{cfg.architecture}\"",
			"XCOPY /I /E /S /Y \"$(ProjectDir)%{prj.name}/Data\" \"%{prj.location}AndroidStudio\\app\\src\\main\\assets\\Data\\\""
			}

		filter "platforms:ARM64"
			architecture "ARM64"
			libdirs
			{
				"ImpulseEngine/%{IncludeDir.firebase}/lib/android/libarm64"
			}
			androidLibDir = "arm64-v8a"
			postbuildcommands
			{
			"copy /y \"%{wks.location}%{cfg.architecture}\\%{cfg.buildcfg}\\lib%{prj.name}.so\"  \"%{prj.location}AndroidStudio\\app\\src\\main\\jniLibs\\" .. androidLibDir .. "\"",
			"XCOPY /I /E /S /Y \"%{wks.location}%{cfg.architecture}\" \"%{prj.location}/bin/" .. outputdir .. "/%{prj.name}\"",
			"RMDIR /Q/S \"%{wks.location}%{cfg.architecture}\"",
			"XCOPY /I /E /S /Y \"$(ProjectDir)%{prj.name}/Data\" \"%{prj.location}AndroidStudio\\app\\src\\main\\assets\\Data\\\""
			}

	filter "system:macosx"
		makesettings [[
			CC = clang
			CXX = clang++
		]]

		defines
		{
			"DA_PLATFORM_MACOSX"
		}

		xcodebuildsettings
		{ 
			["ALWAYS_SEARCH_USER_PATHS"] = "YES"
		}

		if not _OPTIONS['unit-test'] then
			links 
			{
				"Cocoa.framework",
				"IOKit.framework",
				"CoreVideo.framework",
				"QuartzCore.framework",
				"Metal.framework",
				"lua51"
			}

			filter "action:gmake2"
				links
				{
					"vulkan.1.3.216",
					"MoltenVK",
				}
			
			filter "action:xcode4"
				links
				{
					"libvulkan.1.3.216.dylib",
					"libMoltenVK.dylib",
				}
			
			includedirs
			{
				"%{IncludeDir.Vulkan}/include"
			}
			
			libdirs
			{
				"%{IncludeDir.Vulkan}/lib"
			}
		end

		filter "configurations:Debug"
			defines {"DA_DEBUG", "DA_REVIEW"}
			runtime "Debug"
			symbols "On"
			debugger "LLDB"
			
			
		filter "configurations:Release"
			defines {"DA_RELEASE", "DA_REVIEW"}
			runtime "Release"
			optimize "On"
			
		filter "configurations:Final"
			defines "DA_Final"
			runtime "Release"
			optimize "On"
				
		filter "system:linux"
			linkgroups 'on'
			systemversion "latest"
			cppdialect "gnu++20"
			
			defines
			{
				
				"DA_PLATFORM_LINUX",
			}

			links
			{
				"lua51"
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

group ""