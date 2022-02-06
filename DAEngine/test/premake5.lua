workspace "UnitTest"
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

include "../../DAEngine"

project "DAEngineTests"
	kind "ConsoleApp"
	language "C++"
			cppdialect "C++20"
	staticruntime "on"

	targetdir ("%{prj.location}/bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{prj.location}/bin-obj/" .. outputdir .. "/%{prj.name}")

	defines 
	{
		"DA_TEST"
	}
	
	files 
	{
		"src/**.h",
		"src/**.c",
		"src/**.cpp"
	}

	includedirs 
	{
		"../src",
		"src"
	}

	links
	{
		"DAEngine"
	}
	
	filter "system:windows"
		systemversion "latest"
		kind "ConsoleApp"

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
			defines "DA_DEBUG"
			runtime "Debug"
			symbols "On"
			
		filter "configurations:Release"
			defines "DA_RELEASE"
			runtime "Release"
			optimize "On"
			
		filter "configurations:Final"
			defines "DA_Final"
			runtime "Release"
			optimize "On"
	
	filter "system:android"
		toolset "clang"
		cppdialect "gnu++17"
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
			defines "DA_DEBUG"
			runtime "Debug"
			symbols "On"

		filter "configurations:Release"
			defines "DA_RELEASE"
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
		systemversion "latest"
		kind "ConsoleApp"

		defines
		{
			"DA_PLATFORM_MACOSX"
		}

		xcodebuildsettings
		{ 
			["ALWAYS_SEARCH_USER_PATHS"] = "YES"
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
