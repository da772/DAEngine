project "LuaJIT"
    kind "StaticLib"
    compileas "C"
    targetname "luajit"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-Obj/" .. outputdir .. "/%{prj.name}")

	staticruntime "On"
	
    vectorextensions "SSE2"

    dependson "BuildVM" -- Add dependency to make sure its run before this
    links { "src/lj_*.obj", "src/lib_*.obj" } -- BuildVM generates object files, so lets link those as well..

    vpaths {
        ["Headers/*"] = "src/lj_*.h",
        ["Soruces/*"] = "src/lj_*.c",
        ["*"] = "luajit.premake5.lua",
    }

    files {
		"luajit.premake5.lua",
		"src/lj_*.c",
		"src/lib_*.c",
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

    filter "configurations:Debug"
        defines "LUA_USE_APICHECK" -- TODO uncomment above line