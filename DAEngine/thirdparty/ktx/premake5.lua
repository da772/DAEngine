project "ktx"
    kind "StaticLib"
    language "C"
    staticruntime "on"
			
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

	files
	{
        "include/**.h",     
        "lib/**.c",
        "lib/**.h"
    }
	
	includedirs
	{
		"include",
		"other_include",
		"lib",
		"%{IncludeDir.Vulkan}/include"
	}


    filter "system:macosx"
        xcodebuildsettings
        { 
            ["ALWAYS_SEARCH_USER_PATHS"] = "YES",
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
	

   