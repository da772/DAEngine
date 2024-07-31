project "lua51"
    kind "StaticLib"
    language "C"
    staticruntime "on"
			
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-Obj/" .. outputdir .. "/%{prj.name}")

    files {
		"src/**.c",
		"src/**.h",
	}

    defines {
		"_CRT_SECURE_NO_DEPRECATE",
		"_CRT_STDIO_INLINE=__declspec(dllexport)__inline"
	}


    filter "system:macosx"
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
			symbols "On"
		filter "configurations:Final"
			runtime "Release"
            optimize "On"
            
    filter "system:linux"
        pic "On"
    
        systemversion "latest"
        staticruntime "On"
		
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
    