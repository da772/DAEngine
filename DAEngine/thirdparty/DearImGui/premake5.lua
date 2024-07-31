project "ImGui"
    kind "StaticLib"
    language "C++"
	cppdialect "C++20"
    staticruntime "On"
    
    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-Obj/" .. outputdir .. "/%{prj.name}")

    files
    {
      	"imconfig.h",
		"imgui.h",
		"imgui.cpp",
		"imgui_draw.cpp",
		"imgui_internal.h",
		"imgui_tables.cpp",
		"imgui_widgets.cpp",
		"imstb_rectpack.h",
		"imstb_textedit.h",
		"imstb_truetype.h",
		"imgui_demo.cpp"
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

    filter "system:linux"
        systemversion "latest"

        filter "configurations:Debug"
            runtime "Debug"
            symbols "On"
        filter "configurations:Release"
            runtime "Release"
            optimize "On"
