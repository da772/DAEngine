project "recast"
  kind "StaticLib"
  language "C++"
  cppdialect "C++20"
  staticruntime "on"

  targetdir ("bin/" .. outputdir .. "/%{prj.name}")
  objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

  files {
     "DebugUtils/Source/**.cpp",
     "DebugUtils/Source/**.hpp",
     "DebugUtils/Source/**.h",

     "Detour/Source/**.cpp",
     "Detour/Source/**.hpp",
     "Detour/Source/**.h",

     "DetourCrowd/Source/**.cpp",
     "DetourCrowd/Source/**.hpp",
     "DetourCrowd/Source/**.h",

     "DetourTileCache/Source/**.cpp",
     "DetourTileCache/Source/**.hpp",
     "DetourTileCache/Source/**.h",

     "Recast/Source/**.cpp",
     "Recast/Source/**.hpp",
     "Recast/Source/**.h",
  }

  includedirs {
      "DebugUtils/Include",
      "Detour/Include",
      "DetourCrowd/Include",
      "DetourTileCache/Include",
      "Recast/Include",
  }

  filter "system:macosx"
        xcodebuildsettings
        { 
            ["ALWAYS_SEARCH_USER_PATHS"] = "YES",
            ["SKIP_INSTALL"] = "YES"
        }
        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
			
        filter "configurations:Release"
            runtime "Release"
            optimize "On"

    filter "system:android"
        cppdialect "gnu++17"
        linkoptions { "-lm" }
        exceptionhandling ("On")
        rtti ("On")
        androidapilevel(android_version)
        
        filter "platforms:x86"
            architecture "x86"
        filter "platforms:ARM"
            architecture "ARM"
        filter "platforms:ARM64"
            architecture "ARM64"
        filter "platforms:x64"
			architecture "x64"


        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"

    filter "system:ios"
		xcodebuildsettings
		{ 
			["ALWAYS_SEARCH_USER_PATHS"] = "YES",
            ["ARCHS"] = "$(ARCHS_STANDARD)",
            ["SKIP_INSTALL"] = "YES"
        }
        
        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"

    filter "system:windows"
        systemversion "latest"

        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"

    filter "system:linux"
        systemversion "latest"

        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"
